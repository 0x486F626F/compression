//============================================================================
// Name        : dictionary.cpp
// Author      : Daniela Maftuleac
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/* A dictionary is represented as a trie and an array.
 * The trie is used for storing and searching of the strings of the dictionary and the array is used for the indexes of the strings.
 * Each leaf of the trie contains a pointer to an element in the array and vice-versa, an element of the array contains a pointer
 * to a leaf of the array.
 * The dictionary is initiated with 128 ASCII characters.
 * We will consider the dictionary with 2^32 entries, composed of 3 parts:
 * * the first 2^6 strings will be encoded using 6 bits
 * * from 2^6 to 2^16 strings will be encoded using 16 bits
 * * from 2^16 to 2^32 srings will be encoded using 32 bits
 *
 * Given a huge text file, the program creates the global dictionary using LZW algorithm and each time it finds a string that is
 * already contained in the dictionary, it increases its frequency and it gets moved to front.
 *
 * Encoding:
 * Once global dictionary is created, given a text file, we want to encode it as follows:
 * Reads the longest substring present in the global dictionary and then continues checking if it is present in the local dictionary.
 * If not found in local dictionary, it outputs the code of the string(from glocal or local) then inserts the longest substring
 * found (in global or local dictionary) concatenated with next character read from the file.
 * Each time a substring is found (in local or global) it gets moved to front.
 * The encoded form of an index from dictionary will be:
 * b1b2b3b4....b35  (max length 35 bits)
 * * the first bit b1 indicates the dictionary: 0 - global, 1 - local
 * * the second bit b2 (and third bit b3) indicates the number of bits used for encoding:
 * * * if b2 = 0, the index is encoded using 6 bits
 * * * if b2 = 1, and b3 = 0, the index is encoded using 16 bits
 * * * if b2 = 1, and b3 = 1, the index is encoded using 32 bits
 * * * the bits b4 - b35 represent the binary encoding of the index
 *
 *
 * Decoding:
 * Given a global dictionary and local dictionary containing only 128 ASCII characters,
 * REPEAT:
 * * read the first bit and determines the dictionary
 * * read the second (and third bit if the second is 1) and determines the number of bits used for the encoding
 * * decodes and outputs the string from respective dictionary and does move to front of the entry
 * * inserts in the local dictionary previous decoded string + the first character of the current decoded string (same as in classic LZW)
 */


#include <iostream>
#include "lzw-global-local.h"

void print(Trie* trie, int& level)
{
	if(level > 1)
	{
		if(level == 2)
		{
			for(int i = 0; i < level-2; i++)
				std::cout << ".";
			std::cout << trie->parent->character << std::endl;
		}
		for(int i = 0; i < level-1; i++)
			std::cout << ".";
		std::cout << trie->character << std::endl;
	}
	level++;
	for(int i=0; i<trie->children.size(); i++)
	{
		Trie* t = trie->children[i].get();
		if(t)
		{
			print(t, level);
		}
	}
	level--;
}
void print(Trie* trie)
{
	int level = 0;
	print(trie, level);
}
void print(Dictionary* dict)
{
	int level = 0;
	print(dict->trie.get(), level);
}

void Test_Insert()
{
	std::shared_ptr<Dictionary> global(new Dictionary());
	global->insert("Hello");
	global->insert("He87jsa");
	global->insert("dasdas");
	global->insert("Kmdsak");
	global->insert("sklj");

	int index = global->return_index("Kmdsak");
	std::cout << "Test Insert: ";
	if(index == 131)
	{
		std::cout << "OK" << std::endl;
	}else
	{
		std::cout << "FAILED" << std::endl;
	}
}

void Test_Inexistent()
{
	std::shared_ptr<Dictionary> global(new Dictionary());
	global->insert("Hello");
	global->insert("He87jsa");
	global->insert("dasdas");
	global->insert("Kmdsak");
	global->insert("sklj");

	int index = global->return_index("768t678342");
	std::cout << "Test Inexistent: ";
	if(index == -1)
	{
		std::cout << "OK" << std::endl;
	}else
	{
		std::cout << "FAILED" << std::endl;
	}
}
void Test_Search()
{
	std::shared_ptr<Dictionary> global(new Dictionary());
	global->insert("Hello");
	global->insert("He87jsa");
	global->insert("dasdas");
	global->insert("Kmdsak");
	global->insert("sklj");

	std::string str = global->return_string(131);
	std::cout << "Test Search: ";
	if(str == "Kmdsak")
	{
		std::cout << "OK" << std::endl;
	}else
	{
		std::cout << "FAILED" << std::endl;
	}

//	Trie* t = global->trie->children['K'].get();
//	print(t);
}

void Test_Swap()
{
	std::shared_ptr<Dictionary> global(new Dictionary());
	global->insert("Hello");
	global->insert("He87jsa");
	global->insert("dasdas");
	global->insert("Kmdsak");
	global->insert("sklj");

	int index1 = global->return_index("Kmdsak");
	int index2 = global->return_index("a");

	global->swap(index1, index2);
	std::string str = global->return_string(index1);

	std::cout << "Test Swap: ";
		if(str == "a")
		{
			std::cout << "OK" << std::endl;
		}else
		{
			std::cout << "FAILED" << std::endl;
		}

}

int main(){
	/*
	 * Tests:
	 */
	//Test_Insert();
	//Test_Inexistent();
	//Test_Search();
	//Test_Swap();

	Dictionary* global = makeGlobal();

	//encode_text(global, "input-compress.txt");
	decode_text(global);

	//print(global);
	return 0;
}
