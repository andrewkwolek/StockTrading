// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include "market.h"

using namespace std;


int main(int argc, char* argv[]) {

	ios_base::sync_with_stdio(false);

	Market market;
	
	//Print program startup output
	cout << "Processing orders...\n";

	market.setArguments(argc, argv);

	market.ReadHeader();

	//Read the next order from input
	market.ReadOrders();

	return 0;

}