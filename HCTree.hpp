#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include <map>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include <iostream>
#include <string>

using namespace std;

/**
 * A 'function class' for use as the Compare class in a priority_queue<HCNode*>.
 * For this to work, operator< must be defined to do the right thing on HCNodes.
 */
class HCNodePtrComp {
    public:
        bool operator()(HCNode*& lhs, HCNode*& rhs) const {
            return *lhs < *rhs;
        }
};

/**
 * A Huffman Code Tree class.
 * Not very generic: Use only if alphabet consists of unsigned chars.
 */
class HCTree {
    private:
        HCNode* root;
        vector<HCNode*> leaves;
        map<char, string> encode_msg;

    public:
        explicit HCTree() : root(nullptr) {
            leaves = vector<HCNode*>();
        }

        ~HCTree()
        {
            delete root;
        }

        /**
         * Use the Huffman algorithm to build a Huffman coding trie.
         * PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
         * the frequency of occurrence of byte i in the message.
         * POSTCONDITION: root points to the root of the trie,
         * and leaves[i] points to the leaf node containing byte i.
         */
        void build_from_cnt(map<char, unsigned int> char_cnt);

        void build_from_encode_msg();
        /**
         * Write to the given BitOutputStream
         * the sequence of bits coding the given symbol.
         * PRECONDITION: build() has been called, to create the coding
         * tree, and initialize root pointer and leaves vector.
         */
        void encode(my_byte symbol, BitOutputStream& out);


        /**
         * Return symbol coded in the next sequence of bits from the stream.
         * PRECONDITION: build() has been called, to create the coding
         * tree, and initialize root pointer and leaves vector.
         */
        my_byte decode(BitInputStream& in);


        void test_encode(my_byte symbol, ofstream& os);
        my_byte test_decode(ifstream& is);
        string print_encode_msg();
        void write_head(BitOutputStream& os);
        void write_padding(BitOutputStream& os, int num);
        void skip_padding(BitInputStream& is, int num);
        int calc_padding();
        map<char, unsigned int> read_head(BitInputStream& is);
};
#endif // HCTREE_HPP
