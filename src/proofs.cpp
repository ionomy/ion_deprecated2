// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2015-2017 The Silk Network developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "proofs.h"

#include "chainparams.h"
#include "main.h"
#include "uint256.h"
#include "util.h"
#include "amount.h"
#include "checkpoints.h"

#include <math.h>
#include <stdint.h> 

unsigned int nStakeMinAge       = 1 * 60 * 60;
unsigned int nModifierInterval  = 2 * 60 * 60;

uint256 CBlock::GetHash() const {
	return GetPoWHash();
}

uint256 CBlock::GetPoWHash() const {
	return Hash(BEGIN(nVersion), END(nNonce));
}

int DetermineCoinbaseMaturity() {
	if(pindexBest->nHeight <= 100) {
		return (int)10; // This will allow for premine distribution to propogate faster
	} else {
		return (int)60; // Coinbase will take approx. 1 hr to reach confirmation
	}
}

// miner's coin base reward
int64_t GetCoinbaseValue(int nHeight, CAmount nFees)
{
    CAmount nSubsidy = 0;

	if(nHeight == 1) {
		nSubsidy = 16400000 * COIN;
	}

    return nSubsidy;
}

// miner's coin stake reward based on coin age spent (coin-days)
int64_t GetCoinstakeValue(int64_t nCoinAge, CAmount nFees, int nHeight)
{
	CAmount nSubsidy = 0.2 * COIN;

	if(nHeight <= 125146) {
		nSubsidy = 23 * COIN;
	} else if(nHeight <= 568622) {
		nSubsidy = 17 * COIN;
	} else if(nHeight <= 1012098) {
		nSubsidy = 11.5 * COIN;
	} else if(nHeight <= 1455574) {
		nSubsidy = 5.75 * COIN;
	} else if(nHeight <= 3675950) {
		nSubsidy = 1.85 * COIN;
	} else {
		nSubsidy = 0.2 * COIN;
	}

    return nSubsidy + nFees;
}

unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake)
{
    if (pindexLast->nHeight < Params().Fork1Height()){
        return GetNextTargetRequiredV1(pindexLast, fProofOfStake);
    } else {
        return GetNextTargetRequiredV2(pindexLast, fProofOfStake);
    }
    
}

unsigned int GetNextTargetRequiredV1(const CBlockIndex* pindexLast, bool fProofOfStake)
{
    uint256 bnTargetLimit = fProofOfStake ? Params().ProofOfStakeLimit() : Params().ProofOfWorkLimit();

    if (pindexLast == NULL)
        return bnTargetLimit.GetCompact(); // genesis block

    const CBlockIndex* pindexPrev = GetLastBlockIndex(pindexLast, fProofOfStake);
    if (pindexPrev->pprev == NULL)
        return bnTargetLimit.GetCompact(); // first block
    const CBlockIndex* pindexPrevPrev = GetLastBlockIndex(pindexPrev->pprev, fProofOfStake);
    if (pindexPrevPrev->pprev == NULL)
        return bnTargetLimit.GetCompact(); // second block

    int64_t nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();

    if (nActualSpacing < 0) {
        nActualSpacing = Params().TargetSpacing();
    }
    else if (fProofOfStake && nActualSpacing > Params().TargetSpacing() * 10) {
         nActualSpacing = Params().TargetSpacing() * 10;
    }

    // target change every block
    // retarget with exponential moving toward target spacing
    // Includes fix for wrong retargeting difficulty by Mammix2
    uint256 bnNew;
    bnNew.SetCompact(pindexPrev->nBits);

    int64_t nInterval = fProofOfStake ? 10 : 10;
    bnNew *= ((nInterval - 1) * Params().TargetSpacing() + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * Params().TargetSpacing());

    if (bnNew <= 0 || bnNew > bnTargetLimit)
        bnNew = bnTargetLimit;

    return bnNew.GetCompact();
}

// This is MIDAS (Multi Interval Difficulty Adjustment System), a novel getnextwork algorithm.  It responds quickly to
// huge changes in hashing power, is immune to time warp attacks, and regulates the block rate to keep the block height
// close to the block height expected given the nominal block interval and the elapsed time.  How close the
// correspondence between block height and wall clock time is, depends on how stable the hashing power has been.  Maybe
// Bitcoin can wait 2 weeks between updates but no altcoin can.

