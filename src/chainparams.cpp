// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin developers
// Copyright (c) 2017 Empinel/The Ion Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"
#include "amount.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

void MineGenesis(CBlock genesis, uint256 nProofOfWorkLimit){
    // This will figure out a valid hash and Nonce if you're creating a differe$
    uint256 hashTarget = nProofOfWorkLimit;
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
        ++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
    newhash = genesis.GetHash();
    if(newhash < besthash){
        besthash=newhash;
        printf("New best: %s\n", newhash.GetHex().c_str());
    }
    }
    printf("Gensis Hash: %s\n", genesis.GetHash().ToString().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
    printf("Gensis nTime: %u\n", genesis.nTime);
    printf("Gensis nBits: %08x\n", genesis.nBits);
    printf("Gensis Nonce: %u\n\n\n", genesis.nNonce);
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xbf;
        pchMessageStart[1] = 0xf4;
        pchMessageStart[2] = 0x1a;
        pchMessageStart[3] = 0xb6;
        vAlertPubKey = ParseHex("040627d06214ba58f42eb74d475d32bc359c822902fb91766be30bfff2b878d2b5d4efa9e38c2a3438b15ff85e734ce3ce0382f8ebb79b6cdb3bc779af69e0b9b8");
        nDefaultPort = 58273;
        nRPCPort = 59273;
        nProofOfWorkLimit = ~uint256(0) >> 24; // 000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
        nProofOfStakeLimit = ~uint256(0) >> 24;

        const char* pszTimestamp = "Reuters: Sony Entertainment CEO exiting for a top role at Snap";
	std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
	vout[0].scriptPubKey = CScript() << ParseHex("04964ae39ac7421145f93a031791749772f671fa1153e4d6df87b1dce87ed2d68a74b46df6cd023ceffbbae4feed084915372d2b8ca866d24dd979af6f09800b3d") << OP_CHECKSIG;
        vout[0].nValue = (1 * COIN);
	CTransaction txNew(1, 1484310000, vin, vout, 0);
	
	genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1484310000;
        genesis.nBits    = 0x1e00ffff;
        genesis.nNonce   = 359900;

	hashGenesisBlock = genesis.GetHash();
	if (false) { MineGenesis(genesis, nProofOfWorkLimit); }
        
        assert(hashGenesisBlock == uint256("0x0000009b33ffdb43dc629edff9f4490d9b71cfe5effada3de8b7f2179e514175"));
        assert(genesis.hashMerkleRoot == uint256("0x2de1c19c955399e93a071058647e66f315b193f9d64c73f8acac6830eae65295"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,103);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,153);
        base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,40);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        
	vSeeds.push_back(CDNSSeedData("seeder.ionomy.com", "seeder2.ionomy.com"));
        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

	nPoolMaxTransactions = 3;
        //strSporkKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
        //strMasternodePaymentsPubKey = "046f78dcf911fbd61910136f7f0f8d90578f68d0b3ac973b5040fb7afb501b5939f39b108b0569dca71488f5bbf498d92e4d1194f6f941307ffd95f75e76869f0e";
        strDarksendPoolDummyAddress = "iUUCtBZUVR98Cufh9BbSSqUPJFEtPKSLSe";
        nLastPOWBlock 	= 100000; // Preliminary Proof of Work
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x2f;
        pchMessageStart[1] = 0xca;
        pchMessageStart[2] = 0x4d;
        pchMessageStart[3] = 0x3e;
        nProofOfWorkLimit = ~uint256(0) >> 16;
        nProofOfStakeLimit = ~uint256(0) >> 16;
        vAlertPubKey = ParseHex("04cc24ab003c828cdd9cf4db2ebbde8e1cecb3bbfa8b3127fcb9dd9b84d44112080827ed7c49a648af9fe788ff42e316aee665879c553f099e55299d6b54edd7e0");
        nDefaultPort = 27170;
        nRPCPort = 27171;
        strDataDir = "testnet";

        genesis.nVersion = 1;
        genesis.nTime    = 1484332000;
        genesis.nBits    = 0x1f00ffff;
        genesis.nNonce   = 1172;

		if (false) { MineGenesis(genesis, nProofOfWorkLimit); }

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        // assert(hashGenesisBlock == uint256("0x00008ca69320cdd1932a5d19f6602b383d6683e4185b552acb05a32dd94995f2"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,97);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[STEALTH_ADDRESS] = std::vector<unsigned char>(1,40);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        nLastPOWBlock = 0x7fffffff;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    
    bool fTestNet = GetBoolArg("-testnet", false);
    
    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
