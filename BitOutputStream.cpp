#include "BitOutputStream.hpp"

// Setting the nth Bit From the Right
unsigned char setBit(unsigned char input, unsigned int n, unsigned int bit) {
	// YOUR CODE HERE
	unsigned char output = input & (unsigned char)(~(1 << n)) | (unsigned char)(bit << n);
	return output;
}

void BitOutputStream::put_byte(char c)
{
	this->fout.put(c);
}

void BitOutputStream::put_bit(unsigned int bit)
{
	this->buffer = setBit(this->buffer, 7 - this->buffer_index, bit);
	this->buffer_index++;
	if (this->buffer_index == 8)
	{
		this->fout.put(this->buffer);
		this->buffer_index = 0;
		this->buffer = 0;
	}
}
