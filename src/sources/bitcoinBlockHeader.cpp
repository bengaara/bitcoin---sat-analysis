#include <algorithm>
#include <vector>

#include "../headers/bitcoinBlockHeader.h"

void bitcoinBlockHeader::generateBlockHash() {
	block_header = _version;
	block_header.insert(block_header.end(), _previous_block_header_hash.begin(),
			_previous_block_header_hash.end());
	block_header.insert(block_header.end(), _merkle_root.begin(),
			_merkle_root.end());
	block_header.insert(block_header.end(), _time.begin(), _time.end());
	block_header.insert(block_header.end(), _nBits.begin(), _nBits.end());
	block_header.insert(block_header.end(), _nonce.begin(), _nonce.end());
	_block_header_hash = sha256::doubleHash(block_header);
}
wordVector bitcoinBlockHeader::reverse(const wordVector& toReverse) {
	wordVector result = wordVector(toReverse);
	std::reverse(result.begin(), result.end());
	return result;
}
wordVector fromInt(uint from) {
	word tmp = word(from);
	return sha256::fromHex(tmp.toHex());
}
bitcoinBlockHeader::bitcoinBlockHeader(const wordVector& previous_block_header_hash,
		const wordVector& merkle_root, uint time, uint nBits, uint nonce) :
		_version(fromInt(2)), _previous_block_header_hash(
				reverse(previous_block_header_hash)), _merkle_root(
				reverse(merkle_root)), _time(fromInt(time)), _nBits(
				fromInt(nBits)), _nonce(fromInt(nonce)) {

	generateBlockHash();
}
wordVector bitcoinBlockHeader::version() {
	return _version;
}
wordVector bitcoinBlockHeader::previous_block_header_hash() {
	return _previous_block_header_hash;
}
wordVector bitcoinBlockHeader::merkle_root() {
	return _merkle_root;
}
wordVector bitcoinBlockHeader::time() {
	return _time;
}
wordVector bitcoinBlockHeader::nBits() {
	return _nBits;
}
wordVector bitcoinBlockHeader::nonce() {
	return _nonce;
}

wordVector bitcoinBlockHeader::block_header_hash() {
	return _block_header_hash;
}
wordVector bitcoinBlockHeader::block_header_hash_reversed() {
	wordVector result = reverse(_block_header_hash);
	return sha256::fromHex(sha256::toHex(result));;
}

