#include "HCNode.hpp"
using namespace std;

/**
 * Compare this HCNode and other for priority ordering
 * Smaller count means higher priority
 * Use node symbol for deterministic tie-breaking
 */
bool HCNode::operator<(const HCNode& other){
    // if the counts are different, compare counts
    if(count != other.count){
        return count > other.count;
    }

    // if the counts are equal, use symbol to break tie
    return symbol > other.symbol;
}

/**
 * To compare two HCNode* objects, we can compare the HCNode objects they point to
 */
bool comp(HCNode* one, HCNode* other){
    return *one < *other;
}
