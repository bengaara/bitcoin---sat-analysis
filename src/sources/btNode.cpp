#include "../headers/btNode.h"
using namespace std;
const btNodePointer btNode::TRUE(new btNode { true });
const btNodePointer btNode::FALSE(new btNode { false });

btNode::btNode() :
		toBeColapsed { true }, value(false) {

}
btNode::btNode(bool v) :
		toBeColapsed { true }, value(false) {

}
btNode::btNode(const bit&v) :
		toBeColapsed { true }, value(v) {

}

btNode::btNode(const btNode& v) :
		value(v.value), toBeColapsed(v.toBeColapsed), left(v.left), right(
				v.right), operand(v.operand) {
}

btNode::btNode(btNodePointer _left, btNodePointer _right, Operands _operand) :
		toBeColapsed { false }, left(_left), right(_right), operand(_operand) {

}
btNode::~btNode() {
//	if (isCollapsed())
//		bdd_delref(value.id());

}
std::string btNode::toStringOperand(Operands operand) {
	switch (operand) {
	case AND:
		return "AND";
	case OR:
		return "OR";
	case XOR:
		return "XOR";
	case NOT:
		return "NOT";
	case NONE:
	default:
		return "NONE";
	}
}
btNodePointer btNode::Or(btNodePointer rhs) {
	if (this->toBeColapsed && rhs->toBeColapsed)
		return btNodePointer(new btNode { this->value | rhs->value });

	return btNodePointer(new btNode { shared_from_this(), rhs, Operands::OR });
}
btNodePointer btNode::Xor(btNodePointer rhs) {
	if (this->toBeColapsed && rhs->toBeColapsed) {
		return btNodePointer(new btNode { this->value ^ rhs->value });
	}

	return btNodePointer(new btNode { shared_from_this(), rhs, Operands::XOR });
}

btNodePointer btNode::And(btNodePointer rhs) {
	if (this->toBeColapsed && rhs->toBeColapsed) {
		return btNodePointer(new btNode { this->value & rhs->value });
	}
	return btNodePointer(new btNode { shared_from_this(), rhs, Operands::AND });
}
btNodePointer btNode::Not() {
	if (this->toBeColapsed) {
		return btNodePointer(new btNode { !(this->value) });
	}
	return btNodePointer(new btNode { shared_from_this(), shared_from_this(),
			Operands::NOT });
}

int btNode::id() const {
	return value.id();
}
bool btNode::isCollapsed() const {
	return operand == Operands::NONE;
}
void btNode::collapse() {
	if (isCollapsed())
		return;
	toBeColapsed = true;
	if (!(left->isCollapsed())) {
		std::cerr << "btNode.collapse() while left is not collapsed"
				<< std::endl;
		return;
	}
	if (operand != Operands::NOT && !(right->isCollapsed())) {
		std::cerr << "btNode.collapse() while right is not collapsed"
				<< std::endl;
		return;
	}
	switch (operand) {
	case Operands::AND:
		value = left->value & right->value;
		break;
	case Operands::OR:
		value = left->value | right->value;
		break;
	case Operands::XOR:
		value = left->value ^ right->value;
		break;
	case Operands::NOT:
		value = !(left->value);
		break;
	case Operands::NONE:
		std::cerr << "invalid operand on btNode.collapse()" << std::endl;
		return;
	default:
		std::cerr << "illegal operand on btNode.collapse()" << std::endl;
		return;

	}
//	bdd_reorder_init();
	left = NULL;
	right = NULL;
	operand = NONE;

}
bool btNode::isConstat() const {
	return isCollapsed() && value.isConstat();
}
bool btNode::isTrue() const {
	return isCollapsed() && value.isTrue();
}

void btNode::printDot(std::string filenames) const {
	//TODO printdots(filenames, value);
}

std::string btNode::toString() const {
	if (isConstat()) {
		return isTrue() ? "1" : "0";
	}
	if (isCollapsed()) {
		std::string result = "v" + std::to_string(value.id() / 2);
//			cout << result << endl;
		return result;
	}
	/*
	 if (left == NULL && right == NULL) {
	 std::cout<<"Unexpected"<<std::endl;
	 return "(*BOTH NULL*)";
	 } else if (left == NULL) {
	 return "(*LEFT NULL*)";

	 } else if (right == NULL) {
	 return "(*RIGHT NULL*)";

	 }
	 */

	switch (operand) {

	case Operands::AND:
		return "(" + left->toString() + " & " + right->toString() + ")";
		break;
	case Operands::OR:
		return "(" + left->toString() + " | " + right->toString() + ")";
		break;
	case Operands::XOR:
		return "(" + left->toString() + " ^ " + right->toString() + ")";
		break;
	case Operands::NOT:
		return "!(" + left->toString() + ")";
		break;
	case Operands::NONE:
		return "(invalid operand)";
		break;
	default:
//			return "(" + left->toString() + " ? " + right->toString() + ")";
		return "(illegal operand)";
		break;

	}

}

