#include "BitInputStream.hpp"

// Extracting the nth Bit From the Right
unsigned int extractBit(unsigned char input, unsigned int n) {
	// YOUR CODE HERE
	unsigned int output = (unsigned char)(input >> n) & 1;
	return output;
}

bool BitInputStream::get_byte(char& c)
{
	if (this->fin.get(c)) 
		return true;
	else 
		return false;
}

bool BitInputStream::get_bit(unsigned int& bit)
{
	if (this->buffer_index == 8)
	{
		if(this->fin.get(this->buffer))
			this->buffer_index = 0;
		else
			return false;
	}

	bit = extractBit(this->buffer, 7 - this->buffer_index);
	this->buffer_index++;
	return true;
}

