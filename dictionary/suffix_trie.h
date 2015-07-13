#ifndef __SUFFIX_TRIE__
#define __SUFFIX_TRIE__

#include <string>
#include <queue>

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
	public:
		trie();
		~trie();
		void insert(const std::string &s);
		bool approx_match(std::string s, const unsigned int max_mismatch);
		int get_rank(std::string s, std::queue <unsigned int> revealed);
		std::string get_word(std::string s, const std::queue <unsigned int> revealed, unsigned int rank);
};


#endif
