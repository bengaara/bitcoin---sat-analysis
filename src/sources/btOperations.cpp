#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include "../headers/btOperations.h"

using namespace boost::filesystem;
using namespace std;
void btOperations::writeBtToFileSystem(btNodePointer bit, string foldername,
		int depth) {
	if (!(bit->isCollapsed())) {
		cerr << foldername<< " could not be created because btNode is not collapsed" << endl;
		return;
	}
	path dir(foldername);
	create_directory(dir);
	if (bit->isCollapsed()) {
		bit->printDot(foldername + "/value");
		cout << "d=" << depth << "\t" << foldername << "/" << bit->toString()
				<< endl;
		return;
	} else {

		depth++;
		boost::filesystem::ofstream(
				foldername + "/" + btNode::toStringOperand(bit->operand));

		writeBtToFileSystem(bit->left, foldername + "/l", depth);
		if (bit->operand != btNode::NOT) {
			writeBtToFileSystem(bit->right, foldername + "/r", depth);
		}

	}
}
void btOperations::colapseFileSystemBt(string foldername, int depth) {
	cerr << "btOperations::colapseFileSystemBt is not implemented" << endl;
}
void btOperations::colapseInMemoryBt(btNodePointer bit, int depth) {

	if (bit->isCollapsed()) {
		std::cout << "Collapsed: " << depth<< "\t"<<bit::toStringBDDStats()<< std::endl;
		return;
	} else {
		depth++;
		colapseInMemoryBt(bit->left, depth);
		if (bit->operand != btNode::NOT) {
			colapseInMemoryBt(bit->right, depth);
		}
	}

	bit->collapse();
}
