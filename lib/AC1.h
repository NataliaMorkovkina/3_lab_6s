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

// const static int comp = 4;
// const static double ACsize = 1;
const static int byte = 256;
const static int byteNumber = 2;
const static double lStart = 0;
const static double rStart = 1;

int byteToInt(char *number, const int &byteNumber)
{
    union 
    {
        int integer;
        char byte[4];
    } t;
    for (int i = 0; i < byteNumber; i++)
        t.byte[i] = number[i];

    return t.integer;
}

char *intToByte(int number, const int &byteNumber)
{
    union 
    {
        int integer;
        char byte[4];
    } t;
    t.integer = number;

    char *result = new char[byteNumber];
    for (int i = 0; i < byteNumber; i++)
        result[i] = t.byte[i];

    return result;
}

double perenos(double x)
{
    return (x - 0.5) * (rStart - lStart) + (rStart + lStart) / 2;
}

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
    int abcde = 0;
    long long numberChar = 0;
    long long last = -1;
    while (fin.get(c))
    {
        // if (abcde++ >= 5)
        //     break;
        std::map<char, int> mapV;
        int countSymbol = 1;
        do
            mapV[c]++;
        while (fin.get(c) && ++countSymbol < std::pow(byte, byteNumber));


        std::vector<std::pair<char, int>> v;
        for (auto it = mapV.begin(); it != mapV.end(); it++)
            v.push_back({it->first, it->second});


        for (size_t i = 1; i < v.size(); i++)
            v[i].second += v[i - 1].second;
        double sum = v[v.size() - 1].second;

        char *numberInChar = intToByte(v.size(), byteNumber);
        for (int j = 0; j < byteNumber; j++)
            fout << numberInChar[j];

        for (size_t i = 0; i < v.size(); i++)
        {
            fout << v[i].first;
            numberInChar = intToByte(v[i].second, byteNumber);
            for (int j = 0; j < byteNumber; j++)
                fout << numberInChar[j];
        }

        while (countSymbol--)
            fin.unget();

        std::map<char, std::pair<double, double>> range;
        for (size_t i = 0; i < v.size(); i++)
            // range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum * ACsize, v[i].second / sum * ACsize};
            range[v[i].first] = {i == 0 ? 0 : perenos(v[i - 1].second / sum), perenos(v[i].second / sum)};

        // for (size_t i = 0; i < v.size(); i++)
        //     std::cout << v[i].first << ' ' << range[v[i].first].first << " " << range[v[i].first].second << std::endl;


        // int n = comp, i = -1;
        double l = lStart, r = rStart;
        bool repeat = false;
        while(repeat || (fin.get(c) && ++countSymbol < std::pow(byte, byteNumber)))
        {
            repeat = false;
            double newL = l + (r - l) * range[c].first;
            double newR = l + (r - l) * range[c].second;
            
            double m = (newL + newR) / 2;
            int iter = 0;
            for (; iter < 32; iter++)
            {
                // std::cout << numberDouble << ' ';
                double l1 = 0, r2 = 1;
                for (auto it = range.begin(); it != range.end(); it++)
                    if (m >= it->second.first && m < it->second.second)
                    {
                        // std::cout << "in range(" << it->second.first << ", " << it->second.second << "): " << it->first << std::endl;
                        // fout << it->first;
                        l1 = it->second.first;
                        r2 = it->second.second;
                        break;
                    }
                    
                // std::cout << m << ' ' << l1 << ' ' << r2 << " ~ ";
                m = (m - l1) / (r2 - l1);
                // std::cout << m << ' ' << std::abs(m - 0.5) << ' ' << eps << std::endl;

                if (std::abs(m - 0.5) <= eps)
                {
                    // std::cout << iter << " ok\n";
                    break;
                }
            }
            // std::cout << iter << " iter\n";

            if (iter == 32)
            {
                char *number = doubleToByte((l + r) / 2);
                for (int j = 0; j < lds; j++)
                    fout << number[j];
                l = lStart;
                r = rStart;
                repeat = true;
                delete[] number;
            }
            else
            {
                l = newL;
                r = newR;
                numberChar++;
            }

            // if ((long long)((numberChar * 1000) / 1e7) != last)
            // {
            //     last = (long long)((numberChar * 1000) / 1e7);
            //     std::cout << last / 1000 << '.' << last % 1000 << "%\n";
            // }
        }
        if (l != 0 && r != 1)
        {
            char *number = doubleToByte((l + r) / 2);
            for (int j = 0; j < lds; j++)
                fout << number[j];
            delete[] number;
        }
    }
    // std::cout << numberChar << std::endl;

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

    char c;
    while (fin.get(c))
    {
        fin.unget();
        char *numberInChar = new char[byteNumber];
        fin.read(numberInChar, byteNumber);
        // for (int j = 0; j < byteNumber; j++)
        //     std::cout << int(numberInChar[j]) << "\n";
        
        int n = byteToInt(numberInChar, byteNumber);
        // std::cout << n << std::endl;

        std::vector<std::pair<char, int>> v(n);
        for (int i = 0; i < n; i++)
        {
            fin.get(v[i].first);
            
            fin.read(numberInChar, byteNumber);
            v[i].second = byteToInt(numberInChar, byteNumber);
        }
        delete[] numberInChar;
        double sum = v[v.size() - 1].second;

        std::map<char, std::pair<double, double>> range;
        for (size_t i = 0; i < v.size(); i++)
            range[v[i].first] = {i == 0 ? 0 : perenos(v[i - 1].second / sum), perenos(v[i].second / sum)};
            // range[v[i].first] = {i == 0 ? 0 : v[i - 1].second / sum, v[i].second / sum};        

        double numberDouble = 0;
        char *number = new char[lds];
        int countSymbol = 0;
        while(countSymbol < std::pow(byte, byteNumber) && fin.read(number, lds))
        // fin.read(number, lds);
        {
            numberDouble = byteToDouble(number);

            int iter = 0;
            for (; iter < 32 && std::abs(numberDouble - 0.5) > eps; iter++)
            {
                // std::cout << numberDouble << ' ';
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
            countSymbol += iter;
        }
        // std::cout << countSymbol << std::endl;
    }

    fin.close();
    fout.close();
}
