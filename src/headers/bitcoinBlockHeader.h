#include "sha256.h"
#include <cmath>
#include <time.h>

#ifndef HEADERS_BITCOIN_BLOCK_H_
#define HEADERS_BITCOIN_BLOCK_H_

/*
 * Class representing bitcoin block header.
 * Mining is achieved by finding a nonce such that the block header has is lower that the specified difficulty
 */
class bitcoinBlockHeader {

private:
	/*
	 * The block version number indicates which set of block validation rules to follow. See the list of block versions below.
	 */
	wordVector _version;
	/*
	 * A SHA256(SHA256()) hash in internal byte order of the previous block�s header. This ensures no previous block can be changed without also changing this block�s header.
	 */
	wordVector _previous_block_header_hash;
	/*
	 * A SHA256(SHA256()) hash in internal byte order. The merkle root is derived from the hashes of all transactions included in this block, ensuring that none of those transactions can be modified without modifying the header
	 */
	wordVector _merkle_root;
	/*
	 *The block time is a Unix epoch time when the miner started hashing the header (according to the miner). Must be strictly greater than the median time of the previous 11 blocks. Full nodes will not accept blocks with headers more than two hours in the future according to their clock.
	 */
	wordVector _time;
	/*
	 *An encoded version of the target threshold this block�s header hash must be less than or equal to.
	 */
	wordVector _nBits;
	/*
	 * An arbitrary number miners change to modify the header hash in order to produce a hash less than or equal to the target threshold. If all 32-bit values are tested, the time can be updated or the coinbase transaction can be changed and the merkle root updated.
	 */
	wordVector _nonce;
	/*
	 * _block_header_hash
	 */
	wordVector _block_header_hash;
	void generateBlockHash();
	static wordVector reverse(const wordVector& toReverse);


	inline float fast_log(float val) {
		int * const exp_ptr = reinterpret_cast<int *>(&val);
		int x = *exp_ptr;
		const int log_2 = ((x >> 23) & 255) - 128;
		x &= ~(255 << 23);
		x += 127 << 23;
		*exp_ptr = x;

		val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;
		return ((val + log_2) * 0.69314718f);
	}

	float difficulty(unsigned int bits) {
		static double max_body = fast_log(0x00ffff), scaland = fast_log(256);
		return exp(
				max_body - fast_log(bits & 0x00ffffff)
						+ scaland * (0x1d - ((bits & 0xff000000) >> 24)));
	}

public:
	wordVector block_header;
	bitcoinBlockHeader(const wordVector& previous_block_header_hash,
			const wordVector& merkle_root, uint time, uint nBits, uint nonce);
	wordVector version();
	wordVector previous_block_header_hash();
	wordVector merkle_root();
	wordVector time();
	wordVector nBits();
	wordVector nonce();

	wordVector block_header_hash();
	wordVector block_header_hash_reversed();
};
#endif /* HEADERS_BITCOIN_BLOCK_H_ */
