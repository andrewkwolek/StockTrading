// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#ifndef SORTEDPQ_H
#define SORTEDPQ_H

#include "Eecs281PQ.h"
#include <vector>      // Needed for data vector
#include <algorithm>   // You'll need it eventually for sort() and possibly other things


// A specialized version of the 'heap' ADT that is implemented with an
// underlying sorted array-based container.
// Note: The most extreme element should be found at the end of the
// 'data' container, such that traversing the iterators yields the elements in
// sorted order.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class SortedPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit SortedPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
    } // SortedPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n log n) where n is number of elements in range.
    // TODO: When you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    SortedPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function
		for (InputIterator i = start; i < end; ++i) {
			data.push_back(*i);
		}
		std::sort(data.begin(), data.end(), this->compare);
    } // SortedPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~SortedPQ() {
    } // ~SortedPQ()


    // Description: Add a new element to the heap.
    // Runtime: O(n)
    // TODO: When you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function
		data.insert(std::upper_bound(data.begin(), data.end(), val, this->compare), val);

		//upper bound in stl and then insert
	} // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(1)
    virtual void pop() {
        // TODO: Implement this function
		data.pop_back();
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the vector.  This should be a reference for speed.  It MUST
    //              be const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function
		//TYPE temp = data[size() - 1];
        // These lines are present only so that this provided file compiles.
        return data[size() - 1];     // TODO: Delete or change this line
    } // top()


    // Description: Get the number of elements in the heap.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n log n)
    virtual void updatePriorities() {
        // TODO: Implement this function
		std::sort(data.begin(), data.end(), this->compare);
    } // updatePriorities()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    // TODO: Add any additional member functions or data you require here.

}; // SortedPQ

#endif // SORTEDPQ_H
