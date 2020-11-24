Huffman Coding Algorithm
------------------------

About
-----

This program follows Huffman coding, which is a common lossless compression method. It counts each character in the original file and builds the Huffman coding tree, which assigns short codewords for frequent characters. The compressed file includes a head and the encoded data. For uncompression, the programs rebuilds the Huffman tree via reading the head, and decodes data.

Usage
-----

Compression:

```
./compress <input file> <output file>
```

Uncompression:

```
./uncompress <input file> <output file>
```

The binary files are compiled in Windows, for compilation just run the make file.


Compressed File Structure
-------------------------

<table>
<tbody>
  <tr>
    <td colspan="2">N = total number of unique characters[1 byte]</td>
  </tr>
  <tr>
    <td>Character[1 byte]</td>
    <td>Count[4 byte]</td>
  </tr>
  <tr>
    <td>Character[1 byte]</td>
    <td>Count[4 byte]</td>
  </tr>
  <tr>
    <td colspan="2">N times</td>
  </tr>
  <tr>
    <td>P[1 byte]</td>
    <td>P times bit 0</td>
  </tr>
  <tr>
    <td colspan="2">Data</td>
  </tr>
</tbody>
</table>

P is the padding number, it ensures the file size fits in bytes. For example, the size of all codewords is 13 bits, 16-13=3 bits are used for padding.

Algorithm
---------

#### Compress

0. Read the input file and builds a map which contains existing characters and their counts.
1. Construct the Huffman coding tree
   ```
   create priority queue pq
   for each character in map 
       create new node q with character and count
       add q into pq
   loop until only one node remains, i.e, ROOT of H-tree
       create new node q, join two least freq nodes to its left and right
       add p into pq
   ```
2. For each leaf node, traverse up to the root to get its codeword.
3. Read input file again.
4. Write head to output file.
5. Read each character and write codeword bits to output file.

#### Uncompress

0. Read input file head.
1. Construct character count map from head.
2. Construct H-tree from map.
3. Decode data by searching H-tree.
4. Write characters to output file.
