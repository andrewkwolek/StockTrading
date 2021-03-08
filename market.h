// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <getopt.h>
#include "Stock.h"
#include "Trader.h"
#include "P2random.h"
//#include "order.h"

using namespace std;

class Market {
private:
	vector<Stock> stocks;
	vector<Trader> traders;
	string comment;
	string input_mode;
	int num_traders;
	int num_stocks;
	int orders_processed = 0;
	bool verbose;
	bool median;
	bool trader_info;
	bool time_travelers;
	

public:
	struct Order {
		int timestamp;
		char buysell;
		int trader_id;
		int stock_num;
		int price;
		int quantity;
	};

	Market()
		: stocks(), traders(), comment(), input_mode(), num_traders(),
	num_stocks(), verbose(false), median(false), trader_info(false),
	time_travelers(false) { }

	~Market() { }

	void setArguments(int argc, char* argv[]) {

		opterr = true;
		int choice;
		int option_index = 0;
		option long_options[] =
		{
			{"verbose", no_argument, nullptr, 'v'},
			{"median", no_argument, nullptr, 'm'},
			{"trader_info", no_argument, nullptr, 'i'},
			{"time_travelers", no_argument, nullptr, 't'},
			{nullptr, 0, nullptr, '\0'}
		};

		while ((choice = getopt_long(argc, argv, "vmit",
			long_options, &option_index)) != -1) {
			switch (choice) {
			case 'v':
				verbose = true;
				break;
			case 'm':
				median = true;
				break;
			case 'i':
				trader_info = true;
				break;
			case 't':
				time_travelers = true;
				break;
			}
		}
	}

	void ReadHeader() {
		string comment;
		getline(cin, comment);
		string junk;
		cin >> junk;
		cin >> input_mode;
		
		cin >> junk;
		cin >> num_traders;
		
		if (trader_info) {
			for (int i = 0; i < num_traders; ++i) {
				traders.push_back(Trader());
			}
		}
		cin >> junk;
		cin >> num_stocks;
		
		for (int i = 0; i < num_stocks; ++i) {
			stocks.push_back(Stock());
		}
		cin.clear();
		return;
	}

