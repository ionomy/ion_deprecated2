// Copyright (c) 2009-2017 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "proofs.h"
#include "txdb.h"
#include "main.h"
#include "uint256.h"

static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
	typedef std::map<int, uint256> MapCheckpoints;
    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (       0,	Params().HashGenesisBlock())
        (       1,	uint256("0x000000ed2f68cd6c7935831cc1d473da7c6decdb87e8b5dba0afff0b00002690") ) // Premine
        (      10,	uint256("0x00000032f5a96d31d74b380c0336445baccb73a01bdbedec868283019bad7016") ) // Confirmation of Premine
        (      22,	uint256("0x00000002e04f91402d78b84433ec744aacac5c40952b918fe09a7d623ac33967") )
        (      32,	uint256("0x0000001880da8fd09cc6f5e93315135fe686eb49f9054c807fa810d56ebb013b") )
        (      35,	uint256("0x0000000af6204fd43bb9cafea1dd192c245979d4dd7bde19efb92f633589ade5") )
        (      45,	uint256("0x00000006d6b9e9fba4dee10bc63ca7ea764c80c2b9c4fa6ddedb944eb288a371") )
	(    1000,	uint256("0x0000000144b22b0af9bced65256d5eccc4e3f112a89bdb0f08ab8dc2a6145b56") ) // Last POW Block
	(  100000,	uint256("0xa5a4e8f2554cda59955e0117fd71245e1f6d73a30ab420a3d3731384dcbcfffc") )
	(  150000,	uint256("0x13d5c4880d6005fe950ed447a0dcbe4613761b077e1b067048fc1893eaaf21c9") )
	(  176500,	uint256("0xe51d463e2fa39af6d57f54a9691507025163ec777d6ea9f3053b216876763398") ) // Fork June 2017
	(  180000,	uint256("0xe87b33c6bdda1de6f9d697c7751d706e33e531325aea8352958812a1e7bd1216") ) // Fork June 2017
	(  181150,	uint256("0xb11fcb73338b588a721cced4c23efa134a33b10b920f5abfb1f6d5f702e05421") ) // Fork June 2017
;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
