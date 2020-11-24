#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP
#include <iostream>
#include <fstream>
using namespace std;

class BitInputStream {
    private:
        ifstream& fin;      // the input stream to use
        char buffer;      // one byte buffer of bits
        int buffer_index; // current index in buffer

    public:
        BitInputStream(ifstream& s) : fin(s), buffer(0), buffer_index(8) {}
        bool get_byte(char& c);
        bool get_bit(unsigned int& bit);
        void open(const string fname) { fin.open(fname); buffer = 0; buffer_index = 8; }
        void close() { fin.close(); buffer = 0; buffer_index = 8; }
};
#endif
