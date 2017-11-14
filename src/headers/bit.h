/*
 * bit.h
 *
 *  Created on: Sep 15, 2017
 *      Author: bikxs
 */
#include	<string>
#include "../buddy/bdd.h"
#include <iostream>

#ifndef SRC_HEADERS_BIT_H_
#define SRC_HEADERS_BIT_H_

class bit {
public:

	static const int WORDS { 20 };
	static const int WORD_SIZE { 32 };
	static void init();
	static bit variables[];
	static std::string toStringBDDStats();
private:
	static const int CACHE_SIZE { 5000000 };
	static const int NODE_COUNT { CACHE_SIZE * 100 };


	bdd value;
	bit(bdd);
public:
	bit();
	bit(bool _bool);
	bit(const bit &r);
	virtual ~bit();

	int id() const;
	bit& operator=(const bit &r);
	bit operator&(const bit &r) const;
	bit operator^(const bit &r) const;
	bit operator|(const bit &r) const;
	bit operator!() const;
	void printDot(std::string filename) const;
	void printFile(std::string filename) const;

	bool isConstat() const;
	bool isTrue() const;

};

#endif /* SRC_HEADERS_BIT_H_ */
