/************************************************/
/* Application.h, (c) Rene Puchinger            */
/*                                              */
/* sample artihmetic compression program        */
/************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "SimpleBitIOStream.h"
#include "FileIOStream.h"
#include "Model.h"
#include <iostream>

class Application {
	enum { ENCODE, DECODE, IDLE } state;
	ARI_encoder* enc;
	ARI_decoder* dec;
	char* fn_in;
	char* fn_out;
public:
	Application(char c, char* ifn, char* ofn);
	~Application();
	void run();
};

Application::Application(char c, char* ifn, char* ofn) {
    enc = NULL;  // encoder
    dec = NULL;  // decoder
    
    if (c == 'c') {
        enc = new ARI_encoder;
        state = ENCODE;
        fn_in = ifn;
        fn_out = ofn;
    } else if (c == 'd') {
        dec = new ARI_decoder;
        state = DECODE;
        fn_in = ifn;
        fn_out = ofn;
    }
}

void Application::run() {
	if (state == ENCODE) {
		FileInputStream* fin = new FileInputStream(fn_in);
		FileOutputStream* fout = new FileOutputStream(fn_out);
		SimpleBitOutputStream* bout = new SimpleBitOutputStream(fout);
		enc->encode(fin, bout);
		std::cout << "File compressed successfully." << std::endl;
		if (fin) delete fin;
		if (fout) delete fout;
		if (bout) delete bout;
	} else if (state == DECODE) {
		FileInputStream* fin = new FileInputStream(fn_in);
		SimpleBitInputStream* bin = new SimpleBitInputStream(fin);
		FileOutputStream* fout = new FileOutputStream(fn_out);
		dec->decode(bin, fout);
		std::cout << "File decompressed successfully." << std::endl;
		if (fin) delete fin;
		if (bin) delete bin;
		if (fout) delete fout;
	}
}

Application::~Application() {
	if (enc)
		delete enc;
	if (dec)
		delete dec;
}

#endif