// It is important that none of these intervals (5, 7, 9, 17) have any common divisor; eliminating the existence of
// harmonics is an important part of eliminating the effectiveness of timewarp attacks.
void avgRecentTimestamps(const CBlockIndex* pindexLast, int64_t *avgOf5, int64_t *avgOf7, int64_t *avgOf9, int64_t *avgOf17)
{
  int blockoffset = 0;
  int64_t oldblocktime;
  int64_t blocktime;

  *avgOf5 = *avgOf7 = *avgOf9 = *avgOf17 = 0;
  if (pindexLast)
    blocktime = pindexLast->GetBlockTime();
  else blocktime = 0;

  for (blockoffset = 0; blockoffset < 17; blockoffset++)
  {
    oldblocktime = blocktime;
    if (pindexLast)
    {
      pindexLast = pindexLast->pprev;
      blocktime = pindexLast->GetBlockTime();
    }
    else
    { // genesis block or previous
    blocktime -= Params().TargetSpacing();
    }
    // for each block, add interval.
    if (blockoffset < 5) *avgOf5 += (oldblocktime - blocktime);
    if (blockoffset < 7) *avgOf7 += (oldblocktime - blocktime);
    if (blockoffset < 9) *avgOf9 += (oldblocktime - blocktime);
    *avgOf17 += (oldblocktime - blocktime);    
  }
  // now we have the sums of the block intervals. Division gets us the averages. 
  *avgOf5 /= 5;
  *avgOf7 /= 7;
  *avgOf9 /= 9;
  *avgOf17 /= 17;
}

