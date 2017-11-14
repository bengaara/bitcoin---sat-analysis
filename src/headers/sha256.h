#ifndef SHA256_H
#define SHA256_H

#include "word.h"


typedef std::vector<word> wordVector;
/*
 * Class implementing the SHA256 hashing algorithm
 */
class sha256 final {
private:
	wordVector messagePadded;
	wordVector message;

public:
	static wordVector init(uint* values, uint size);
	static uint sha256_k[];
	static uint initialValues[];
	static std::string toHex(const wordVector& source);
	static wordVector fromHex(const std::string& hex);
	static wordVector fromHexReversed(const std::string& hex);
	static wordVector fromAscii(const std::string& message);
	static wordVector hash(wordVector messageword);
	static wordVector doubleHash(wordVector messageword);
	sha256(wordVector messageword);
	sha256(std::string message);
	sha256();
	void setMessage(const wordVector& message);
	wordVector calculateHash();
};

word Ch(const word& x, const word& y, const word& z);

word Maj(const word& x, const word& y, const word& z);

word Sigma0(const word& x);

word Sigma1(const word& x);
word Epsilon0(const word& x);
word Epsilon1(const word& x);
wordVector fromAscii(const std::string& message);
wordVector fromHex(const std::string& message);
std::string toHex(const word& w);
std::string toHex(const wordVector& message);
wordVector padMessage(const wordVector& message);

#endif // SHA256_H
