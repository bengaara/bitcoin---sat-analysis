#undef __cplusplus
#define __cplusplus 201900L

#include "../headers/word.h"

using namespace std;
btNodePointer word::operator[](const uint& v) {
	return value[v];
}

bool word::isNumeric() const {
	for (uint var = 0; var < WORD_SIZE; ++var) {
		btNodePointer _bit = (value[var]);

		if (!(_bit->isConstat())) {
			return false;
		}
	}
	return true;

}

word word::Or(const word& rhs) const {
	return this->value.Or(rhs.value);
}
word word::Xor(const word& rhs) const {
	return this->value.Xor(rhs.value);
}
word word::And(const word& rhs) const {
	return this->value.And(rhs.value);
}
word word::Not() const {
	return this->value.Not();
}
word word::Add(const word& rhs) const {
	if (DEBUG_TRACE_INFO)
		cout << "Add" << endl;
	return this->value.Add(rhs.value);
}
word word::rightRotate(unsigned int x) const {
	return this->value.rightRotate(x);
}
word word::leftRotate(unsigned int x) const {
	return this->value.leftRotate(x);
}

word word::rightShift(unsigned int x) const {
	return this->value.rightShift(x);
}

//constructors
word::word() :
		value(0) {
}
word::word(const uint& v) :
		value(v) {
}
word::word(const uint& start, bit vars[]) :
		value(start, vars) {

}

word::word(string hex) {
	uint myval { };
	std::stringstream ss { };
	ss << hex;
	ss >> std::hex >> myval;
	value = word(myval).value;
}

word::word(const bitVector& v) :
		value(v) {
}
word::word(const word& v) :
		value(v.value) {
}
/*
 word& word::operator=(const word& v) {
 this->value = v.value;
 return *this;
 }
 word& word::operator=(const uint& v) {
 word result = v;
 value = result.value;
 return *this;
 }
 */

uint word::toInt() const {
	uint result = 0;
	uint temp = 1;
	for (uint x = 0; x < WORD_SIZE; x++) {
		btNode _bit = *(value[x]);
		if (_bit.isTrue()) {
			result += temp << x;
		}

	}
	return result;
}
//toStrings
string word::toString() const {
	if (this->isNumeric())
		return toHex();
	string result = "{";
	for (uint i = 0; i < WORD_SIZE; ++i) {
		btNode _bit = *(value[i]);
		result = result + "\t" + _bit.toString() + "\n";
	}
	return result + "}";
}
string word::toHex() const {
	int intval = (toInt());
	char hexval[9] { };
	snprintf(hexval, 9, "%08x", intval);
	string result = hexval;
//string result = string{resultTemp[6],resultTemp[7],resultTemp[4],resultTemp[5],resultTemp[2],resultTemp[3],resultTemp[0],resultTemp[1]} ;

	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}
string word::toBinary() const {
	char hexval[WORD_SIZE + 1] { };
	for (uint var = 0; var < WORD_SIZE; ++var) {
		btNode _bit = *(value[var]);
		if (!(_bit.isConstat())) {
			hexval[var] = 'X';
		}
		if (_bit.isTrue()) {
			hexval[var] = '1';
		} else {
			hexval[var] = '0';
		}
	}
	std::reverse(std::begin(hexval), std::end(hexval) - 1);
	hexval[WORD_SIZE] = '\0';
	return string(hexval);
}

void word::printdots(string filenames) {
//cout << "printing dots for " << filenames << endl;
	for (uint i = 0; i < WORD_SIZE; ++i) {
		btNode _bit = *(value[i]);
		_bit.printDot(filenames + std::to_string(i));
	}
}

btNodesVector::btNodesVector(std::vector<btNodePointer> _value) :
		value(_value) {
}
btNodesVector::btNodesVector() :
		value(VECTOR_SIZE, btNode::FALSE) {
}
btNodesVector::btNodesVector(const uint& v) :
		value(VECTOR_SIZE, btNode::FALSE) {
	std::bitset<VECTOR_SIZE> bits(v);
	for (uint x = 0; x < VECTOR_SIZE; x++) {
		if (bits[x]) {
			value[x] = btNode::TRUE;
		} else {
			value[x] = btNode::FALSE;
		}
	}
}
btNodesVector::btNodesVector(const btNodesVector& v) :
		value(v.value) {

}

btNodesVector::btNodesVector(const uint& start, bit vars[]) :
		value(VECTOR_SIZE, btNode::FALSE) {
	const uint& stop = start + VECTOR_SIZE;
	for (uint x = start; x < stop; x++) {
		value[x - start] = btNodePointer(new btNode(bit::variables[x]));
//			cout<<x-start<<") "<<value[x-start].toString()<<endl;
	}
}

btNodePointer btNodesVector::operator[](const uint& v) const {

	return value[v];
}
btNodesVector& btNodesVector::operator=(const btNodesVector& v) {

	this->value = v.value;
	return *this;
}
btNodesVector btNodesVector::Or(const btNodesVector& rhs) const {
	btNodesVector result = 0;
	for (uint n = 0; n < VECTOR_SIZE; n++) {
		result.value[n] = (this->value[n])->Or(rhs.value[n]);
	}
	return result;
}

btNodesVector btNodesVector::Xor(const btNodesVector& rhs) const {
	btNodesVector result = 0; //this->value ^ rhs.value;
	for (uint n = 0; n < VECTOR_SIZE; n++) {
		result.value[n] = this->value[n]->Xor(rhs.value[n]);
	}
	return result;
}
btNodesVector btNodesVector::And(const btNodesVector& rhs) const {
	btNodesVector result = 0;
	;
	for (uint n = 0; n < VECTOR_SIZE; n++) {
		result.value[n] = this->value[n]->And(rhs.value[n]);
	}
	return result;
}
btNodesVector btNodesVector::Not() const {
	btNodesVector result = 0; //!(this->value);
	for (uint n = 0; n < VECTOR_SIZE; n++) {
//			std::cout << n << std::endl;
		result.value[n] = this->value[n]->Not();
	}
	return result;
}
btNodesVector btNodesVector::Add(const btNodesVector& rhs) const {
	btNodesVector result = 0; // this->value + rhs.value;

	btNodePointer carry = this->value[0]->And(rhs.value[0]);
	result.value[0] = this->value[0]->Xor(rhs.value[0]);
	for (uint n = 1; n < VECTOR_SIZE; n++) {
		btNodePointer bitL = this->value[n];
		btNodePointer bitR = rhs.value[n];

		btNodePointer bitO = bitL->Xor(bitR)->Xor(carry);

		carry = bitL->And(bitR)->Or((bitL->And(carry)))->Or((bitR->And(carry)));

		result.value[n] = bitO;

	}
	return result;
}

btNodesVector btNodesVector::rightRotate(unsigned int x) const {
	x = x % VECTOR_SIZE;
	btNodesVector result = this->value;
	std::rotate(result.value.begin(), result.value.begin() + x,
			result.value.end());
	return result;
}
btNodesVector btNodesVector::leftRotate(unsigned int x) const {
	x = x % VECTOR_SIZE;
	return rightRotate(VECTOR_SIZE - x);
}

btNodesVector btNodesVector::rightShift(unsigned int x) const {
	x = x % VECTOR_SIZE;
	btNodesVector result = 0;
	for (uint i = 0; i < VECTOR_SIZE; ++i) {
		if (i < (VECTOR_SIZE - x)) {
			result.value[i] = this->value[x + i];
		} else {
			break;
		}
	}
	return result;
}

