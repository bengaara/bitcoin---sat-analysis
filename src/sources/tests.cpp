#include "../headers/tests.h"
#include "../cute/cute.h"
using namespace std;

cute::suite tests::testSuiteWords() {
	cute::suite s { };

	s.push_back(CUTE(tests::testInitialiseVarBit));
	s.push_back(CUTE(tests::testWordConstructionsWithVariables));
	s.push_back(CUTE(tests::testWordInitializationFromInt));
	s.push_back(CUTE(tests::testWordHexRepresentationFromString));
	s.push_back(CUTE(tests::testWordAssignmentOperator));

	s.push_back(CUTE(tests::testWordNOT));
	s.push_back(CUTE(tests::testWordAND));
	s.push_back(CUTE(tests::testWordOR));
	s.push_back(CUTE(tests::testWordXOR));
	s.push_back(CUTE(tests::testWordADD));
	s.push_back(CUTE(tests::testWordMixedOperations));

	s.push_back(CUTE(tests::testWordShifting));
	s.push_back(CUTE(tests::testWordRightRotate));
	s.push_back(CUTE(tests::testWordLeftRotate));
	return s;
}
cute::suite tests::testSuiteSHA256() {
	cute::suite s { };
	s.push_back(CUTE(tests::testCH));
	s.push_back(CUTE(tests::testMaj));
	s.push_back(CUTE(tests::testEpsilon0));
	s.push_back(CUTE(tests::testEpsilon1));
	s.push_back(CUTE(tests::testSigman0));
	s.push_back(CUTE(tests::testSigma1));
	s.push_back(CUTE(tests::testASCIItoHEX));
	s.push_back(CUTE(tests::testSHA256));
	s.push_back(CUTE(tests::testDoubleSHA256));

	return s;
}
cute::suite tests::testSuiteBitCoins() {
	cute::suite s { };
	s.push_back(CUTE(tests::testBitCoinHashes));
	return s;
}
cute::suite tests::testSuiteBinaryTreeCreation() {
	cute::suite s { };
	s.push_back(CUTE(tests::testMakeBinaryTreeforSmallExpression));
	s.push_back(CUTE(tests::testMakeBinaryTreeforEntireSHA256));
	return s;
}

uint32_t tests::tsfstr(std::string datetime = "1970.01.01 00:00:00") {
	if (datetime.length() < 19) {
		std::cout << "invalid string - cant convert to timestamp";
		return 0;
	}
	struct tm tm { };
	tm.tm_year = atoi(datetime.substr(0, 4).c_str()) - 1900;
	tm.tm_mon = atoi(datetime.substr(5, 2).c_str()) - 1;
	tm.tm_mday = atoi(datetime.substr(8, 2).c_str());
	tm.tm_hour = atoi(datetime.substr(11, 2).c_str());
	tm.tm_min = atoi(datetime.substr(14, 2).c_str());
	tm.tm_sec = atoi(datetime.substr(17, 2).c_str());

	return mktime(&tm) + 0x2A30;
}

void tests::testSha256(string input, string hash) {
	string hashed = sha256::toHex(sha256::hash(sha256::fromAscii(input)));
	ASSERT_EQUALM("Hashing '" + input + "'", hashed, hash);

}
void tests::testDoubleSha256(string input, string hash) {
	string hashed = sha256::toHex(sha256::doubleHash(sha256::fromAscii(input)));
	ASSERT_EQUALM("Double Hashing '" + input + "'", hashed, hash);

}

void tests::testInitialiseVarBit() {

	btNodePointer bitA {new btNode(bit::variables[0])};
	ASSERT_EQUALM("Create a bit with a varible bdd", bitA->toString(), "v1");
}

