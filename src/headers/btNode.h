#ifndef BIT_H_
#define BIT_H_

#undef __cplusplus
#define __cplusplus 201900L

#include "bit.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <iostream>

class btNode;
typedef unsigned int uint;
//typedef std::unique_ptr<bit> bitPtr;

typedef std::shared_ptr<btNode> btNodePointer;

/*
 * BitNode class is part of a binary tree where the left and right members are arguments and
 * operand represents the operation. In the collapsed state left,right and operand are null with value holding a
 * valid BDD
 */
class btNode: public std::enable_shared_from_this<btNode> {
private:

public:

	enum Operands {
		NONE = 0, AND = 1, OR = 2, XOR = 3, NOT = 4
	};
	static std::string toStringOperand(Operands operand);
	static void printdots(std::string filenames, const bdd& _bdd);
	const static btNodePointer TRUE;
	const static btNodePointer FALSE;

	bit value;
	bool toBeColapsed=false;
	btNodePointer left { };
	btNodePointer right { };
	Operands operand = NONE;

	btNode();
	btNode(bool);
	btNode(const bit&);
	btNode(const btNode&);
	btNode(btNodePointer left, btNodePointer right, Operands operand);

	~btNode();

	btNodePointer Or(btNodePointer rhs);
	btNodePointer Xor(btNodePointer rhs);
	btNodePointer And(btNodePointer rhs);
	btNodePointer Not();

	int id() const;
	bool isCollapsed() const;
	void collapse();
	bool isConstat() const;
	bool isTrue() const;

	void printDot(std::string filenames) const;
	std::string toString() const;

};

#endif /* BIT_H_ */
