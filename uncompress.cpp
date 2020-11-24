#include "HCTree.hpp"
#include <fstream>
#include <iostream>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include <bitset>
#include <stdlib.h>
#include <time.h>
using namespace std;

//void test_bit_input_output()
//{
//    ifstream fin("test_bit.txt");
//    BitInputStream bit_fin(fin);
//    while (true)
//    {
//        unsigned int bit;
//        if (!bit_fin.get(bit))
//        {
//            break;
//        }
//        cout << bit;
//    }
//    fin.close();
//    cout << endl;
//
//
//    ofstream fout;
//    fout.open("test_output_bit.txt");
//    BitOutputStream bit_fout(fout);
//    srand(time(NULL));
//    for (size_t i = 0; i < 32; i++)
//    {
//        unsigned int c = rand() % 2;
//        bit_fout.put(c);
//        cout << c;
//    }
//    fout.close();
//    cout << endl;
//
//    fin.open("test_output_bit.txt");
//    char c;
//    while (fin.get(c))
//    {
//        bitset<8> x(c);
//        cout << x;
//    }
//    fin.close();
//}

int uncompress_main(int argc, char const* argv[])
{
    // Parse the command line arguments and throw an error message if the user runs your program incorrectly
    if (argc != 3)
    {
        cout << "Usage:" << endl << "./compress <original_file> <compressed_file>";
        return -1;
    }
    string input_fname = argv[1];
    string output_fname = argv[2];

    // Open the input file for reading
    cout << "Opening the file" << endl;
    ifstream fin;
    fin.open(input_fname, ios_base::binary);
    if (fin.fail())
    {
        cout << "[!] Input file cannot be opened" << endl;
        return -1;
    }

    ofstream fout;
    fout.open(output_fname, ios_base::binary);
    if (fout.fail())
    {
        cout << "[!] Output file cannot be opened" << endl;
        return -1;
    }

    BitInputStream bit_fin(fin);

    // decoding
    HCTree new_tree;

    cout << "Read head" << endl;
    map<char, unsigned int> char_cnt = new_tree.read_head(bit_fin);
    cout << "The counts of char:" << endl;
    for (auto iter = char_cnt.begin(); iter != char_cnt.end(); iter++)
    {
        cout << iter->first << ": " << iter->second << endl;
    }

    cout << "Build Huffman tree" << endl;
    new_tree.build_from_cnt(char_cnt);
    string encode_msg = new_tree.print_encode_msg();
    cout << "Huffman code:" << endl << encode_msg;

    cout << "Decoding data" << endl;
    while (true)
    {
        char symbol = new_tree.decode(bit_fin);
        if (!fin.eof())
        {   
            fout << symbol;
        }
        else
        {
            unsigned int b;
            if (bit_fin.get_bit(b))
                cout << "file not end" << endl;
            else
                cout << "file end" << endl;
            break;
        }
    }
    fin.close();
    fout.close();
    return 0;
}

int main(int argc, char const *argv[])
{
    return uncompress_main(argc, argv);
    //test_bit_input_output();
    //return 1;
}
