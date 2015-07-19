#ifndef __ENCODE_H__
#define __ENCODE_H__
#include <string>
#include "suffix_trie.h"
#include "wordclass.h"

std::string bestCompression (std::string text, trie * GlobalSuffixTrie);

CompressedWords * tryAllLetters(std::string text, int normalLen, trie * GlobalSuffixTrie);

#endif
