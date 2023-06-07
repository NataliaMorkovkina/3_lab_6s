#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>

class FileIO {
	std::ifstream fin;
	std::ofstream fout;
	int bit_count;
	int buffer;
	void flush_bits();      /* flush the remaining bits when finishing encoding */
public:
	FileIO(char* fn_in, char* fn_out);
	~FileIO();
	void put_bit(int bit);  /* write a bit to the output file - for encoding */
	void put_char(int c);
	int get_bit();          /* read a bit from the input file - for decoding */
	int get_char();
};

FileIO::FileIO(char* fn_in, char* fn_out) {
	bit_count = 0;
	buffer = 0;
	fin.open(fn_in, std::ios::binary);
	fout.open(fn_out, std::ios::binary);
}

FileIO::~FileIO() {
	flush_bits();
	fout.flush();
	fin.close();
	fout.close();
}

void FileIO::put_bit(int bit) {
	buffer <<= 1;
	buffer |= bit;
	if (++bit_count == 8) {
		fout.put((char) buffer);
		bit_count = 0;
		buffer = 0;
	}
}

void FileIO::put_char(int c) {
	fout.put((char) c);
}

int FileIO::get_bit() {
	static int bit_count = 0;
	static int buffer = 0;
	if (bit_count == 0) {
		buffer = (int) fin.get();
		if (buffer == EOF)
			return 0;
		else
			bit_count = 8;
	}
	return (buffer >> --bit_count) & 1;
}

int FileIO::get_char() {
	return fin.get();
}

void FileIO::flush_bits() {
	while (bit_count > 0)
		put_bit(0);
}

#endif