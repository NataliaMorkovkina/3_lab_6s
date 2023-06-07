/************************************************/
/* FileIOStream.h, (c) Rene Puchinger           */
/*                                              */
/* classes for buffered file input and output   */
/* operations; supports only files              */
/* smaller than 2 GB                            */
/************************************************/

#ifndef FILEIOSTREAM_H
#define FILEIOSTREAM_H

#include <cstdio>
#include "IOStream.h"
#include "Exception.h"

class FileInputStream: public InputStream {
	static const long BUF_SIZE = 65535;    /* buffer size */
	char* buffer;
	long buf_pos;
	long buf_size;                         /* real buffer size */
	FILE* fp;
public:
	FileInputStream(const char* fn);       /* fn = input file name */
	~FileInputStream();
	int get_char();                        /* get a character from input */
	long get_size();                       /* get the file size */
};

class FileOutputStream: public OutputStream {
	static const long BUF_SIZE = 65535;    /* buffer size */
	char* buffer;
	long buf_pos;
	FILE* fp;
public:
	FileOutputStream(const char* fn);      /* fn = output file name */
	~FileOutputStream();
	void put_char(int c);                  /* put a character to output */
	void flush();                          /* flush the file buffer */
};

FileInputStream::FileInputStream(const char* fn) {
	buffer = new char[BUF_SIZE];
	if (buffer == NULL)
		throw Exception(Exception::ERR_MEMORY);
	buf_pos = 0;
	buf_size = 0;
	fp = fopen(fn, "rb");
	if (fp == NULL)
		throw Exception(Exception::ERR_FILE_NOT_FOUND);
}

FileInputStream::~FileInputStream() {
	fclose(fp);
}

int FileInputStream::get_char() {
	if (buf_pos > 0 && buf_size == 0)
		return EOF;
	if (buf_pos == buf_size) {
		buf_size = (long) fread((void*) buffer, sizeof(char), BUF_SIZE, fp);
		if (buf_size == 0)
			return EOF;
		buf_pos = 0;
	}
	return (int) ((unsigned char) buffer[buf_pos++]);
}

long FileInputStream::get_size() {
	long cur_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	long result = ftell(fp);
	fseek(fp, cur_pos, SEEK_SET);
	return result;
}

FileOutputStream::FileOutputStream(const char* fn) {
	buffer = new char[BUF_SIZE];
	if (buffer == NULL)
		throw Exception(Exception::ERR_MEMORY);
	buf_pos = 0;
	fp = fopen(fn, "wb");
	if (fp == NULL)
		throw Exception(Exception::ERR_FILE_ACCESS);
}

FileOutputStream::~FileOutputStream() {
	flush();
	fclose(fp);
}

void FileOutputStream::put_char(int c) {
	if (buf_pos == BUF_SIZE) {
		if ((long) fwrite((void*) buffer, sizeof(char), BUF_SIZE, fp) != BUF_SIZE)
			throw Exception(Exception::ERR_FILE_WRITE);
		buf_pos = 0;
	}
	buffer[buf_pos++] = (char) c;
}

void FileOutputStream::flush() {
	if (buf_pos > 0 && fwrite((void*) buffer, sizeof(char), buf_pos, fp) != buf_pos)
			throw Exception(Exception::ERR_FILE_WRITE);
	buf_pos = 0;
	fflush(fp);
}

#endif