void tests::testWordConstructionsWithVariables() {

	word w = word(0, bit::variables);
	word x = word(32, bit::variables);
	btNodePointer bitA = w[0];
	ASSERT_EQUALM("First bit of word", bitA->toString(), "v1");
	bitA = w[1];
	ASSERT_EQUALM("Second bit of word", bitA->toString(), "v2");

	bitA = w[2];
	ASSERT_EQUALM("Third bit of word", bitA->toString(), "v3");

	bitA = x[2];
	ASSERT_EQUALM("Third bit of word x", bitA->toString(), "v35");
}
void tests::testWordInitializationFromInt() {
	word a = 42;

	ASSERT_EQUALM("word integer constructor a = " + a.toString(), a.toBinary(),
			"00000000000000000000000000101010");

	ASSERT_EQUALM("word integer constructor a = " + a.toBinary(), a.toString(),
			"0000002A");

	word b = 99999;
	ASSERT_EQUALM("word integer constructor b", b.toString(), "0001869F");

	word p = 0x3956c25b;
	ASSERT_EQUALM("word hex representation", p.toHex(), "3956C25B");
}
void tests::testWordHexRepresentationFromString() {
	word q = string("F956c25b");
	ASSERT_EQUALM("word hex representation from string", q.toHex(), "F956C25B");
}
void tests::testWordAssignmentOperator() {
	word p = 1;
	ASSERT_EQUALM("word hex representation for p=0b1," + p.toBinary(),
			p.toHex(), "00000001");
	word c { };
	c = 99998;
	ASSERT_EQUALM("word integer assignment c", c.toString(), "0001869E");

	int i { 0 };

	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = 0;
	z = x;
	ASSERT_EQUALM("word variable assignment x", "v1", z[0]->toString());
	z = y;
	ASSERT_EQUALM("word variable assignment y", "v33", z[0]->toString());

}
void tests::testWordADD() {
	word b = 99999;
	word c = 99998;
	word a = b.Add(c);
	ASSERT_EQUALM("word addition a= b+c", a.toString(), "00030D3D");

	int i { 0 };
	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);

	word z = x.Add(y);
	ASSERT_EQUALM("word addition z = x.Add(y) 1", "(v1 ^ v33)",
			z[0]->toString());

	ASSERT_EQUALM("word addition z = x.Add(y) 2", "((v2 ^ v34) ^ (v1 & v33))",
			z[1]->toString());

	//ASSERT_EQUALM("word addition z = x.Add(y) 3",z[3]->toString(), "((v4 ^ v36) ^ (((v3 & v35) | (v3 & (((v2 & v34) | (v2 & (v1 & v33))) | (v34 & (v1 & v33))))) | (v35 & (((v2 & v34) | (v2 & (v1 & v33))) | (v34 & (v1 & v33)))))) but was: ((v4 ^ v36) ^ (((v3 & v35) | (v3 & (((v2 & v34) | (v2 & (v1 & v33))) | (v34 & (v1 & v33))))) | (v35 & (((v2 & v34) | (v2 & (v1 & v33))) | (v34 & (v1 & v33)))");

}
void tests::testWordNOT() {

	word c = 99998;
	word a = c.Not();
	ASSERT_EQUALM(
			"word negation of c (NOT C) c=" + c.toHex() + " a = " + a.toHex(),
			a.toBinary(), "11111111111111100111100101100001");
	ASSERT_EQUALM(
			"word negation of c (NOT C) c=" + c.toBinary() + " a = "
					+ a.toBinary(), a.toString(), "FFFE7961");

	int i { 0 };

	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);

	ASSERT_EQUALM("word y", "v33", y[0]->toString());

	word z = x.Not();

	ASSERT_EQUALM("word s.Not()", z.value[0]->toString(), "!(v1)");
	return;

	word z2 = y.Not();
	ASSERT_EQUALM("word x.Not()", "!(v33)", z2[0]->toString());

}
void tests::testWordAND() {
	word c = 0x0F0F0F0F;
	word b = 0xF0F0F0F0;
	word a = b.And(c);

	ASSERT_EQUALM("word AND function b.Add(c)", "00000000", a.toString());

	int i { 0 };
	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = x.And(y);
	ASSERT_EQUALM("word z = x.And(y) 1", "(v1 & v33)", z[0]->toString());
	ASSERT_EQUALM("word z = x.And(y) 2", "(v2 & v34)", z[1]->toString());

	z = x.And(c);
	ASSERT_EQUALM("word z = x.And(y) 2", "(v1 & 1)", z[0]->toString());

}
void tests::testWordOR() {
	word c = 99998;
	word a = c.Or(c.Not());
	ASSERT_EQUALM("word OR function a = c.Or(c.Not());", a.toString(),
			"FFFFFFFF");

	int i { 0 };
	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = x.Or(y);
	ASSERT_EQUALM("word  z = x.Or(y)", "(v1 | v33)", z[0]->toString());
}
void tests::testWordXOR() {
	word c = 99998;
	word b = 99999;
	word a = b.Xor(c);
	ASSERT_EQUALM("word XOR function a = b.Xor(c)", a.toString(), "00000001");
	int i { 0 };
	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = x.Xor(y);
	ASSERT_EQUALM("word z = x.Xor(y)", "(v1 ^ v33)", z[0]->toString());
}
void tests::testWordMixedOperations() {
	int i { 0 };

	word s = word(i++ * word::WORD_SIZE, bit::variables);
	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = word(i++ * word::WORD_SIZE, bit::variables);

	word w = s.Not();
	ASSERT_EQUALM("s.Not()", w[0]->toString(), "!(v1)");
	word v = x.Not();

	ASSERT_EQUALM("w.And(v)", "(!(v1) & !(v33))", w.And(v)[0]->toString());

	ASSERT_EQUALM("y.Or(w.And(x))", "(v66 | (!(v2) & !(v34)))",
			y.Or(w.And(v))[1]->toString());

	word u;
	u = y.Or(w.And(v));
	ASSERT_EQUALM("y.Or(w.And(x)) Try#1", "(v65 | (!(v1) & !(v33)))",
			u[0]->toString());

	word u2 = u;
	ASSERT_EQUALM("y.Or(w.And(x)) Try#2", "(v65 | (!(v1) & !(v33)))",
			u2[0]->toString());

	word u3 = y.Or(w.And(v));
	ASSERT_EQUALM("y.Or(w.And(x)) Try#3", "(v65 | (!(v1) & !(v33)))",
			u3[0]->toString());
	ASSERT_EQUALM("y.Or(w.And(x)) Try#4", "(v65 | (!(v1) & !(v33)))",
			y.Or(w.And(v))[0]->toString());

	word temp = w.And(v).Xor(u);
	ASSERT_EQUALM("w.And(x).Xor(u) Try#1",
			"((!(v1) & !(v33)) ^ (v65 | (!(v1) & !(v33))))",
			temp[0]->toString());

	ASSERT_EQUALM("w.And(x).Xor(u) Try#2",
			"((!(v1) & !(v33)) ^ (v65 | (!(v1) & !(v33))))",
			w.And(v).Xor(u)[0]->toString());
}

