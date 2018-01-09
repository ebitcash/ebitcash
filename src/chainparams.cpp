// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 01/Dec/2013 eBitcash";
    const CScript genesisOutputScript = CScript() << ParseHex("0431b2df855e540aa9290858f38e59a0eed60930d78b76a95fd9a0b8476fed43caecfee127391f9e570c8f2c39bf2cdc9b7d55936817f907498da46954198b8fe5") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 100000;
		consensus.BIP34Height = 1;
		consensus.BIP34Hash = uint256S("0x000002c0d94254515569c50edfb27f2e3b7e8771cbdc335851f92cfc636b9f0b");
		consensus.BIP65Height = 2;
		consensus.BIP66Height = 3;
		consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nPowTargetTimespan = 4 * 60 * 60;
		consensus.nPowTargetSpacing = 2 * 60;
		consensus.fPowAllowMinDifficultyBlocks = true;
		consensus.fPowNoRetargeting = false;
		consensus.nRuleChangeActivationThreshold = 114; // 95% of 120
		consensus.nMinerConfirmationWindow = 120; // nPowTargetTimespan / nPowTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1448928000; // 2015
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1451606400; // 2016

		// Deployment of BIP68, BIP112, and BIP113.
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1452902400; // 2016
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1484524800; // 2017

		// Deployment of SegWit (BIP141, BIP143, and BIP147)
		consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
		consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1458086400; // 2016
		consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1489622400; // 2017

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000bdc792735e");

		// By default assume that the signatures in ancestors of this block are valid.
		consensus.defaultAssumeValid = uint256S("0x0000069112cd0ee7b6e2b6b5b4c43f6e49560c4e9500aee0575aa0521934ce79"); //369000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
		pchMessageStart[0] = 0xdc;//0xec;
		pchMessageStart[1] = 0xc9;//0xa5;
		pchMessageStart[2] = 0xb9;//0xc9;
		pchMessageStart[3] = 0xef;//0xe2;

        nDefaultPort = 9555;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1448928000, 2191336, 0x1e0ffff0, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000006802e01ee7aa8b6bd8b6d6dfacaa03146acdaf3782df68f2f6cfa95c3f5"));
        assert(genesis.hashMerkleRoot == uint256S("0xff41162d03e3434bd8a92ceb9729c97e4d3035d869c17da2269a6fa5bf5b6655"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("dnsseed.ebitcash.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,2);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,9);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,168);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x24, 0x86, 0xE9, 0x3D};
        base58Prefixes[EXT_SECRET_KEY] = {0x24, 0x86, 0xFE, 0xD5};

        //vFixedSeeds.clear();
        //vSeeds.clear();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));



        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
			{
				{  1500, 	uint256S("0x0000013bb3c7a23d2f7977717aa7b5250efd00bba0f8f50f25b76c158db880ca") },
				{  4030, 	uint256S("0x000001ed0ed05d89818726b880f92ce91ce6b8f5ae35b262609c4c94e0c1083c") },
				{  8070, 	uint256S("0x000001ed0ed05d89818726b880f92ce91ce6b8f5ae35b262609c4c94e0c1083c") },
				{ 10900, 	uint256S("0x0000010f08330b6fd52f05b8fa9c4582f103cd185d6d433c4e5f38aae233ce91") },
				{ 20170, 	uint256S("0x0000012c1ea4cd2babbc69991e1ab72cb5288fa0cdaf4b7cf50ef9f42587ba06") },
				{ 33333, 	uint256S("0x000000548528846768feace532e00a9ef8dd922e75334c7a9dff9edc67204021") },
				{ 77444, 	uint256S("0x0000024ce4afa3c0e6f241539cd691786cd4abf28173d1348c29f7632ebbd58e") },
				{109000, 	uint256S("0x000005d59944b143f7a5870b8cc75ba55cffff883f76f66f6675ea96a6550625") },
				{209000, 	uint256S("0x000009edf658a879adf3a186c46debd9ebeb8350aa9e495f650d33f3b4bcd03f") },
				{321000, 	uint256S("0x000001ca90c93efe1da681ed813fb53b90ff591b49e21045864bfa62dab5cb9a") },
				{409000, 	uint256S("0x0000061b9f77c7570ef40e2ac3004353bf4f525d704292ca9c0b8d9226ca6fcd") },
				{509000, 	uint256S("0x00000d8fde7d3d7ce6eba946ba54adee4b0712885608b81449f26cea3bd00cfb") },
				{529000, 	uint256S("0x0000057a0d7727266286acbf3a9096a732422b387b458dcd2a800484e0a6651b") },
				{549000,	uint256S("0x000000011689755f6af78e96241ab0697ab9fc53630a4500798e3da7ee5f6c5f") }

			}
		};

		chainTxData = ChainTxData{
			1514959898, // * UNIX timestamp of last known number of transactions
			549297,	// * total number of transactions between genesis and that timestamp
            		//   (the tx=... number in the SetBestChain debug.log lines)
			0.1	// * estimated number of transactions per second after that timestamp
		};
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x00000adb022aac929528d3fae478ebed50983505725fe009732df72929929a69");
        consensus.BIP65Height = 2; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 3; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 60 * 60;
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 90; // 75% for testchains
        consensus.nMinerConfirmationWindow = 120; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1448928000; // 2015
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1451606400; // 2016

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1452902400; // 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1484524800; // 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1458086400; // 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1489622400; // 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000d000c1");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000006e21c13815985f442b8c59a59d70e35f4dacde7888af8e36c534b6cf808"); //12

        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xdc;
        pchMessageStart[2] = 0x15;
        pchMessageStart[3] = 0x45;
        nDefaultPort = 19555;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1515476670, 1261765, 0x1e0ffff0, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000004ca8b6a0d2ea5e8f3f77a54f00510cd75d06ae1bb5ec57aee1fc4d62585"));
        assert(genesis.hashMerkleRoot == uint256S("0xff41162d03e3434bd8a92ceb9729c97e4d3035d869c17da2269a6fa5bf5b6655"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.ebitcash.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            {
                {12, uint256S("000006e21c13815985f442b8c59a59d70e35f4dacde7888af8e36c534b6cf808")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 000006e21c13815985f442b8c59a59d70e35f4dacde7888af8e36c534b6cf808 (height 12)
            1515485791,
            12,
            0.01
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 2; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 3; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 60 * 60;
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 9; // 75% for testchains
        consensus.nMinerConfirmationWindow = 12; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1515476670, 2, 0x207fffff, 1, 10000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7226c3363e11d7cf391d2ef6f24f1b5f85d3db6b4c69a01235b1142be5b7d703"));
        assert(genesis.hashMerkleRoot == uint256S("0xff41162d03e3434bd8a92ceb9729c97e4d3035d869c17da2269a6fa5bf5b6655"));


        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("7226c3363e11d7cf391d2ef6f24f1b5f85d3db6b4c69a01235b1142be5b7d703")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
