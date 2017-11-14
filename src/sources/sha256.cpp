#include <algorithm>
#include "../headers/sha256.h"
#include "../headers/word.h"

using namespace std;
wordVector sha256::init(uint* values, uint size) {
	wordVector result = wordVector(size, word(0));
	for (unsigned int i = 0; i < size; ++i) {
		result[i] = values[i];
	}
	return result;
}

/*
 * (first 32 bits of the fractional parts of the cube roots of the first
 * 64 primes 2..311):
 */
uint sha256::sha256_k[] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
		0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa,
		0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138,
		0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624,
		0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f,
		0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

/*
 * Working varialbles Initialize hash values:(first 32 bits of the
 * fractional parts of the square roots of the first 8 primes. 2..19):
 */
uint sha256::initialValues[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

wordVector sha256::calculateHash() {
	// Initialize hash values:
	wordVector hash = init(initialValues, 8);

	// declare working variables and initialize the working variables
	word a = hash[0];
	word b = hash[1];
	word c = hash[2];
	word d = hash[3];
	word e = hash[4];
	word f = hash[5];
	word g = hash[6];
	word h = hash[7];

	// Initialize array of round constants
	wordVector k = init(sha256_k, 64);

	// message is ready: Formated into 512bits chunks (courtesy of
	// setmessage();

	//exit(0);
	// for each chunk - index=m
	for (unsigned int m = 0; m < messagePadded.size(); m += 16) {
		// create a 64-entry message schedule array w[0..63] of 32-bit words
		wordVector messageSchedule = wordVector(64, word(0));
		{
			for (int i = 0; i < 64; i++) {
				if (word::DEBUG_TRACE_INFO)
					cout << "Message shuffling Round " << i << " " << endl;
//				{
//					cout << i<<"\t";
//					for (int var = 0; var < messageSchedule.size(); ++var) {
//						cout << messageSchedule[var].toHex();
//					}
//					cout << endl;
//				}
				if (i < 16) {
					// copy chunk into first 16 words w[0..15] of the
					// message schedule array
					messageSchedule[i] = messagePadded[m + i];
				} else {
					// 16..63] of the message schedule array:
					word T1 = (messageSchedule[i - 2]);
					word T2 = (messageSchedule[i - 15]);

					messageSchedule[i] = (Epsilon1(T1)).Add(
							messageSchedule[i - 7]).Add(Epsilon0(T2)).Add(
							messageSchedule[i - 16]);

				}
			}
		}
		//exit(0);
		// Initialize working variables to current hash value:
		{
			a = hash[0];
			b = hash[1];
			c = hash[2];
			d = hash[3];
			e = hash[4];
			f = hash[5];
			g = hash[6];
			h = hash[7];
		}
		for (int i = 0; i < 64; i++) { //should be 64
			if (word::DEBUG_TRACE_INFO)
				cout << "Compression Round " << i << endl;
			// Compression function main
			{
				word T1 = h.Add(Sigma1(e)).Add(Ch(e, f, g)).Add(k[i]).Add(
						messageSchedule[i]);

				word T2 = Sigma0(a).Add(Maj(a, b, c));

				h = g;
				g = f;
				f = e;
				e = d.Add(T1);
				d = c;
				c = b;
				b = a;
				a = T1.Add(T2);

			}
		}
		// Add the compressed chunk to the current hash value:
		{
			hash[0] = hash[0].Add(a);
			hash[1] = hash[1].Add(b);
			hash[2] = hash[2].Add(c);
			hash[3] = hash[3].Add(d);
			hash[4] = hash[4].Add(e);
			hash[5] = hash[5].Add(f);
			hash[6] = hash[6].Add(g);
			hash[7] = hash[7].Add(h);
		}
	}

	return hash;
}

sha256::sha256(wordVector messageword) {
	setMessage(messageword);
}
sha256::sha256(string message) :
		sha256(fromAscii(message)) {
}
sha256::sha256() :
		sha256("") {
}
void sha256::setMessage(const wordVector& message) {
	this->message = message;
	this->messagePadded = padMessage(message);
}

word Ch(const word& x, const word& y, const word& z) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Ch" << endl;
	return (x.And(y)).Xor((x.Not()).And(z));

}

word Maj(const word& x, const word& y, const word& z) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Maj" << endl;
	return (x.And(y)).Xor(x.And(z)).Xor(y.And(z));
}

word Sigma0(const word& x) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Sigma0" << endl;
	return (x.rightRotate(2)).Xor(x.rightRotate(13)).Xor(x.rightRotate(22));

}

word Sigma1(const word& x) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Sigma1" << endl;
	return (x.rightRotate(6)).Xor(x.rightRotate(11)).Xor(x.rightRotate(25));

}

word Epsilon0(const word& x) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Epsilon0" << endl;

	return (x.rightRotate(7)).Xor(x.rightRotate(18)).Xor(x.rightShift(3));
}
word Epsilon1(const word& x) {
	if (word::DEBUG_TRACE_INFO)
		cout << "Epsilon1" << endl;
	return (x.rightRotate(17)).Xor(x.rightRotate(19)).Xor(x.rightShift(10));
}

wordVector sha256::fromAscii(const string& message) {
	string message2 = string(
			(message.length() % 4 == 0 ? 0 : 4 - message.length() % 4), '\0')
			+ message;
	unsigned int len = message2.length() / 4;
	wordVector result = wordVector(len);

	for (unsigned int i = 0; i < len; ++i) {
		const char* s_num = message2.substr(i * 4, 4).c_str();
		char r_num[] = { s_num[3], s_num[2], s_num[1], s_num[0] };
		unsigned int t_num = *((unsigned int*) r_num);
		result[i] = word(t_num);
	}
	return result;
}

wordVector sha256::fromHex(const string& message) {

	string message2 = string(message.length() % 4, 0) + message;
	std::transform(message2.begin(), message2.end(), message2.begin(),
			::toupper);
	//cout<<message2<<endl;
	unsigned int len = message2.length() / 8;
	wordVector result = wordVector(len);
	for (unsigned int i = 0; i < len; ++i) {
		string s_num = message2.substr(i * 8, 8);

		string temp = string { s_num[6], s_num[7], s_num[4], s_num[5], s_num[2],
				s_num[3], s_num[0], s_num[1] };

		result[i] = word(temp);
	}
	return result;
}

string sha256::toHex(const wordVector& message) {
	std::stringstream ss = stringstream();
	string result;
	for (unsigned int i = 0; i < message.size(); ++i) {
		ss << (message[i].toHex());
	}
	ss >> result;
	return result;
}

wordVector padMessage(const wordVector& message) {
	const unsigned int blocksize = 16;
	int length = message.size();
	word head = 0x80000000;
	wordVector middle = wordVector(blocksize - (length % blocksize) - 2,
			word(0));
	word tail = length * word::WORD_SIZE;
	wordVector pad = message;
	pad.push_back(head);
	pad.insert(pad.end(), middle.begin(), middle.end());
	pad.push_back(tail);
	return pad;
}

wordVector sha256::fromHexReversed(const string& hex) {
	wordVector reversed = fromHex(hex); // reversed(hex);
	std::reverse(reversed.begin(), reversed.end());
	return reversed;
}
wordVector sha256::hash(wordVector messageword) {
	return (sha256(messageword)).calculateHash();
}
wordVector sha256::doubleHash(wordVector messageword) {
	return (hash(hash(messageword)));
}
