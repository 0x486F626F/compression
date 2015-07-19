#ifndef __SUFFIX_TRIE__
#define __SUFFIX_TRIE__

#include <string>
#include <queue>
#include <set>

struct word_counter {
	unsigned int freq;
	std::string word;
	word_counter(const unsigned int &freq, const std::string &word);
	bool operator < (const word_counter &other) const;
};

struct trie_node {
	unsigned int count;
	trie_node* child[27];
	trie_node();
	~trie_node();
};

class trie {
	private:
		trie_node* root;
		void _insert(trie_node* &current, std::string s);
		std::set <word_counter> _match_words(std::string s, std::queue <unsigned int> dontcare, bool usedLast);
	public:
		trie();
		~trie();
		void insert(const std::string &s);
		bool approx_match(std::string s, const unsigned int max_mismatch);
		int get_rank(std::string s, std::queue <unsigned int> revealed, bool usedLast = false);
		std::string get_word(std::string s, const std::queue <unsigned int> revealed, unsigned int rank, bool usedLast = false);
};


#endif
