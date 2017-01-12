// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2015-2016 The Gulden developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// This file contains Delta, the Gulden Difficulty Re-adjustment algorithm developed by Frank (dt_cdog@yahoo.com) with various enhancements by Malcolm MacLeod (mmacleod@webmail.co.za)
// The core algorithm works by taking time measurements of four periods (last block; short window; medium window; long window) and then apply a weighting to them.
// This allows the algorithm to react to short term fluctuations while still taking long term block targets into account, which helps prevent it from overreacting.
//
// In addition to the core algorithm several extra rules are then applied in certain situations (e.g. multiple quick blocks) to enhance the behaviour.
//

#include "pow.h"

#include "chainparams.h"
#include "main.h"
#include "uint256.h"
#include "util.h"

#include <math.h>
#include <stdint.h> 

// #include "arith_uint256.h"

int nTargetSpacing = 60;
static int64_t nTargetTimespan = 2 * 60;

uint256 bnProofOfStakeLimit(~uint256(0) >> 20);

static uint256 GetProofOfStakeLimit(int nHeight)
{
    return bnProofOfStakeLimit;
}
/*
static unsigned int DeltaRetargetingAlgorithm(const INDEX_TYPE pindexLast, bool fProofOfStake,
													 int nTargetSpacing, unsigned int nFirstDeltaBlock) {

	  uint256 bnTargetLimit = GetProofOfStakeLimit(pindexLast->nHeight);

      int64_t nRetargetTimespan = nTargetSpacing;

      const unsigned int nLastBlock = 1;
      const unsigned int nShortFrame = 3;
      const unsigned int nMiddleFrame = 24;
      const unsigned int nLongFrame = 576;

      const int64_t nLBWeight = 64;
      const int64_t nShortWeight = 8;
      int64_t nMiddleWeight = 2;
      int64_t nLongWeight = 1;

      const int64_t nLBMinGap = nRetargetTimespan / 6;
      const int64_t nLBMaxGap = nRetargetTimespan * 6;

      const int64_t nQBFrame = nShortFrame + 1;
      const int64_t nQBMinGap = (nRetargetTimespan * PERCENT_FACTOR / 120) * nQBFrame;

      const int64_t nBadTimeLimit = 0;
      const int64_t nBadTimeReplace = nRetargetTimespan / 10;

      const int64_t nLowTimeLimit = nRetargetTimespan * 90 / PERCENT_FACTOR;
      const int64_t nFloorTimeLimit = nRetargetTimespan * 65 / PERCENT_FACTOR;

      const int64_t nDrift = 1;
      int64_t nLongTimeLimit = ((6 * nDrift)) * 60;
      int64_t nLongTimeStep = nDrift * 60;

      unsigned int nMinimumAdjustLimit = (unsigned int) nRetargetTimespan * 75 / PERCENT_FACTOR;

      unsigned int nMaximumAdjustLimit = (unsigned int) nRetargetTimespan * 150 / PERCENT_FACTOR;

      int64_t nDeltaTimespan = 0;
      int64_t nLBTimespan = 0;
      int64_t nShortTimespan = 0;
      int64_t nMiddleTimespan = 0;
      int64_t nLongTimespan = 0;
      int64_t nQBTimespan = 0;

      int64_t nWeightedSum = 0;
      int64_t nWeightedDiv = 0;
      int64_t nWeightedTimespan = 0;

      const INDEX_TYPE pindexFirst = pindexLast; //multi algo - last block is selected on a per algo basis.

      if (pindexLast == NULL)
        return bnTargetLimit.GetCompact(); // genesis block;

      if (INDEX_HEIGHT(pindexLast) <= nQBFrame)
        return bnTargetLimit.GetCompact(); // genesis block;

      pindexFirst = INDEX_PREV(pindexLast);
      nLBTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(pindexFirst);

      if (nLBTimespan > nBadTimeLimit && nLBTimespan < nLBMinGap)
        nLBTimespan = nLBTimespan * 50 / PERCENT_FACTOR;

      if (nLBTimespan <= nBadTimeLimit)
        nLBTimespan = nBadTimeReplace;

      if (nLBTimespan > nLBMaxGap)
        nLBTimespan = nLBTimespan * 150 / PERCENT_FACTOR;

      pindexFirst = pindexLast;
      for (unsigned int i = 1; pindexFirst != NULL && i <= nQBFrame; i++) {
        nDeltaTimespan = INDEX_TIME(pindexFirst) - INDEX_TIME(INDEX_PREV(pindexFirst));

        if (nDeltaTimespan <= nBadTimeLimit)
          nDeltaTimespan = nBadTimeReplace;

        if (i <= nShortFrame)
          nShortTimespan += nDeltaTimespan;
        nQBTimespan += nDeltaTimespan;
        pindexFirst = INDEX_PREV(pindexFirst);
      }

      if (INDEX_HEIGHT(pindexLast) - nFirstDeltaBlock <= nMiddleFrame) {
        nMiddleWeight = nMiddleTimespan = 0;
      } else {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst != NULL && i <= nMiddleFrame; i++) {
          nDeltaTimespan = INDEX_TIME(pindexFirst) - INDEX_TIME(INDEX_PREV(pindexFirst));

          if (nDeltaTimespan <= nBadTimeLimit)
            nDeltaTimespan = nBadTimeReplace;

          nMiddleTimespan += nDeltaTimespan;
          pindexFirst = INDEX_PREV(pindexFirst);
        }
      }

      if (INDEX_HEIGHT(pindexLast) - nFirstDeltaBlock <= nLongFrame) {
        nLongWeight = nLongTimespan = 0;
      } else {
        pindexFirst = pindexLast;
        for (unsigned int i = 1; pindexFirst != NULL && i <= nLongFrame; i++)
          pindexFirst = INDEX_PREV(pindexFirst);

        nLongTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(pindexFirst);
      }

      if ((nQBTimespan > nBadTimeLimit) && (nQBTimespan < nQBMinGap) && (nLBTimespan < nRetargetTimespan * 40 / PERCENT_FACTOR)) {
			nMiddleWeight = nMiddleTimespan = nLongWeight = nLongTimespan = 0;
      }

      nWeightedSum = (nLBTimespan * nLBWeight) + (nShortTimespan * nShortWeight);
      nWeightedSum += (nMiddleTimespan * nMiddleWeight) + (nLongTimespan * nLongWeight);
      nWeightedDiv = (nLastBlock * nLBWeight) + (nShortFrame * nShortWeight);
      nWeightedDiv += (nMiddleFrame * nMiddleWeight) + (nLongFrame * nLongWeight);
      nWeightedTimespan = nWeightedSum / nWeightedDiv;

      if (DIFF_ABS(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 20 / PERCENT_FACTOR) {

        nMinimumAdjustLimit = (unsigned int) nRetargetTimespan * 90 / PERCENT_FACTOR;
        nMaximumAdjustLimit = (unsigned int) nRetargetTimespan * 110 / PERCENT_FACTOR;
      } else if (DIFF_ABS(nLBTimespan - nRetargetTimespan) < nRetargetTimespan * 30 / PERCENT_FACTOR) {

        nMinimumAdjustLimit = (unsigned int) nRetargetTimespan * 80 / PERCENT_FACTOR;
        nMaximumAdjustLimit = (unsigned int) nRetargetTimespan * 120 / PERCENT_FACTOR;
      }

      if (nWeightedTimespan < nMinimumAdjustLimit)
        nWeightedTimespan = nMinimumAdjustLimit;

      if (nWeightedTimespan > nMaximumAdjustLimit)
        nWeightedTimespan = nMaximumAdjustLimit;

      arith_uint256 bnNew;
      SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
      bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(nWeightedTimespan));
      bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(nRetargetTimespan));

      nLBTimespan = INDEX_TIME(pindexLast) - INDEX_TIME(INDEX_PREV(pindexLast));
      arith_uint256 bnComp;
      SET_COMPACT(bnComp, INDEX_TARGET(pindexLast));
      if (nLBTimespan > 0 && nLBTimespan < nLowTimeLimit && BIGINT_GREATER_THAN(bnNew, bnComp)) {

        if (nLBTimespan < nFloorTimeLimit) {
          SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
          bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(95));
          bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(PERCENT_FACTOR));
        } else {
          SET_COMPACT(bnNew, INDEX_TARGET(pindexLast));
        }
      }

      /*if ((BLOCK_TIME(block) - INDEX_TIME(pindexLast)) > nLongTimeLimit) {

        int64_t nNumMissedSteps = ((BLOCK_TIME(block) - INDEX_TIME(pindexLast) - nLongTimeLimit) / nLongTimeStep) + 1;
        for (int i = 0; i < nNumMissedSteps; ++i) {
          bnNew = BIGINT_MULTIPLY(bnNew, arith_uint256(110));
          bnNew = BIGINT_DIVIDE(bnNew, arith_uint256(PERCENT_FACTOR));
        }
      }

	  // TODO: RE-EXAMINE RISKS!
	  
      SET_COMPACT(bnComp, bnTargetLimit.GetLow64());
      if (BIGINT_GREATER_THAN(bnNew, bnComp))
        SET_COMPACT(bnNew, bnTargetLimit.GetLow64());

      return GET_COMPACT(bnNew);
}
*/
unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake)
{
    uint256 bnTargetLimit = fProofOfStake ? GetProofOfStakeLimit(pindexLast->nHeight) : Params().ProofOfWorkLimit();

    if (pindexLast == NULL)
        return bnTargetLimit.GetCompact(); // genesis block

    const CBlockIndex* pindexPrev = GetLastBlockIndex(pindexLast, fProofOfStake);
    if (pindexPrev->pprev == NULL)
        return bnTargetLimit.GetCompact(); // first block
    const CBlockIndex* pindexPrevPrev = GetLastBlockIndex(pindexPrev->pprev, fProofOfStake);
    if (pindexPrevPrev->pprev == NULL)
        return bnTargetLimit.GetCompact(); // second block

    int64_t nActualSpacing = pindexPrev->GetBlockTime() - pindexPrevPrev->GetBlockTime();
    if (nActualSpacing < 0)
        nActualSpacing = nTargetSpacing;

    if (nActualSpacing > nTargetSpacing * 10)
        nActualSpacing = nTargetSpacing * 10;

    // ppcoin: target change every block
    // ppcoin: retarget with exponential moving toward target spacing
	
	uint256 bnNew;
    uint256 bnOld;
	bnNew.SetCompact(pindexLast->nBits);
	bnOld = bnNew;

    int64_t nInterval = nTargetTimespan / nTargetSpacing;
    bnNew *= ((nInterval - 1) * nTargetSpacing + nActualSpacing + nActualSpacing);
    bnNew /= ((nInterval + 1) * nTargetSpacing);

    if (bnNew <= 0 || bnNew > bnTargetLimit)
        bnNew = bnTargetLimit;

	return bnNew.GetCompact();
	//return DeltaRetargetingAlgorithm(pindexLast, fProofOfStake, nTargetSpacing, 1);
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
