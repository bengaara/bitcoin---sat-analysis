#include <iostream>
#include <string>
#include "word.h"
#include "sha256.h"
#include "../cute/cute_suite.h"
#include "bitcoinBlockHeader.h"
#include "btOperations.h"



#ifndef SRC_HEADERS_TESTS_H_
#define SRC_HEADERS_TESTS_H_

extern const int WORDS;

/*
 * Cute unit tests
 */
class tests final {
public:
	static cute::suite testSuiteWords();
	static cute::suite testSuiteSHA256();
	static cute::suite testSuiteBinaryTreeCreation();
	static cute::suite testSuiteBitCoins();

private:

	static void testInitialiseVarBit();

	static void testWordConstructionsWithVariables();
	static void testWordInitializationFromInt();
	static void testWordHexRepresentationFromString();
	static void testWordAssignmentOperator();

	static void testWordADD();
	static void testWordNOT();
	static void testWordAND();
	static void testWordOR();
	static void testWordXOR();
	static void testWordMixedOperations();
	static void testWordShifting();
	static void testWordRightRotate();
	static void testWordLeftRotate();
	static void testCH();
	static void testMaj();
	static void testEpsilon0();
	static void testEpsilon1();
	static void testSigman0();
	static void testSigma1();
	static void testASCIItoHEX();
	static void testSHA256();
	static void testDoubleSHA256();

	static void testBitCoinHashes();

	static void testMakeBinaryTreeforSmallExpression();
	static void testMakeBinaryTreeforEntireSHA256();

	static void testSha256(std::string input, std::string hash);
	static void testDoubleSha256(std::string input, std::string hash);
	static void testBitCoinHash(std::string block_number,
				const wordVector& previous_block_header_hash,
				const wordVector& merkle_root, uint32_t timestamp, uint32_t nBits,
				uint32_t nonce, std::string blockhash);
		static float difficulty(unsigned int bits);

	static uint32_t tsfstr(std::string datetime);

};

#endif /* SRC_HEADERS_TESTS_H_ */
