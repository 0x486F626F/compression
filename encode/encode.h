#ifndef __ENCODE_H__
#define __ENCODE_H__
#include <string>
#include "suffix_trie.h"

std::string bestCompression (std::string text, trie * GlobalSuffixTrie);

#endif
