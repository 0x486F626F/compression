#ifndef __DECODE_H__
#define __DECODE_H__
#include <string>
#include <iostream>
#include "suffix_trie.h"

std::string decodeText(std::istringstream & in, trie * GlobalSuffixTrie);

#endif