void tests::testWordShifting() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;

	//shifting
	word d = 4;
	ASSERT_EQUALM("word integer assignment d", d.toString(), "00000004");

	word g = d.rightShift(1);
	ASSERT_EQUALM(
			"right shifting g = d.rightShift(1), d=" + d.toBinary() + " g="
					+ g.toBinary(), g.toString(), "00000002");

	d = 7;
	g = d.rightShift(1);
	ASSERT_EQUALM(
			"right shifting g = d.rightShift(1), d=" + d.toBinary() + " g="
					+ g.toBinary(), g.toString(), "00000003");

	ASSERT_EQUALM(
			"RightShift(c,4) c=" + c.toBinary() + " exp="
					+ c.rightShift(4).toBinary(), c.rightShift(4).toHex(),
			"00000207");
	ASSERT_EQUALM(
			"RightShift(c,3) c=" + c.toBinary() + " exp="
					+ c.rightShift(3).toBinary(), c.rightShift(3).toHex(),
			"0000040F");
	ASSERT_EQUALM(
			"RightShift(c,13) c=" + c.toBinary() + " exp="
					+ c.rightShift(13).toBinary(), c.rightShift(13).toHex(),
			"00000001");
	ASSERT_EQUALM(
			"RightShift(c,14) c=" + c.toBinary() + " exp="
					+ c.rightShift(14).toBinary(), c.rightShift(14).toHex(),
			"00000000");

}
void tests::testWordRightRotate() {
	word d = 16;

	word n = d.rightRotate(5);
	ASSERT_EQUALM(
			"word integer rotation d=0x00000010 right 5 times d=" + d.toBinary()
					+ " n=" + n.toBinary(), n.toString(), "80000000");
}
void tests::testWordLeftRotate() {
	word d = 16;
	word o = d.leftRotate(5);
	ASSERT_EQUALM("word integer rotation d=0x10 left 5 times", o.toString(),
			"00000200");

	word m = d.leftRotate(6);
	ASSERT_EQUALM("word integer rotation d=0x10 left 6 times", m.toString(),
			"00000400");

	m = d.leftRotate(28);
	ASSERT_EQUALM("word integer rotation d=0x10 left 28 times", m.toString(),
			"00000001");
}
void tests::testCH() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;

	ASSERT_EQUALM("Ch(a,b,c)", Ch(a, b, c).toHex(), "000020EB");
}
void tests::testMaj() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;
	ASSERT_EQUALM("Maj(a,b,c)", Maj(a, b, c).toHex(), "000020D2");
}
void tests::testEpsilon0() {

	word a = 16532;
	word b = 56539842;
	word c = 8315;
	ASSERT_EQUALM("Epsilon0(c)", Epsilon0(c).toHex(), "FE1EC44F");
	ASSERT_EQUALM("Epsilon0(a)", Epsilon0(a).toHex(), "38250893");
	ASSERT_EQUALM("Epsilon0(b)", Epsilon0(b).toHex(), "2ADDEAFA");

	int i { 0 };

	word s = word(i * word::WORD_SIZE, bit::variables);

	i++;
	word x = word(i * word::WORD_SIZE, bit::variables);
	i++;
	word y = word(i * word::WORD_SIZE, bit::variables);
	i++;
	word z = word(i * word::WORD_SIZE, bit::variables);
	i++;

	word w = s.Not();
	word v = x.Not();

	word u = y.Xor(w.And(v));

	word temp = w.And(x);

	word z1 = Epsilon0(temp);
	ASSERT_EQUALM("Epsilon0(u)",
			"(((!(v8) & v40) ^ (!(v19) & v51)) ^ (!(v4) & v36))",
			z1[0]->toString());

}
void tests::testEpsilon1() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;
	ASSERT_EQUALM("Epsilon1(c)", Epsilon1(c).toHex(), "1432E008");
	ASSERT_EQUALM("Epsilon1(a)", Epsilon1(a).toHex(), "28588010");
	ASSERT_EQUALM("Epsilon1(b)", Epsilon1(b).toHex(), "8A39966A");
}
void tests::testSigman0() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;
	ASSERT_EQUALM("Sigma0(c)", Sigma0(c).toHex(), "C359E41F");
	ASSERT_EQUALM("Sigma0(b)", Sigma0(b).toHex(), "2C2CBC48");
	ASSERT_EQUALM("Sigma0(a)", Sigma0(a).toHex(), "05A24027");
}
void tests::testSigma1() {
	word a = 16532;
	word b = 56539842;
	word c = 8315;
	ASSERT_EQUALM("Sigma1(c)", Sigma1(c).toHex(), "E3703D05");
	ASSERT_EQUALM("Sigma1(a)", Sigma1(a).toHex(), "42A04B0A");

	ASSERT_EQUALM("Sigma1(b)", Sigma1(b).toHex(), "FF10703D");
}
void tests::testASCIItoHEX() {
	ASSERT_EQUALM("ASCII '' in hex", sha256::toHex(sha256::fromAscii("")), "");

	ASSERT_EQUALM("ASCII 'a' in hex", sha256::toHex(sha256::fromAscii("a")),
			"00000061");

	ASSERT_EQUALM("ASCII 'abcd' in hex",
			sha256::toHex(sha256::fromAscii("abcd")), "61626364");

	ASSERT_EQUALM("ASCII 'grape' in hex",
			sha256::toHex(sha256::fromAscii("grape")), "0000006772617065");
}
void tests::testSHA256() {
	//length of ascii inout shoud be a multiple of 4 characters
	testSha256("",
			"E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855");

	testSha256("abcd",
			"88D4266FD4E6338D13B845FCF289579D209C897823B9217DA3E161936F031589");

	testDoubleSha256("abcd",
			"7E9C158ECD919FA439A7A214C9FC58B85C3177FB1613BDAE41EE695060E11BC6");

	testSha256("Bundi needs liqour now!!",
			"66CE1E0740E6386D7EFAAB622E868490735A06E6C8E310425B3D361DAB9F3F04");

	testSha256("redgrape",
			"81B9AF468A22327989527D36172C4E88002FA175D08F67549185A86748485BCB");
	testSha256("Biko is Awesome!",
			"B690AE56CD121D5377CA3D5EC033EEE8B9F6FA675432924B71CB865C20335A04");

	testSha256(
			"Across an inclined taxpayer sings Biko. Biko alternates a brother. The bugger accounts Kenya throughout the beast. When can the crack complete the untrue space?",
			"D3DE70422A786A9DFEC72545A2914D53740924AF060BDF1A22462C0605BCF61F");
	testSha256(
			"Kenya disorders the eastern dip behind the enormous mixture. Kenya pretends! Biko accustoms Kenya across the toast. Kenya alleges the bathroom after a paranoid surprise.Museveni leaves an anger before a unavailable league. Uganda revolts under Museveni. Will Museveni color opposite Uganda? The hostile concept prevails before the ink. Does Uganda boil next to Museveni?Tanzania fences the away python. Kikwete contracts Tanzania below the mailbox. A secretary reflects next to Kikwete. Kikwete appears into Tanzania. Underneath Tanzania responds Kikwete.The freezing robot drains Uswazi. Zuma summarizes the preferred pit. Zuma smokes! Should the empties farce cable Zuma? Zuma toes whatever military hope with any state generator.",
			"160919F0E5E8E8A19B28EDA0800290C142E5988933A3927F32F6F865E1F6A546");

	testDoubleSha256("Bundi needs liqour",
			"B5E8B7223B8B012094889B761D95461B13A4CF3ECDDF4AEC076718A57C0D5442");

}
void tests::testDoubleSHA256() {
	testDoubleSha256(
			"Across an inclined taxpayer sings Biko. Biko alternates a brother. The bugger accounts Kenya throughout the beast. When can the crack complete the untrue space?",
			"BEF42CCB2F9A7FCAC6A29B1ECCC25A41AD4AD981EC530DFAB44A32B747BE5159");

	testDoubleSha256(
			"Kenya disorders the eastern dip behind the enormous mixture. Kenya pretends! Biko accustoms Kenya across the toast. Kenya alleges the bathroom after a paranoid surprise.Museveni leaves an anger before a unavailable league. Uganda revolts under Museveni. Will Museveni color opposite Uganda? The hostile concept prevails before the ink. Does Uganda boil next to Museveni?Tanzania fences the away python. Kikwete contracts Tanzania below the mailbox. A secretary reflects next to Kikwete. Kikwete appears into Tanzania. Underneath Tanzania responds Kikwete.The freezing robot drains Uswazi. Zuma summarizes the preferred pit. Zuma smokes! Should the empties farce cable Zuma? Zuma toes whatever military hope with any state generator.",
			"E4F0CF4C586DD2F11B282AFBA26AF43505DF5C85730A03F99985567A77283618");

}

