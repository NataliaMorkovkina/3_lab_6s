#pragma once

#include "Header.h"

static std::string BWTdata;

std::string shiftString(const int &n)
{
    return std::string(BWTdata.begin() + n, BWTdata.end()) + std::string(BWTdata.begin(), BWTdata.begin() + n);
}

bool BWTcmp1(const int &a, const int &b)
{
    return shiftString(a) < shiftString(b);
}

bool BWTcmp2(const std::pair<char, int> &a, const std::pair<char, int> &b)
{
    return a.first < b.first;
}

char lastAfterShiftString(const int &n)
{
    if (n == 0)
        return BWTdata[BWTdata.size() - 1];
    return BWTdata[n - 1];
}

void BWTCompression(const std::string &inputFileName, const std::string &outputFileName)
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
    while(fin.get(c))
        BWTdata += c;
    BWTdata += '\000';

    std::vector<int> v;
    for (size_t i = 0; i < BWTdata.size(); i++)
        v.push_back(i);
    
    std::sort(v.begin(), v.end(), BWTcmp1);

    std::string data;
    for (size_t i = 0; i < BWTdata.size(); i++)
        data += lastAfterShiftString(v[i]);

    fout << data;
    BWTdata.clear();
}

void BWTDecompression(const std::string &inputFileName, const std::string &outputFileName)
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
    std::vector<std::pair<char, int>> v;
    std::string s;
    for (int i = 0; fin.get(c); i++)
    {
        s += c;
        v.push_back({c, i});
    }

    std::sort(v.begin(), v.end(), BWTcmp2);

    std::string res;
    int endi = 0;
    for (size_t i = 0; i < v.size(); i++)
        if (s[i] == '\000')
        {
            endi = i;
            break;
        }

    int i = endi;
    while (v[i].second != endi)
    {
        res += v[i].first;
        i = v[i].second;
    }

    fout << res;
}
