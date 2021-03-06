// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>       // You'll need it eventually for several member functions
#include <utility>     // You'll need swap() eventually


// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            explicit Node(const TYPE &val)
				: elt{ val }, child{ nullptr }, sibling{ nullptr }, previous{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
			Node *previous;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
		root = nullptr;
		sz = 0;
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
		std::deque<Node*> nodes;
		InputIterator i = start;
		for (; i < end; i++) {
			Node *n = new Node(*i);
			nodes.push_back(n);
		}
		sz = nodes.size();

		while (nodes.size() > 1) {
			Node* one = nodes.front();
			nodes.pop_front();
			Node* two = nodes.front();
			nodes.pop_front();

			nodes.push_back(meld(one, two));
		}
		if (sz == 0) {
			root = nullptr;
			return;
		}


		root = nodes[0];
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare } {
		std::deque<Node*> nodes;
		root = nullptr;
		sz = 0;

		nodes.push_back(other.root);

		while (!nodes.empty()) {
			if (nodes.front()->child) {
				nodes.push_back(nodes.front()->child);
			}
			if (nodes.front()->sibling) {
				nodes.push_back(nodes.front()->sibling);
			}
			push(nodes.front()->getElt());
			nodes.pop_front();
		}
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: Implement this function.
		PairingPQ copy(rhs);
		std::swap(root, copy.root);
		std::swap(sz, copy.sz);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
			std::deque<Node*> nodes;
			if (root != nullptr) {
				nodes.push_back(root);
			}
			

			while (!nodes.empty()) {
				if (nodes.front()->child) {
					nodes.push_back(nodes.front()->child);
				}
				if (nodes.front()->sibling) {
					nodes.push_back(nodes.front()->sibling);
				}
				delete nodes.front();
				nodes.pop_front();
			}
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
		std::deque<Node*> nodes;
		std::deque<Node*> melds;
		nodes.push_back(root);

		while (!nodes.empty()) {
			if (nodes.front()->child) {
				nodes.push_back(nodes.front()->child);
				nodes.front()->child = nullptr;
			}
			if (nodes.front()->sibling) {
				nodes.push_back(nodes.front()->sibling);
				nodes.front()->sibling = nullptr;
			}
			
			nodes.front()->previous = nullptr;

			melds.push_back(nodes.front());

			nodes.pop_front();
		}

		while (melds.size() > 1) {
			Node* one = melds.front();
			melds.pop_front();
			Node* two = melds.front();
			melds.pop_front();

			melds.push_back(meld(one, two));
		}

		root = melds[0];
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
		addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
		std::deque<Node*> nodes;
		std::deque<Node*> melds;
		if (!root->child) {
			root = nullptr;
			sz = 0;
			return;
		}

		nodes.push_back(root->child);
		delete root;

		while (!nodes.empty()) {
			if (nodes.front()->sibling) {
				nodes.push_back(nodes.front()->sibling);
				nodes.front()->sibling = nullptr;
			}
			nodes.front()->previous = nullptr;

			melds.push_back(nodes.front());

			nodes.pop_front();

		}

		while (melds.size() > 1) {
			Node* one = melds.front();
			melds.pop_front();
			Node* two = melds.front();
			melds.pop_front();

			melds.push_back(meld(one, two));
		}

		root = melds.front();
		--sz;
		
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // These lines are present only so that this provided file compiles.
		return root->getElt();      // TODO: Delete or change this line
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return sz; // TODO: Delete or change this line
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
		if (sz == 0) {
			return true;
		}
		return false;
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        // TODO: Implement this function
		if (node == root) {
			root->elt = new_value;
			return;
		}

		if (node == node->previous->sibling) {
			node->previous->sibling = node->sibling;
			if (node->previous) {
				node->sibling->previous = node->previous;
			}
			node->sibling = nullptr;
			node->previous = nullptr;
		}
		else if (node == node->previous->child) {
			node->previous->child = node->sibling;
			if (node->previous) {
				node->sibling->previous = node->previous;
			}
			node->sibling = nullptr;
			node->previous = nullptr;
		}
		

		node->elt = new_value;

		root = meld(root, node);

    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        // TODO: Implement this function
		Node * n = new Node(val);
		if (root == nullptr) {
			root = n;
			sz++;
			return n;
		}
		root = meld(root, n);
		sz++;
        return n; // TODO: Delete or change this line
    } // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
	Node* root;
	size_t sz;

    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
	Node * meld(Node* root_one, Node* root_two) {
		if (this->compare(root_one->getElt(), root_two->getElt())) {
			root_one->sibling = root_two->child;
			root_two->child = root_one;
			root_one->previous = root_two;
			return root_two;
		}
		else {
			root_two->sibling = root_one->child;
			root_one->child = root_two;
			root_two->previous = root_one;
			return root_one;
		}
	}
};


#endif // PAIRINGPQ_H
