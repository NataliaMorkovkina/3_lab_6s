#pragma once

#include "Header.h"



class huffman_tree_node
{
private:
	char data;									// Символ
	unsigned freq;								// Частота
	huffman_tree_node *left, *right;			// Левый и правый потомок
	friend class huffman_coding;

public:
	huffman_tree_node() {};
	huffman_tree_node(char _data, unsigned _freq);			// Создание узла
};

typedef huffman_tree_node *pointer_huffman;
typedef huffman_tree_node **p_pointer_huffman;

inline huffman_tree_node::huffman_tree_node(char _data, unsigned _freq)
{
	left = right = NULL;
	data = _data;
	freq = _freq;
}


class huffman_coding
{
public:
	pointer_huffman root_main;
	std::map <char, std::string> replace_abc;
	std::map <std::string, char> replace_abc_decode;
	std::string encode;
	std::string decode;
public:
	struct compare
	{
		bool operator()(huffman_tree_node* left, huffman_tree_node* right)
		{
			return (left->freq > right->freq);
		}
	};

	void HuffmanCodes(std::string data, int freq[]);					// Кодирование
	std::string decode_text(std::string str);								// Раскодирование через дерево
	void printCodes(huffman_tree_node* root, std::string str);
};

void huffman_coding::printCodes(huffman_tree_node* root, std::string str)
{
	if (!root)
	{
		return;
	}
	if (root->data != '$')
	{
		replace_abc.insert(std::pair <char, std::string>(root->data, str));
		replace_abc_decode.insert(std::pair <std::string, char>(str, root->data));
	}

	printCodes(root->left, str + "0");
	printCodes(root->right, str + "1");
} //01101110 10001010 11011100

std::string huffman_coding::decode_text(std::string str)
{
	int main_counter = 0;
	decode = "";
	for (size_t i = main_counter; i < str.size();)
	{
		pointer_huffman temp = root_main;
		while (true)
		{
			if (str[i] == '0')
			{
				if (temp->left != NULL)
				{
					temp = temp->left;
				}
				else
				{
					decode += temp->data;
					break;
				}
			}
			else
			{
				if (temp->right != NULL)
				{
					temp = temp->right;
				}
				else
				{
					decode += temp->data;
					break;
				}
			}
			i++;
			main_counter = i;
		}
		
		i = main_counter;
	}
	return decode;
}

void huffman_coding::HuffmanCodes(std::string data, int freq[])
{
	pointer_huffman left, right, top;

	// Создаем узлы с данными и частотой (учитывая compare функцию сравнения)
	std::priority_queue<pointer_huffman, std::vector<huffman_tree_node*>, compare> min_heap;
	for (size_t i = 0; i < data.size(); ++i)
	{
		pointer_huffman temp = new huffman_tree_node(data[i], freq[i]);
		min_heap.push(temp);		
	}

	// Пока размер очереди не дойдёт до единицы
	while (min_heap.size() != 1)
	{
		// Достаем два минимальных по частоте элемента из дерева
		left = min_heap.top();
		min_heap.pop();

		right = min_heap.top();
		min_heap.pop();

		// Создаем новый узел с частотой, равной сумме двух 
		// частот узлов c наименьшей частотой

		top = new huffman_tree_node('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		min_heap.push(top);
	}

	// Выводим на экран код Хаффмана
	root_main = min_heap.top();
	
	printCodes(root_main, "");
}

void HACompression(const std::string &inputFileName, const std::string &outputFileName)
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

	std::map <char, int> freq;
	char c;
	while (fin >> c)
		freq[c]++;

	fin.close();
	fin.open(inputFileName, std::ios::in);
	fin << std::noskipws;

	std::string data;
	int freq_data[freq.size()];
	for (auto it = freq.begin(); it != freq.end(); it++)
	{
		data += it->first;
		freq_data[distance(freq.begin(), it)] = it->second;
	}

	huffman_coding huff;
	huff.HuffmanCodes(data, freq_data);

	fout << char(data.size());
	for (auto it = freq.begin(); it != freq.end(); it++)
	{
		fout << it->first;
		char *temp = intToByte(it->second);
		for (int i = 0; i < 4; i++)
			fout << temp[i];
	}
	
	char buffer = '\000';
	int counter = 0;
	while (fin >> c)
	{
		for (size_t i = 0; i < huff.replace_abc[c].size(); i++)
		{
			if (huff.replace_abc[c][i] == '1')
				buffer |= (1 << (7 - counter));
			counter++;
			if (counter == 8)
			{
				fout << buffer;
				counter = 0;
				buffer = '\000';
			}
		}
	}
	if (counter != 0)
		fout << buffer;
	
	fin.close();
	fout.close();
}

void HADecompression(const std::string &inputFileName, const std::string &outputFileName)
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

	unsigned char size;
	fin >> size;
	std::string data(int(size), ' ');
	int freq[int(size)];
	for (int i = 0; i < int(size); i++)
	{
		char c;
		fin >> c;
		data[i] = c;

		char *buffer = new char[4];
		for (int i = 0; i < 4; i++)
			fin >> buffer[i];
		freq[i] = byteToInt(buffer);
	}


	huffman_coding huff;
	huff.HuffmanCodes(data, freq);

	char c;
	std::string str;
	while (fin >> c)
	{
		for (int i = 0; i < 8; i++)
		{
			if (c & (1 << (7 - i)))
				str += '1';
			else
				str += '0';
		
			if (huff.replace_abc_decode.find(str) != huff.replace_abc_decode.end())
			{
				fout << huff.replace_abc_decode[str];
				str = "";
			}
		}	
	}

	fin.close();
	fout.close();
}