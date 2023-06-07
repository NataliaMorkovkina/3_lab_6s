#pragma once

#include "Header.h"

void MTFCompression(const std::string &inputFileName, const std::string &outputFileName)
{
    std::fstream fin(inputFileName, std::ios::in);
    if (!fin.is_open())
    {
        std::cout << "Error opening file " << inputFileName << std::endl;
        return;
    }
    fin << std::noskipws;

    std::fstream fout(outputFileName, std::ios::out);
    if (!fout.is_open())
    {
        std::cout << "Error opening file " << outputFileName << std::endl;
        return;
    }

    std::vector<char> v;
    char c = 0;
    do
        v.push_back(c++);
    while(c != 0);

    while(fin.get(c))
    {
        char i = 0;
        while(v[i] != c)
            i++;
        fout << i;
        v.erase(v.begin() + i);
        v.insert(v.begin(), c);
    }

    fin.close();
    fout.close();
}

void MTFDecompression(const std::string &inputFileName, const std::string &outputFileName)
{
    std::fstream fin(inputFileName, std::ios::in);
    if (!fin.is_open())
    {
        std::cout << "Error opening file " << inputFileName << std::endl;
        return;
    }
    fin << std::noskipws;

    std::fstream fout(outputFileName, std::ios::out);
    if (!fout.is_open())
    {
        std::cout << "Error opening file " << outputFileName << std::endl;
        return;
    }

    std::vector<char> v;
    char c = 0;
    do
        v.push_back(c++);
    while(c != 0);

    while(fin.get(c))
    {
        char c1 = v[c];
        fout << c1;
        v.erase(v.begin() + c);
        v.insert(v.begin(), c1);
    }
    
    fin.close();
    fout.close();
}
