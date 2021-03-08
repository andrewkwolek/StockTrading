// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <getopt.h>
//#include "order.h"

using namespace std;

struct Pack {
	int price;
	int quantity;
	int id;
	int position;
};

struct PackLess {
	bool operator()(Pack const& p1, Pack const& p2) {
		if (p1.price == p2.price) {
			return p1.position > p2.position;
		}
		return p1.price < p2.price;
	}
};

struct PackMore {
	bool operator()(Pack const& p1, Pack const& p2) {
		if (p1.price == p2.price) {
			return p1.position > p2.position;
		}
		return p1.price > p2.price;
	}
};

struct TimeT {
	char bs;
	int time;
	int price;
};

class Stock {
private:
	//int num;

public:
	Stock()
		: buy(), sell(), lowers(), highers(), travelers() { }

	~Stock() { }


	priority_queue<Pack, vector<Pack>, PackLess> buy;
	priority_queue<Pack, vector<Pack>, PackMore> sell;
	priority_queue<int, vector<int>, less<int>> lowers;
	priority_queue<int, vector<int>, greater<int>> highers;
	vector<TimeT> travelers;

	


	int getMedian() {
		if (lowers.size() == 0 && highers.size() == 0) {
			return 0;
		}
		else if (lowers.size() == highers.size()) {
			return (lowers.top() + highers.top()) / 2;
		}
		else if (lowers.size() > highers.size()) {
			return lowers.top();
		}
		else {
			return highers.top();
		}
		
	}

	void rebalance() {
		if (lowers.size() > highers.size()) {
			if (lowers.size() - highers.size() >= 2) {
				highers.push(lowers.top());
				lowers.pop();
			}

		}
		else if (highers.size() > lowers.size()) {
			if (highers.size() - lowers.size() >= 2) {
				lowers.push(highers.top());
				highers.pop();
			}
		}
	}
};