//unsigned int GetNextWorkRequired(const CBlockIndex *pindexLast, const CBlockHeader *pblock)
unsigned int GetNextTargetRequiredV2(const CBlockIndex* pindexLast, bool fProofOfStake)
{
    int64_t avgOf5;
    int64_t avgOf9;
    int64_t avgOf7;
    int64_t avgOf17;
    int64_t toofast;
    int64_t tooslow;
    int64_t difficultyfactor = 10000;
    int64_t now;
    int64_t BlockHeightTime;

    int64_t nFastInterval = (Params().TargetSpacing() * 9 ) / 10; // seconds per block desired when far behind schedule
    int64_t nSlowInterval = (Params().TargetSpacing() * 11) / 10; // seconds per block desired when far ahead of schedule
    int64_t nIntervalDesired;

    uint256 bnTargetLimit = fProofOfStake ? Params().ProofOfStakeLimit() : Params().ProofOfWorkLimit();
    unsigned int nTargetLimit = bnTargetLimit.GetCompact();
    
    if (pindexLast == NULL)
        // Genesis Block
        return nTargetLimit;
   
    // Regulate block times so as to remain synchronized in the long run with the actual time.  The first step is to
    // calculate what interval we want to use as our regulatory goal.  It depends on how far ahead of (or behind)
    // schedule we are.  If we're more than an adjustment period ahead or behind, we use the maximum (nSlowInterval) or minimum
    // (nFastInterval) values; otherwise we calculate a weighted average somewhere in between them.  The closer we are
    // to being exactly on schedule the closer our selected interval will be to our nominal interval (TargetSpacing).

    now = pindexLast->GetBlockTime();
    BlockHeightTime = Params().GenesisBlock().nTime + pindexLast->nHeight * Params().TargetSpacing();
    
    if (now < BlockHeightTime + Params().TargetTimespan() && now > BlockHeightTime )
    // ahead of schedule by less than one interval.
    nIntervalDesired = ((Params().TargetTimespan() - (now - BlockHeightTime)) * Params().TargetSpacing() +  
                (now - BlockHeightTime) * nFastInterval) / Params().TargetSpacing();
    else if (now + Params().TargetTimespan() > BlockHeightTime && now < BlockHeightTime)
    // behind schedule by less than one interval.
    nIntervalDesired = ((Params().TargetTimespan() - (BlockHeightTime - now)) * Params().TargetSpacing() + 
                (BlockHeightTime - now) * nSlowInterval) / Params().TargetTimespan();

    // ahead by more than one interval;
    else if (now < BlockHeightTime) nIntervalDesired = nSlowInterval;
    
    // behind by more than an interval. 
    else  nIntervalDesired = nFastInterval;
    
    // find out what average intervals over last 5, 7, 9, and 17 blocks have been. 
    avgRecentTimestamps(pindexLast, &avgOf5, &avgOf7, &avgOf9, &avgOf17);    

    // check for emergency adjustments. These are to bring the diff up or down FAST when a burst miner or multipool
    // jumps on or off.  Once they kick in they can adjust difficulty very rapidly, and they can kick in very rapidly
    // after massive hash power jumps on or off.
    
    // Important note: This is a self-damping adjustment because 8/5 and 5/8 are closer to 1 than 3/2 and 2/3.  Do not
    // screw with the constants in a way that breaks this relationship.  Even though self-damping, it will usually
    // overshoot slightly. But normal adjustment will handle damping without getting back to emergency.
    toofast = (nIntervalDesired * 2) / 3;
    tooslow = (nIntervalDesired * 3) / 2;

    // both of these check the shortest interval to quickly stop when overshot.  Otherwise first is longer and second shorter.
    if (avgOf5 < toofast && avgOf9 < toofast && avgOf17 < toofast)
    {  //emergency adjustment, slow down (longer intervals because shorter blocks)
      LogPrint("difficulty", "GetNextWorkRequired EMERGENCY RETARGET\n");
      difficultyfactor *= 8;
      difficultyfactor /= 5;
    }
    else if (avgOf5 > tooslow && avgOf7 > tooslow && avgOf9 > tooslow)
    {  //emergency adjustment, speed up (shorter intervals because longer blocks)
      LogPrint("difficulty", "GetNextWorkRequired EMERGENCY RETARGET\n");
      difficultyfactor *= 5;
      difficultyfactor /= 8;
    }

    // If no emergency adjustment, check for normal adjustment. 
    else if (((avgOf5 > nIntervalDesired || avgOf7 > nIntervalDesired) && avgOf9 > nIntervalDesired && avgOf17 > nIntervalDesired) ||
         ((avgOf5 < nIntervalDesired || avgOf7 < nIntervalDesired) && avgOf9 < nIntervalDesired && avgOf17 < nIntervalDesired))
    { // At least 3 averages too high or at least 3 too low, including the two longest. This will be executed 3/16 of
      // the time on the basis of random variation, even if the settings are perfect. It regulates one-sixth of the way
      // to the calculated point.
      LogPrint("difficulty", "GetNextWorkRequired RETARGET\n");
      difficultyfactor *= (6 * nIntervalDesired);
      difficultyfactor /= (avgOf17 +(5 * nIntervalDesired));
    }

    // limit to doubling or halving.  There are no conditions where this will make a difference unless there is an
    // unsuspected bug in the above code.
    if (difficultyfactor > 20000) difficultyfactor = 20000;
    if (difficultyfactor < 5000) difficultyfactor = 5000;

    uint256 bnNew;
    uint256 bnOld;

    bnOld.SetCompact(pindexLast->nBits);

    if (difficultyfactor == 10000) // no adjustment. 
      return(bnOld.GetCompact());

    bnNew = bnOld / difficultyfactor;
    bnNew *= 10000;

    if (bnNew > Params().ProofOfWorkLimit())
      bnNew = Params().ProofOfWorkLimit();

    LogPrint("difficulty", "Actual time %d, Scheduled time for this block height = %d\n", now, BlockHeightTime );
    LogPrint("difficulty", "Nominal block interval = %d, regulating on interval %d to get back to schedule.\n", 
          Params().TargetSpacing(), nIntervalDesired );
    LogPrint("difficulty", "Intervals of last 5/7/9/17 blocks = %d / %d / %d / %d.\n",
          avgOf5, avgOf7, avgOf9, avgOf17);
    LogPrint("difficulty", "Difficulty Before Adjustment: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("Difficulty After Adjustment:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
     
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    uint256 bnTarget;
    bnTarget.SetCompact(nBits);

    // Check range
    if (bnTarget <= 0 || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}
