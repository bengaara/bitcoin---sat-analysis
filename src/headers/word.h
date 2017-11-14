#ifndef WORD_H_
#define WORD_H_


#include "btNode.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <bitset>



class btNodesVector {
private:
	static const unsigned int VECTOR_SIZE = 32;
	std::vector<btNodePointer> value;
	btNodesVector(std::vector<btNodePointer> _value);
public:
	btNodesVector();
	btNodesVector(const uint& v) ;
	btNodesVector(const btNodesVector& v);
	btNodesVector(const uint& start, bit vars[]) ;
	btNodePointer operator[](const uint& v) const;
	btNodesVector& operator=(const btNodesVector& v);
	btNodesVector Or(const btNodesVector& rhs) const ;
	btNodesVector Xor(const btNodesVector& rhs) const;
	btNodesVector And(const btNodesVector& rhs) const;
	btNodesVector Not() const;
	btNodesVector Add(const btNodesVector& rhs) const ;
	btNodesVector rightRotate(unsigned int x) const;
	btNodesVector leftRotate(unsigned int x) const;

	btNodesVector rightShift(unsigned int x) const ;
};
typedef btNodesVector bitVector;

/*
 * Class representing a 32 bit integer that can be passed into any interger function (including sha256)
 * The bits in this case are btNodes which can be manipulated a binary tree collapsable into a BDD
 */
class word final {
protected:

public:
	bitVector value;
	static const bool DEBUG_TRACE_INFO = false;
	static const unsigned int WORD_SIZE = 32;

	word();
	word(std::string hex);
	word(const uint& v);
	word(const uint& start, bit variables[]);
	word(const bitVector& v);
	word(const word& v);

	btNodePointer operator[](const uint& v);

	word Or(const word& rhs) const;
	word Xor(const word& rhs) const;
	word And(const word& rhs) const;
	word Add(const word& rhs) const;
	word Not() const;
	word rightRotate(unsigned int x) const;
	word leftRotate(unsigned int x) const;
	word rightShift(unsigned int x) const;

	bool isNumeric() const;
	uint toInt() const;

	//toStrings
	std::string toString() const;
	std::string toBinary() const;
	std::string toHex() const;

	void printdots(std::string filenames);

};

typedef word uint32;




#endif /* WORD_H_ */
