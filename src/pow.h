// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ION_POW_H
#define ION_POW_H

#include <stdint.h>

#include <arith_uint256.h>
// #include <main.h>
#include <sync.h>
#include <util.h>
#include <stdint.h>

// class CBlockHeader;
class CBlockIndex;
class uint256;
class arith_uint256;

#define PERCENT_FACTOR 100

#define BLOCK_TYPE CBlock *
#define BLOCK_TIME(block) block->nTime
#define INDEX_TYPE CBlockIndex *
#define INDEX_HEIGHT(block) block->nHeight
#define INDEX_TIME(block) block->GetBlockTime()
#define INDEX_PREV(block) block->pprev
#define INDEX_TARGET(block) block->nBits
#define DIFF_ABS std::abs
#define SET_COMPACT(EXPANDED, COMPACT) EXPANDED.SetCompact(COMPACT)
#define GET_COMPACT(EXPANDED) EXPANDED.GetCompact()
#define BIGINT_MULTIPLY(x, y) x* y
#define BIGINT_DIVIDE(x, y) x / y
#define BIGINT_GREATER_THAN(x, y) (x > y)

unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake);

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, unsigned int nBits);

#endif // BITCOIN_POW_H
