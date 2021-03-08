// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
/*
 * Compile this test against your .h files to make sure they compile. Note how
 * the eecs281 priority queues can be constructed with the different types. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly.  If you do not call a
 * function from here, it is NOT compiled due to templates!  So for instance,
 * if you don't add code to test updatePriorities(), you could have compiler
 * errors that you don't even know about.
 *
 * Notice that testPairing() tests the range-based constructor but main() and
 * testPriorityQueue() do not.  Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * These tests are NOT a complete test of your priority queues!
 */

#include "BinaryPQ.h"
#include "Eecs281PQ.h"
#include "UnorderedPQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


// Used to test a priority queue containing pointers to integers.
struct IntPtrComp {
    bool operator() (const int *a, const int  *b) const {
        return *a < *b;
    }
};


// TODO: Make sure that you're using this->compare() properly, and everywhere
// that you should.  Complete this function by adding a functor that compares
// two HiddenData structures, create a PQ of the specified type, add two or
// three HiddenData objects to the PQ, and call this function from main().
void testHiddenData(const string &pqType) {
    struct HiddenData {
        int data;
    };

    struct HiddenDataComp {
        bool operator()(const HiddenData &a, const HiddenData &b) {
            return a.data < b.data;
        }
    };

    cout << "Testing " << pqType << " with hidden data\n";
} // testHiddenData()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePrioritiesHelper(Eecs281PQ<int *, IntPtrComp> *pq) {
    vector<int> data{ 1, 5 };
    pq->push(&data[0]);
    pq->push(&data[1]);
    data[0] = 10;
    pq->updatePriorities();
    assert(*pq->top() == 10);
} // testUpdatePrioritiesHelper()


// TODO: Add more code to this function to test if updatePriorities()
// is working properly.
void testUpdatePriorities(const string &pqType) {
    Eecs281PQ<int *, IntPtrComp> *pq = nullptr;
    cout << "Testing updatePriorities() on " << pqType << endl;
    if (pqType == "Unordered") {
        pq = new UnorderedPQ<int *, IntPtrComp>;
    } // if
	if (pqType == "Sorted") {
		pq = new SortedPQ<int*, IntPtrComp>;
	} // if
	if (pqType == "Binary") {
		pq = new BinaryPQ<int*, IntPtrComp>;
	} // if
	if (pqType == "Pairing") {
		pq = new PairingPQ<int*, IntPtrComp>;
	} // if
    testUpdatePrioritiesHelper(pq);
    delete pq;
} // testUpdatePriorities()


// Very basic testing.
void testPriorityQueue(Eecs281PQ<int> *pq, const string &pqType) {
    cout << "Testing priority queue: " << pqType << endl;

    pq->push(3);
    pq->push(4);
	pq->push(7);
	pq->push(5);
    assert(pq->size() == 4);
    assert(pq->top() == 7);

    pq->pop();
    assert(pq->size() == 3);
    assert(pq->top() == 5);
    assert(!pq->empty());

    pq->pop();
    assert(pq->size() == 2);
    assert(!pq->empty());

    // TODO: Add more testing here!

    cout << "testPriorityQueue() succeeded!" << endl;
} // testPriorityQueue()


// Test the pairing heap's range-based constructor, copy constructor,
// and operator=().  Still not complete, should have more code, test
// addNode(), updateElt(), etc.
void testPairing(vector<int> & vec) {
    cout << "Testing Pairing Heap separately" << endl;
    PairingPQ<int> * pq1 = new PairingPQ<int>(vec.begin(), vec.end());
    
    // This line is different just to show two different ways to declare a
    // pairing heap: as an Eecs281PQ and as a PairingPQ. Yay for inheritance!
    PairingPQ<int> * pq3 = new PairingPQ<int>();
    //*pq3 = *((PairingPQ<int> *)pq2);

	pq1->push(38);
	pq1->push(12);
	pq1->push(49);
	pq1->push(27);
	pq1->updateElt(pq1->addNode(50), 60);

	Eecs281PQ<int>* pq2 = new PairingPQ<int>(*((PairingPQ<int>*)pq1));
	*pq3 = *((PairingPQ<int>*)pq1);

	while (!pq1->empty() && !pq2->empty() && !pq3->empty()) {
		assert(pq1->top() == pq2->top());
		assert(pq1->top() == pq3->top());
		pq1->pop();
		pq2->pop();
		pq3->pop();
	}



    cout << "Basic tests done, calling destructors" << endl;

    delete pq1;
    delete pq2;
    delete pq3;

    cout << "testPairing() succeeded" << endl;
} // testPairing()


int main() {
    // Basic pointer, allocate a new PQ later based on user choice.
    Eecs281PQ<int> *pq;
    vector<string> types{ "Unordered", "Sorted", "Binary", "Pairing" };
    int choice;

    cout << "PQ tester" << endl << endl;
    for (size_t i = 0; i < types.size(); ++i)
        cout << "  " << i << ") " << types[i] << endl;
    cout << endl;
    cout << "Select one: ";
    choice = 3;
	cout << choice << endl;

    if (choice == 0) {
        pq = new UnorderedPQ<int>;
    } // if
    else if (choice == 1) {
        pq = new SortedPQ<int>;
    } // else if
    else if (choice == 2) {
        pq = new BinaryPQ<int>;
    } // else if
    else if (choice == 3) {
        pq = new PairingPQ<int>;
    } // else if
    else {
        cout << "Unknown container!" << endl << endl;
        exit(1);
    } // else
   
    testPriorityQueue(pq, types[choice]);
    testUpdatePriorities(types[choice]);

    if (choice == 3) {
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(1);
        testPairing(vec);
    } // if

    // Clean up!
    delete pq;

    return 0;
} // main()