void tests::testBitCoinHash(string block_number,
		const wordVector& previous_block_header_hash,
		const wordVector& merkle_root, uint32_t timestamp, uint32_t nBits,
		uint32_t nonce, string blockhash) {

	bitcoinBlockHeader blockT = bitcoinBlockHeader(previous_block_header_hash,
			merkle_root, timestamp, nBits, nonce);

	string actual = sha256::toHex(blockT.block_header_hash_reversed());

	ASSERT_EQUALM("Block#" + block_number, actual, blockhash);

}
void tests::testBitCoinHashes() {
	testBitCoinHash("286819",
			sha256::fromHex(
					"000000000000000117c80378b8da0e33559b5997f2ad55e2f7d18ec1975b9717"),
			sha256::fromHex(
					"871714dcbae6c8193a2bb9b2a69fe1c0440399f38d94b3a0f1b447275a29978a"),
			tsfstr("2014-02-20 04:57:25"), 419520339, 856192328,

			"0000000000000000E067A478024ADDFECDC93628978AA52D91FABD4292982A50");
	return;
	testBitCoinHash("470346",
			sha256::fromHex(
					"000000000000000000b2bb7085855ed85908359b791932adfba1475ab667a5a1"),
			sha256::fromHex(
					"fc4e4bc5bf386c2e6abd77dbe33b2003dff53b330db3bf46a7801b937b1de29a"),
			tsfstr("2017-06-08 10:14:29"), 402759343, 2922051395,
			("000000000000000000c8fef32b56fb773b3aa86c326e9545de6d3339639e5c84"));

	testBitCoinHash("470375",
			sha256::fromHex(
					"0000000000000000008942f4c2e8ce206e6e593e43058bc20fffcb4863f3027f"),
			sha256::fromHex(
					"7027aa56df8c32731df48942fa22814f88a620bb14370fb14108941392a1aeee"),
			tsfstr("2017-06-08 14:30:52"), 402759343, 565260539,
			("000000000000000000b68b9f0a4c75eb9d8d570ab60c1ec5ae0be4df652acd63"));

}

