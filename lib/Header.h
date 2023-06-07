#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstddef>
#include <cmath>
#include <queue>

char *intToByte(int number)
{
    union 
    {
        unsigned int integer;
        char byte[4];
    } t;
    t.integer = number;

    char *result = new char[4];
    for (int i = 0; i < 4; i++)
        result[i] = t.byte[i];

    return result;
}

int byteToInt(char *number)
{
    union 
    {
        unsigned int integer;
        char byte[4];
    } t;
    for (int i = 0; i < 4; i++)
        t.byte[i] = number[i];

    return t.integer;
}