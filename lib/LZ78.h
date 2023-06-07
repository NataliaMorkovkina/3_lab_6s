#pragma once

#include "Header.h"

struct LZ78Node
{
	int index;
	std::string data;
	LZ78Node *next;

    LZ78Node(int _index, std::string _data)
    {
        index = _index;
        data = _data;
        next = NULL;
    }
};

class LZ78Tree
{
private:
    LZ78Node *head;

public:
    LZ78Tree(){}

    LZ78Tree(int index, std::string data)
    {
        head = new LZ78Node(index, data);
    }

    void insert_Node(int index, std::string data){
        LZ78Node *new_Node = new LZ78Node (index, data);

        LZ78Node *curr = head;
        while (curr != NULL)
        {
            if (curr->next == NULL)
            {
                curr->next = new_Node;
                return;
            }
            curr = curr->next;
        }
    }

    LZ78Node *search_Node(std::string data)
    {
        LZ78Node *curr = head;
        while (curr != NULL)
        {
            if (data == curr->data)
                return curr;
            else
                curr = curr->next;
        }
        return NULL;
    }

    LZ78Node *search_Node(int index)
    {
        LZ78Node *curr = head;
        while (curr != NULL)
        {
            if (index == curr->index)
                return curr;
            else
                curr = curr->next;
        }
        return NULL;
    }

    bool delete_Node(LZ78Node *to_delete){
        if (to_delete == NULL)
            return false;
        else if (to_delete == head)
        {
            head = to_delete->next;
            delete to_delete;
            return true;
        }
        else{
            LZ78Node *curr = head;
            while (curr)
            {
                if (curr->next == to_delete)
                {
                    curr->next = to_delete->next;
                    delete to_delete;
                    return true;
                }
                curr = curr->next;
            }
            return false;
        }
    }

};


void LZ78Compression(const std::string &inputFileName, const std::string &outputFileName)
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
    
    int last_seen, index = 1;

    char c;
    fin.get(c);
    std::string word; word = c;
    LZ78Tree dictionary(1, word);
    
    char *t = intToByte(0);
    for (int i = 0; i < 4; i++)
        fout << t[i];
    fout << c;


    while (fin.get(c))
    {
        std::string data; data = c;

    re_check:
        LZ78Node *search = dictionary.search_Node(data);

        if (search)
        {
            fin.get(c);
            data += c;
            last_seen = search->index;
            goto re_check;
        }
        else
        {
            char zero;
            if (c == ' ')
                zero = '0';
            else
                zero = c;

            if ((int)data.length() < 2)
            {
                t = intToByte(0);
                for (int i = 0; i < 4; i++)
                    fout << t[i];
                fout << zero;
            }
            else
            {
                t = intToByte(last_seen);
                for (int i = 0; i < 4; i++)
                    fout << t[i];
                fout << zero;
            }
            index++;
            
            dictionary.insert_Node(index, data);
        }
    }
}

void LZ78Decompression(const std::string &inputFileName, const std::string &outputFileName)
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

    LZ78Tree dictionary;
    std::string result;

    std::vector<std::pair<int, char>> s;
    int i = 0;
    char c;
    char *num = new char[4];
    while (fin.get(c))
    {
        if ((i + 1) % 5)
            num[i % 5] = c;
        else
            s.push_back({byteToInt(num), c});
        i++;
    }
    
    int zz = 2;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (i == 0)
        {
            std::string data; data = s[i].second;
            dictionary = LZ78Tree(1, data);
            fout << data;
        }
        else
        {
            LZ78Node *serched;
            std::string get_search; get_search = s[i].second;
            serched = dictionary.search_Node(s[i].first);
            if (serched)
            {
                fout << serched->data << get_search;
                get_search = serched->data + s[i].second;
                dictionary.insert_Node(zz, get_search);
            }
            else
            {
                if (s[i].first == 0)
                    dictionary.insert_Node(zz, get_search);
                else
                    dictionary.insert_Node(zz, get_search);

                fout << get_search;
            }
            zz++;
        }
    }
}
