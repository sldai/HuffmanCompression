#ifndef HCNODE_HPP
#define HCNODE_HPP
#include <iostream>
typedef unsigned char my_byte;

using namespace std;

/**
 * A class, instances of which are nodes in an HCTree.
 */
class HCNode {
    friend bool comp(HCNode* one, HCNode* other);

public:
    int count;
    my_byte symbol; // my_byte in the file we're keeping track of
    HCNode* c0;  // pointer to '0' child
    HCNode* c1;  // pointer to '1' child
    HCNode* p;   // pointer to parent
    bool is_leaf; // leaf sign

    HCNode(int count, my_byte symbol, HCNode* c0 = nullptr, HCNode* c1 = nullptr, HCNode* p = nullptr)
        : count(count), symbol(symbol), c0(c0), c1(c1), p(p), is_leaf(false) {}
    ~HCNode()
    {
        if (c0 != nullptr)
        {
            delete c0;
        }
        if (c1 != nullptr)
        {
            delete c1;
        }
    }
    /** Less-than comparison, so HCNodes will work in std::priority_queue
     *  We want small counts to have high priority.
     *  And we want to break ties deterministically.
     */
    bool operator<(const HCNode& other);
};

/**
 * For printing an HCNode to an ostream
 * Possibly useful for debugging.
 */
//ostream& operator<<(ostream&, const HCNode&) __attribute__((weak)); // shut the linker up
//ostream& operator<<(ostream& stm, const HCNode& n) {
//    stm << "[" << n.count << "," << (int) (n.symbol) << "]";
//    return stm;
//}

bool comp(HCNode* one, HCNode* other);
#endif // HCNODE_HPP
