#include <iostream>
#include <fstream>
#include <map>
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;

int main(int argc, char const *argv[])
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


    // Read bytes from the file. Count the number of occurrences of each byte value. Close the file
    cout << "Reading the file" << endl;
    map<char, unsigned int> char_cnt;
    char c;
    while (fin.get(c))
    {
        if(char_cnt.find(c) == char_cnt.end())
        {
            char_cnt[c] = 0;
        }
        char_cnt[c]++;
    }
    fin.close();

    cout << "The counts of char:" << endl;
    for (auto iter = char_cnt.begin(); iter != char_cnt.end(); iter++)
    {
        cout << iter->first << ": " << iter->second << endl;
    }
    
    // Use the byte counts to construct a Huffman coding tree
    cout << "Building the Huffman coding tree" << endl;
    HCTree tree;
    tree.build_from_cnt(char_cnt);
    string encode_msg = tree.print_encode_msg();
    cout << "Huffman code:" << endl << encode_msg;

    // testing 
    {
        // encoding
        //ifstream fin;
        //fin.open(input_fname);
        //ofstream fout;
        //fout.open(output_fname);
        //cout << "Encoding head" << endl;
        //tree.write_head(fout);

        //cout << "Encoding data" << endl;
        //char c;
        //while (fin.get(c))
        //{
        //    tree.test_encode(c, fout);
        //}
        //cout << "End encoding" << endl;
        //fin.close();
        //fout.close();

        //// decoding
        //fin.open(output_fname);
        //fout.open("decode.txt");
        //HCTree new_tree;
        //
        //cout << "Read head" << endl;
        //map<char, unsigned int> char_cnt = new_tree.read_head(fin);

        //cout << "Build Huffman tree" << endl;
        //new_tree.build_from_cnt(char_cnt);

        //cout << "Decoding data" << endl;
        //while (true)
        //{
        //    char symbol = new_tree.test_decode(fin);
        //    if (!fin.eof())
        //    {
        //        fout << symbol;
        //    }
        //    else
        //    {
        //        break;
        //    }
        //}
        //fin.close();
        //fout.close();
    }

    // write bit
    {
        ifstream fin;
        fin.open(input_fname, ios_base::binary);
        ofstream fout;
        fout.open(output_fname, ios_base::binary);
        BitOutputStream bit_fout(fout);

        cout << "Encoding head" << endl;
        tree.write_head(bit_fout);

        cout << "Encoding data" << endl;
        char c;
        while (fin.get(c))
        {
            tree.encode(c, bit_fout);
        }
        cout << "End encoding" << endl;
        fin.close();
        bit_fout.close();
    }
    return 0;
}
