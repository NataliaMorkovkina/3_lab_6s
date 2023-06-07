/************************************************/
/* SimpleBitIOStream.h, (c) Rene Puchinger      */
/*                                              */
/* one-bit-oriented input and output            */
/************************************************/

#ifndef SIMPLEBITIOSTREAM_H
#define SIMPLEBITIOSTREAM_H

#include "IOStream.h"

class SimpleBitInputStream {
	InputStream* in;
	int bit_count;
	int buffer;
public:
	SimpleBitInputStream(InputStream* _in);
	int get_bit();
};

class SimpleBitOutputStream {
	OutputStream* out;
	int bit_count;
	int buffer;
public:
	SimpleBitOutputStream(OutputStream* _out);
	void put_bit(int but);
	void flush_bits();
	void flush() { flush_bits(); out->flush(); }
};

SimpleBitInputStream::SimpleBitInputStream(InputStream* _in) {
	in = _in;
	bit_count = 0;
	buffer = 0;
}

int SimpleBitInputStream::get_bit() {
	if (bit_count == 0) {
		buffer = in->get_char();
		if (buffer == EOF)
			return 0;
		else
			bit_count = 8;
	}
	return (buffer >> --bit_count) & 1;
}

/*****************************************************************************************/

SimpleBitOutputStream::SimpleBitOutputStream(OutputStream* _out) {
	out = _out;
	bit_count = 0;
	buffer = 0;
}

void SimpleBitOutputStream::put_bit(int bit) {
	buffer <<= 1;
	buffer |= bit;
	if (++bit_count == 8) {
		out->put_char(buffer);
		bit_count = 0;
		buffer = 0;
	}
}

void SimpleBitOutputStream::flush_bits() {
	while (bit_count > 0)
		put_bit(0);
}

#endif