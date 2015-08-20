#ifndef __SUFFIX_TRIE__
#define __SUFFIX_TRIE__

#include <string>
#include <queue>
#include <set>
#include <iostream>

struct word_counter {
	unsigned long long freq;
	std::string word;
	word_counter(const unsigned long long &freq, const std::string &word);
	bool operator < (const word_counter &other) const;
};

struct trie_node {
	unsigned long long count;
	trie_node* child[27];
	trie_node();
	~trie_node();
};

class trie {
	private:
		trie_node* root;
		void _insert(trie_node* &current, std::string s);
		std::set <word_counter> _match_words(std::string s, std::queue <unsigned int> dontcare, bool usedLast);
		void _traverse(std::ostream & out, trie_node* &current);
		void _load(std::istream & in, trie_node* &current);
	public:
		trie();
		~trie();
		void insert(const std::string &s);
		bool approx_match(std::string s, const unsigned int max_mismatch);
		long long get_rank(std::string s, std::queue <unsigned int> revealed, bool usedLast = false);
		std::string get_word(std::string s, const std::queue <unsigned int> revealed, unsigned long long rank, bool usedLast = false);
		void traverse_trie();
		void load();
};


#endif