	void ReadOrders() {
		Order ord;
		int current_time = 0;
		int current_pos = 0;
		char t;
		char s;
		char dol;
		char pound;
		string junk;
		if (input_mode == "TL") {
			//Read in all values
			while (cin >> ord.timestamp >> ord.buysell >> junk >> t >> ord.trader_id >> s 
				>> ord.stock_num >> dol >> ord.price >> pound >> ord.quantity) {
				if (ord.timestamp < 0) {
					cerr << "Timestamp must be greater than non-negative" << endl;
					exit(1);
				}
				if (ord.trader_id < 0 || ord.trader_id >= num_traders) {
					cerr << "Trader ID is not valid" << endl;
					exit(1);
				}
				if (ord.stock_num < 0 || ord.stock_num >= num_stocks) {
					cerr << "Stock Number is not valid" << endl;
					exit(1);
				}
				if (ord.price < 1) {
					cerr << "Price is invalid!" << endl;
					exit(1);
				}
				if (ord.quantity < 1) {
					cerr << "Quantity is invalid!" << endl;
					exit(1);
				}
				Pack pack;
				pack.price = ord.price;
				pack.quantity = ord.quantity;
				pack.id = ord.trader_id;
				pack.position = current_pos;
				++current_pos;
				if (time_travelers) {
					TimeT t;
					t.bs = ord.buysell;
					t.price = ord.price;
					t.time = ord.timestamp;
					stocks[ord.stock_num].travelers.push_back(t);
				}


				if (ord.buysell == 'B') {
					stocks[ord.stock_num].buy.push(pack);
				}
				else if (ord.buysell == 'S') {
					stocks[ord.stock_num].sell.push(pack);
				}
				

				if (current_time != ord.timestamp) {
					if (ord.timestamp < current_time) {
						cerr << "Timestamps not in order!" << endl;
						exit(1);
					}
					if (median) {
						for (int i = 0; i < num_stocks; ++i) {
							if (stocks[i].getMedian() != 0) {
								cout << "Median match price of Stock " << i << " at time "
									<< current_time << " is $" << stocks[i].getMedian() << "\n";
							}
						}
					}
					current_time = ord.timestamp;
				}
				
				ExecuteTrades();
				
			}
			
			
		}
		else if (input_mode == "PR") {
			string junk;
			int seed;
			int num_orders;
			int arr_rate;

			cin >> junk;
			cin >> seed;
			cin >> junk;
			cin >> num_orders;
			cin >> junk;
			cin >> arr_rate;


			stringstream ss;

			P2random::PR_init(ss, seed, num_traders, num_stocks, num_orders, arr_rate);

			istream& inputStream = (input_mode == "PR") ? ss : cin;

			while (inputStream >> ord.timestamp >> ord.buysell >> junk >> t >> ord.trader_id >> s
				>> ord.stock_num >> dol >> ord.price >> pound >> ord.quantity) {
				Pack pack;
				pack.price = ord.price;
				pack.quantity = ord.quantity;
				pack.id = ord.trader_id;
				pack.position = current_pos;
				++current_pos;
				if (time_travelers) {
					TimeT t;
					t.bs = ord.buysell;
					t.price = ord.price;
					t.time = ord.timestamp;
					stocks[ord.stock_num].travelers.push_back(t);
				}


				if (ord.buysell == 'B') {
					stocks[ord.stock_num].buy.push(pack);
				}
				else if (ord.buysell == 'S') {
					stocks[ord.stock_num].sell.push(pack);
				}


				if (current_time != ord.timestamp) {
					if (median) {
						for (int i = 0; i < num_stocks; ++i) {
							if (stocks[i].getMedian() != 0) {
								cout << "Median match price of Stock " << i << " at time "
									<< current_time << " is $" << stocks[i].getMedian() << "\n";
							}
						}
					}
					current_time = ord.timestamp;
				}

				ExecuteTrades();
			}
		}

		if (median) {
			for (int i = 0; i < num_stocks; ++i) {
				if (stocks[i].getMedian() != 0) {
					cout << "Median match price of Stock " << i << " at time "
						<< current_time << " is $" << stocks[i].getMedian() << "\n";
				}
			}
		}


		cout << "---End of Day---\n";
		cout << "Orders Processed: " << orders_processed << "\n";

		if (trader_info) {
			cout << "---Trader Info---\n";
			for (int i = 0; i < num_traders; ++i) {
				cout << "Trader " << i << " bought " << traders[i].bought
					<< " and sold " << traders[i].sold << " for a net transfer of $"
					<< traders[i].net_sale << "\n";
			}
		}

		if (time_travelers) {
			cout << "---Time Travelers---\n";
			TimeTravelingAlgorithm();
		}
	}
	void ExecuteTrades() {
		for (size_t i = 0; i < stocks.size(); ++i) {
			while (!stocks[i].buy.empty() && !stocks[i].sell.empty()) {
				if (stocks[i].buy.top().price < stocks[i].sell.top().price) {
					break;
				}
				else {
					if (stocks[i].buy.top().quantity > stocks[i].sell.top().quantity) {
						if (stocks[i].buy.top().position < stocks[i].sell.top().position) {
							//Only exectute if trader_info is on
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].sell.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].sell.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].sell.top().quantity * stocks[i].buy.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].sell.top().quantity * stocks[i].buy.top().price;
							}
							//Only exectutes if verbose is on
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].sell.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].buy.top().price << "/share\n";
							}
							if (median) {
								if (stocks[i].lowers.size() == 0 || stocks[i].buy.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].buy.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].buy.top().price);
								}
								stocks[i].rebalance();
							}
						}
						else {
							//Only exectute if trader_info is on
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].sell.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].sell.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].sell.top().quantity * stocks[i].sell.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].sell.top().quantity * stocks[i].sell.top().price;
							}
							//Only exectutes if verbose is on
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].sell.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].sell.top().price << "/share\n";
							}
							if (median) {
								if (stocks[i].lowers.size() == 0 || stocks[i].sell.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].sell.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].sell.top().price);
								}
								stocks[i].rebalance();
							}
						}
						int diff = stocks[i].buy.top().quantity - stocks[i].sell.top().quantity;
						Pack new_buy = stocks[i].buy.top();
						new_buy.quantity = diff;

						stocks[i].buy.pop();
						stocks[i].sell.pop();
						stocks[i].buy.push(new_buy);
						++orders_processed;
					}
					else if (stocks[i].buy.top().quantity < stocks[i].sell.top().quantity) {
						if (stocks[i].buy.top().position < stocks[i].sell.top().position) {
							//Only exectutes if trader_info is on
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].buy.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].buy.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].buy.top().quantity * stocks[i].buy.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].buy.top().quantity * stocks[i].buy.top().price;
							}
							//Only exectutes if verbose is on
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].buy.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].buy.top().price << "/share\n";
							}
							if (median) {
								//stocks[i].median.push_back(stocks[i].buy.top().price);
								if (stocks[i].lowers.size() == 0 || stocks[i].buy.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].buy.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].buy.top().price);
								}
								stocks[i].rebalance();
							}
						}
						else {
							//Only exectutes if trader_info is on
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].buy.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].buy.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].buy.top().quantity * stocks[i].sell.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].buy.top().quantity * stocks[i].sell.top().price;
							}
							//Only exectutes if verbose is on
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].buy.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].sell.top().price << "/share\n";
							}
							if (median) {
								//stocks[i].median.push_back(stocks[i].sell.top().price);
								if (stocks[i].lowers.size() == 0 || stocks[i].sell.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].sell.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].sell.top().price);
								}
								stocks[i].rebalance();
							}
						}
						int diff = stocks[i].sell.top().quantity - stocks[i].buy.top().quantity;
						Pack new_sell = stocks[i].sell.top();
						new_sell.quantity = diff;

						stocks[i].buy.pop();
						stocks[i].sell.pop();
						stocks[i].sell.push(new_sell);
						++orders_processed;
					}
					else {
						if (stocks[i].buy.top().position < stocks[i].sell.top().position) {
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].buy.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].buy.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].buy.top().quantity * stocks[i].buy.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].buy.top().quantity * stocks[i].buy.top().price;
							}
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].buy.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].buy.top().price << "/share\n";
							}
							if (median) {
								//stocks[i].median.push_back(stocks[i].buy.top().price);
								if (stocks[i].lowers.size() == 0 || stocks[i].buy.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].buy.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].buy.top().price);
								}
								stocks[i].rebalance();
							}
						}
						else {
							if (trader_info) {
								traders[stocks[i].sell.top().id].sold += stocks[i].buy.top().quantity;
								traders[stocks[i].buy.top().id].bought += stocks[i].buy.top().quantity;
								traders[stocks[i].sell.top().id].net_sale += stocks[i].buy.top().quantity * stocks[i].sell.top().price;
								traders[stocks[i].buy.top().id].net_sale += (-1) * stocks[i].buy.top().quantity * stocks[i].sell.top().price;
							}
							if (verbose) {
								cout << "Trader " << stocks[i].buy.top().id << " purchased " << stocks[i].buy.top().quantity <<
									" shares of Stock " << i << " from Trader " << stocks[i].sell.top().id << " for $" <<
									stocks[i].sell.top().price << "/share\n";
							}
							if (median) {
								//stocks[i].median.push_back(stocks[i].sell.top().price);
								if (stocks[i].lowers.size() == 0 || stocks[i].sell.top().price < stocks[i].lowers.top()) {
									stocks[i].lowers.push(stocks[i].sell.top().price);
								}
								else {
									stocks[i].highers.push(stocks[i].sell.top().price);
								}
								stocks[i].rebalance();
							}
						}
						stocks[i].buy.pop();
						stocks[i].sell.pop();
						++orders_processed;
					}
				}
			}
		}
		return;
	}

	void TimeTravelingAlgorithm() {
		for (int i = 0; i < num_stocks; ++i) {
			int curr_buy = -1;
			int curr_sell = -1;
			int pot_buy = -1;
			//int profit = 0;
			int pot_buy_time = -1;
			int buy_time = -1;
			int sell_time = -1;
			for (size_t j = 0; j < stocks[i].travelers.size(); ++j) {
				//if a sell order comes in
				if (stocks[i].travelers[j].bs == 'S') {
					//if it is less than the 
					if ((curr_sell == -1 && stocks[i].travelers[j].price < curr_buy) 
						|| curr_buy == -1){
						curr_buy = stocks[i].travelers[j].price;
						buy_time = stocks[i].travelers[j].time;
					}
					else if ((stocks[i].travelers[j].price < curr_buy && pot_buy == -1) ||
						stocks[i].travelers[j].price < pot_buy) {
						pot_buy = stocks[i].travelers[j].price;
						pot_buy_time = stocks[i].travelers[j].time;
					}
					
				}
				else if (curr_buy != -1) {
					//Does nothing if the price to sell at is less than the current price to
					//sell at or does not yield a better profit
					if (stocks[i].travelers[j].price < curr_buy ||
						curr_sell - curr_buy > stocks[i].travelers[j].price - pot_buy) {
						continue;
					}
					else if (stocks[i].travelers[j].price > curr_buy && stocks[i].travelers[j].price > curr_sell) {
						curr_sell = stocks[i].travelers[j].price;
						sell_time = stocks[i].travelers[j].time;
						if (buy_time == -1) {
							buy_time = pot_buy_time;
						}

					}
					if (pot_buy != -1 && 
						curr_sell - curr_buy < stocks[i].travelers[j].price - pot_buy) {
						curr_sell = stocks[i].travelers[j].price;
						sell_time = stocks[i].travelers[j].time;
						curr_buy = pot_buy;
						buy_time = pot_buy_time;
					}
				}
			}
			if (sell_time == -1) {
				buy_time = -1;
			}
			cout << "A time traveler would buy shares of Stock " << i
				<< " at time: " << buy_time << " and sell these shares at time: "
				<< sell_time << "\n";
		}
	}

};