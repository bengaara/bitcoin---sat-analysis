#undef __cplusplus
#define __cplusplus 201900L

#include <iostream>
#include <string>

#include "headers/tests.h"
#include "cute/cute.h"
#include "cute/ide_listener.h"
#include "cute/xml_listener.h"
#include "cute/cute_runner.h"

using namespace std;


void runSuites(int argc, char const *argv[]) {
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);

	cute::suite s = tests::testSuiteWords();
	cute::makeRunner(lis, argc, argv)(s, "words suite");

	s = tests::testSuiteSHA256();
	cute::makeRunner(lis, argc, argv)(s, "SHA256 suite");

	s = tests::testSuiteBitCoins();
	cute::makeRunner(lis, argc, argv)(s, "Bitcoins suite");

	s = tests::testSuiteBinaryTreeCreation();
	cute::makeRunner(lis, argc, argv)(s, "Binary Tree making");

}


void print(string output) {
	cout << output << endl;
}
int main(int argc, char const *argv[]) {
	print("A program to hash SHA256 using BDD (buddy package)");
	bit::init();
	runSuites(argc, argv);

	print("End of program");
	return 0;
}

