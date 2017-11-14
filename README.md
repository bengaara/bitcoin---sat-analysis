# README #

### What is this repository for? ###

* Quick summary. 
   Bitcoing mining using boolean SAT analysis
* Version 1.0

### What is Boolean Satisfiabiliy (SAT) analysis

https://en.wikipedia.org/wiki/Boolean_satisfiability_problem

### What is a Binary Decision Diagram (BDD)

* https://en.wikipedia.org/wiki/Binary_decision_diagram
* http://pyeda.readthedocs.io/en/latest/bdd.html


### How do I get set up? ###

#### Needed IDE config
Ubuntu 16.04,Eclipse,Cute C++ unit testing,Boost for C++

#### Setting up ubuntu enviroment
##### update your ubuntu software

sudo apt-get update
sudo apt-get upgrade

##### setup git
sudo apt install git

##### setup graphiv - for making the dots (images)
http://www.graphviz.org/
sudo apt install graphiv

##### setup dev enviroment
* 1 sudo apt-get install build-essential
* 2 sudo apt install autoconf
* 3 sudo apt install libtool
* 4 sudo apt-get install manpages-dev

##### setup boost for c++
http://www.boost.org/
sudo apt-get install libboost-all-dev

##### compiling boost yourself
* 1 wget -O boost_1_55_0.tar.gz http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz/download
* 2 tar xzvf boost_1_55_0.tar.gz
* 3 cd boost_1_55_0/
* 4 sudo apt-get update
* 5 sudo apt-get install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev
* 6 ./bootstrap.sh --prefix=/usr/
* 7 ./b2
* 8 sudo ./b2 install 


#### download this code
* 1 cd ~
* 2 git clone https://username@bitbucket.org/Bikxs/bitcoin.git bitcoin++
* 3 cd bitcoin++/Debug
* 4 make clean
* 5 make

### Method
#### BDD Preparations
1. Make a binary tree having operands left and rights bits for 256 doubleSha256
2. Collapse this binarytrees to 256 flat variables (kill all subfolders)
3. Create difficulty precalculted bits(Key BDDs) 

#### Mining
1. Query bitcoind using getwork protocol. This gives a 640 bit result (called block) that should be searched for a valid nonce
2. Restrict the bdd. Pass the received blockHeader constant bits and varible nonce bits  to prehashed difficulty bit (Key BDDs).
3. Check if the prehashed bit is satisfiable (32 varibles now)
4. If satisfiable extract the first satisfying nonce and send back to bitcoind and wait for confimation message. $$$ :-)Issue new getWork() (step 1)
5. If not satisfiable :-(. Issue new getWork() (step 1)

https://en.bitcoin.it/wiki/Block_hashing_algorithm


### Who do I talk to? ###
* Person of interest1 : bb