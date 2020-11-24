#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP
#include <iostream>
#include <fstream>
using namespace std;

class BitOutputStream {
    private:
        ofstream& fout;     // the output stream to use
        char buffer;      // one byte buffer of bits
        int buffer_index; // current index in buffer     

    public:
        BitOutputStream(ofstream& s) : fout(s), buffer(0), buffer_index(0) {}
        void put_byte(char c);
        void put_bit(unsigned int bit);
        void open(const string fname) { fout.open(fname); buffer = 0; buffer_index = 0; }
        void close() { fout.close(); buffer = 0; buffer_index = 0; }
};
#endif 
