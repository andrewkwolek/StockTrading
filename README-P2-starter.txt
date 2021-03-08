Make sure that all of your source code files, for parts A and B, include
the project 2 identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

Get a copy of the latest Makefile from our GitLab server:
https://gitlab.eecs.umich.edu/eecs281/makefile

The P2random.h and P2random.cpp files are needed for Part A; all of the other
files are needed for Part B.

You can use the commands "make alltests" or "make testPQ" to try out the
priority queue tester.  You should add more code and more tests to the
testPQ.cpp file!

There are two versions of the Unordered priority queue: UnorderedPQ.h and
UnorderedFastPQ.h.  The 'fast' version uses a mutable member variable to
change the majority of calls to pop() from O(n) to O(1).  Both versions
are complete, and can be used as an example of how to use this->compare.
