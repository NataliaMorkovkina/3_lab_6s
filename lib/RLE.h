#pragma once

#include "Header.h"

void RLECompression(const std::string &inputFileName, const std::string &outputFileName)
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

    char c;
    std::string buff;
    unsigned char count = 0;

    while (fin.get(c))
    {
        if (buff.empty())
        {
            buff += c;
            count = 1;
            continue;
        }

        if (c == buff.back())
            if (buff.size() == 1)
            {
                if (count == 255)
                {
                    fout << count << buff;
                    buff.clear();
                    count = 0;
                }
                else
                    count++;
            }
            else
            {
                buff.pop_back();
                fout << '-' << (unsigned char)buff.size() << buff;
                buff.clear();
                buff += c;
                count = 2;
            }
        else
            if (count > 1)
            {
                fout << count << buff;
                buff.clear();
                buff += c;
                count = 1;
            }
            else
            {
                buff += c;
                if (buff.size() == 255)
                {
                    buff.pop_back();
                    fout << '-' << (unsigned char)buff.size() << buff;
                    buff = c;
                    count = 1;
                }
            }
    }
    if (count > 1)
        fout << count << buff;
    else
        fout << '-' << (unsigned char)buff.size() << buff;
}

void RLEDecompression(const std::string &inputFileName, const std::string &outputFileName)
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

    char c;
    unsigned char count;

    while (fin >> count)
    {
        if (count == '-')
        {
            fin >> count;
            for (int i = 0; i < count; i++)
            {
                fin.get(c);
                fout << c;
            }
        }
        else
        {
            fin.get(c);
            for (int i = 0; i < count; i++)
                fout << c;
        }
    }
}