void tests::testMakeBinaryTreeforSmallExpression() {

	int i { 0 };

	word x = word(i++ * word::WORD_SIZE, bit::variables);
	word y = word(i++ * word::WORD_SIZE, bit::variables);
	word z = word(i++ * word::WORD_SIZE, bit::variables);

	word a = Ch(x, y, z);
	word b = Maj(x, y, z);

	ASSERT_EQUALM("a = Ch(x, y, z)", "((v1 & v33) ^ (!(v1) & v65))",
			a[0]->toString());
	ASSERT_EQUALM("b = Maj(x, y, z)",
			"(((v1 & v33) ^ (v1 & v65)) ^ (v33 & v65))", b[0]->toString());

//	return;
	for (uint i = 0; i < word::WORD_SIZE; i++) {
		cout << endl << "Making a" + std::to_string(i) << endl;
		btOperations::colapseInMemoryBt(a.value[i], 0);
		btOperations::writeBtToFileSystem(a.value[i],
				"bits/testing/a" + std::to_string(i), 0);
		cout << endl << "Making b" + std::to_string(i) << endl;
		btOperations::colapseInMemoryBt(b.value[i], 0);
		btOperations::writeBtToFileSystem(b.value[i],
				"bits/testing/b" + std::to_string(i), 0);
	}
}

void tests::testMakeBinaryTreeforEntireSHA256() {

	int i { 0 };
	wordVector input;
	for (int var = 0; var < bit::WORDS; ++var) {
		input.push_back(word(i++ * word::WORD_SIZE, bit::variables));
	}

	wordVector doubleHashed = sha256::doubleHash(input);
	ASSERT_EQUALM("sha256 return size in bits", 256,
			doubleHashed.size() * word::WORD_SIZE);

	for (uint i = 0; i < word::WORD_SIZE; i++) {
		cout << endl << "Making bits/doubleSHA256/dH" + std::to_string(i) << endl;
		btOperations::colapseInMemoryBt(doubleHashed[i].value[i], 0);
		btOperations::writeBtToFileSystem(doubleHashed[i].value[i], "bits/doubleSHA256/dH" + std::to_string(i), 0);
	}

}
