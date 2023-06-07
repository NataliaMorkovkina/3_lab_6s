#pragma once

#include "Header.h"

const static int lds = 8;
const static double eps = 1e-6;
char *doubleToByte(double number)
{
    union 
    {
        double integer;
        char byte[lds];
    } t;
    t.integer = number;

    char *result = new char[lds];
    for (int i = 0; i < lds; i++)
        result[i] = t.byte[i];

    return result;
}

double byteToDouble(char *number)
{
    union 
    {
        double integer;
        char byte[lds];
    } t;
    for (int i = 0; i < lds; i++)
        t.byte[i] = number[i];

    return t.integer;
}

const static int repiatNumber = 9;
// const static double ACsize = 1;


void ACCompression(const std::string &inputFileName, const std::string &outputFileName)
{
    std::fstream fin(inputFileName, std::ios::in);
    if (!fin.is_open())
    {
        std::cout << "Error opening file " << inputFileName << std::endl;
        return;
    }
    fin << std::noskipws;

    std::fstream fout(outputFileName, std::ios::out | std::ios::binary);
    if (!fout.is_open())
    {
        std::cout << "Error opening file " << outputFileName << std::endl;
        return;
    }

    char c;

    std::map<char, int> mapV;
    while (fin.get(c))
        mapV[c]++;


    std::vector<std::pair<char, int>> v;
    for (auto it = mapV.begin(); it != mapV.end(); it++)
        v.push_back({it->first, it->second});


    for (size_t i = 1; i < v.size(); i++)
        v[i].second += v[i - 1].second;
    double sum = v[v.size() - 1].second;

    char *numberInChar = intToByte(v.size());
    for (int j = 0; j < 4; j++)
        fout << numberInChar[j];

    for (size_t i = 0; i < v.size(); i++)
    {
        fout << v[i].first;
        numberInChar = intToByte(v[i].second);
        for (int j = 0; j < 4; j++)
            fout << numberInChar[j];
    }

    fin.close();
    fin.open(inputFileName, std::ios::in);
    fin << std::noskipws;

    std::map<char, std::pair<double, double>> range;
    for (size_t i = 0; i < v.size(); i++)
        // range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum * ACsize, v[i].second / sum * ACsize};
        range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum, v[i].second / sum};

    // for (size_t i = 0; i < v.size(); i++)
    //     std::cout << v[i].first << ' ' << range[v[i].first].first << " " << range[v[i].first].second << std::endl;


    int n = 0;
    double l = 0, r = 1;
    while((fin.get(c)))
    {
        if (n == repiatNumber)
        {
            char *number = doubleToByte((l + r) / 2);
            for (int j = 0; j < lds; j++)
                fout << number[j];
            delete[] number;

            l = 0;
            r = 1;
            n = 0;
        }
        double newL = l + (r - l) * range[c].first;
        double newR = l + (r - l) * range[c].second;

        l = newL;
        r = newR;
        n++;
    }
    char *number = doubleToByte((l + r) / 2);
    for (int j = 0; j < lds; j++)
        fout << number[j];
    delete[] number;


    fin.close();
    fout.close();
}

void ACDecompression(const std::string &inputFileName, const std::string &outputFileName)
{
    std::fstream fin(inputFileName, std::ios::in | std::ios::binary);
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


    char *numberInChar = new char[4];
    fin.read(numberInChar, 4);
    // for (int j = 0; j < byteNumber; j++)
    //     std::cout << int(numberInChar[j]) << "\n";
    
    int n = byteToInt(numberInChar);
    // std::cout << n << std::endl;

    std::vector<std::pair<char, int>> v(n);
    for (int i = 0; i < n; i++)
    {
        fin.get(v[i].first);
        
        fin.read(numberInChar, 4);
        v[i].second = byteToInt(numberInChar);
    }
    delete[] numberInChar;
    double sum = v[v.size() - 1].second;

    std::map<char, std::pair<double, double>> range;
    for (size_t i = 0; i < v.size(); i++)
        range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum, v[i].second / sum};
        // range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum, v[i].second / sum};        

    double numberDouble = 0;
    char *number = new char[lds];
    while(fin.read(number, lds))
    {
        numberDouble = byteToDouble(number);

        for (int iter = 0; iter < repiatNumber; iter++)
        {
            double l = 0, r = 1;
            for (auto it = range.begin(); it != range.end(); it++)
                if (numberDouble >= it->second.first && numberDouble < it->second.second)
                {
                    // std::cout << "in range(" << it->second.first << ", " << it->second.second << "): " << it->first << std::endl;
                    fout << it->first;
                    l = it->second.first;
                    r = it->second.second;
                    break;
                }
            // std::cout << numberDouble << ' ' << l << ' ' << r << " ~ ";
            numberDouble = (numberDouble - l) / (r - l);
            // std::cout << numberDouble << std::endl;
        }
    }
    // std::cout << countSymbol << std::endl;

    fin.close();
    fout.close();
}
