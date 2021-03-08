// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <getopt.h>

using namespace std;

class Trader {
public:
	int bought;
	int sold;
	int net_sale;

	Trader()
		: bought(0), sold(0), net_sale(0) { }

	~Trader() { }

};