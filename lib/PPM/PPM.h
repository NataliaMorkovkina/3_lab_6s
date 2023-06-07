#include <cstring>
#include <string>
#include "Application.h"

void PPMCompression(const std::string &inputFileName, const std::string &outputFileName)
{
	char *ifn = new char(inputFileName.size()+1); strcpy(ifn, inputFileName.c_str());
	char *ofn = new char(outputFileName.size()+1); strcpy(ofn, outputFileName.c_str());
	Application app('c', ifn, ofn);
	app.run();
}

void PPMDecompression(const std::string &inputFileName, const std::string &outputFileName)
{
	char *ifn = new char(inputFileName.size()+1); strcpy(ifn, inputFileName.c_str());
	char *ofn = new char(outputFileName.size()+1); strcpy(ofn, outputFileName.c_str());
	Application app('d', ifn, ofn);
	app.run();
}
