/*
 * bit.cpp
 *
 *  Created on: Sep 15, 2017
 *      Author: bikxs
 */

#include "../headers/bit.h"
#include <memory>

using namespace std;
bit bit::variables[WORD_SIZE * WORDS];
void bit::init() {
	bdd_init(NODE_COUNT, CACHE_SIZE);
	bdd_setvarnum(WORD_SIZE * WORDS);
	for (uint var = 0; var < WORD_SIZE * WORDS; var++) {
		variables[var] = bit(bdd_ithvarpp(var));
	}
	//	bdd_reorder(BDD_REORDER_SIFT);
	bdd_autoreorder(BDD_REORDER_SIFT);
	//	bdd_reorder_verbose(1);
	std::cout << "Initialized BDDs" << std::endl;
	std::cout << toStringBDDStats() << std::endl;

}

bit::bit() :
		value(bdd_false()) {

}

bit::bit(bdd _value) :
		value(_value) {

}
bit::bit(bool v) :
		bit(v ? bdd_true() : bdd_false()) {
}
bit::bit(const bit &r) :
		value(r.value) {
}
bit::~bit() {
	//Do nothing for now
}

int bit::id() const {
	return value.id();
}

bit& bit::operator=(const bit &r) {
	value = r.value;
	return *this;
}

bit bit::operator&(const bit &r) const {
	return bit(value & r.value);
}
bit bit::operator^(const bit &r) const {
	return bit(value ^ r.value);
}
bit bit::operator|(const bit &r) const {
	return bit(value | r.value);
}
bit bit::operator!() const {
	return bit(!value);
}
std::string exec(const char* cmd) {
	std::array<char, 128> buffer { };
	std::string result { };
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if (!pipe)
		throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != NULL)
			result += buffer.data();
	}
	cout << "exec:" << string(cmd) << endl;
	return result;
}
void bit::printDot(std::string filename) const {

	std::string str2 = (filename + ".png");
	char *fnPng = &str2[0u];

	std::string str3 = (filename + ".dot");
	char *fnDot = &str3[0u];

	bdd_fnprintdot(fnDot, value);
	exec(("dot -Tpng " + string(fnDot) + " -o " + string(fnPng)).c_str());

}
void bit::printFile(std::string filename) const {
	std::string str3 = (filename + ".bdd");
	char *fnBdd = &str3[0u];
	bdd_fnsave(fnBdd, value);
}

bool bit::isConstat() const {
	return (value == bdd_true() || value == bdd_false());
}
bool bit::isTrue() const {
	return isConstat() && value == bdd_true();
}

std::string bit::toStringBDDStats() {
	/*
	 long int produced; //Total number of new nodes ever produced.
	 int nodenum; //Currently allocated number of bdd nodes.
	 int maxnodenum; //User defined maximum number of bdd nodes.
	 int freenodes; //Number of currently free nodes.
	 int minfreenodes; //Minimum number of nodes that should be left after a garbage collection.
	 int varnum; //Number of defined bdd variables.
	 int cachesize; //Number of entries in the internal caches.
	 int gbcnum;	//Number of garbage collections done until now.
	 */

	bddStat stats { };
	bdd_stats(stats);
	return "varNum = " + to_string(stats.varnum) + ", nodes ="
			+ to_string(stats.nodenum) + ", produced ="
			+ to_string(stats.produced) + ", cacheSize ="
			+ to_string(stats.cachesize) + ", maxNodes ="
			+ to_string(stats.maxnodenum) + ", freeNodes="
			+ to_string(stats.freenodes) + " gbcCount="
			+ to_string(stats.gbcnum);

}
