#include "suffix_trie.h"

#include <iostream>
#include <queue>

int main() {
	using std::cin;
	using std::cout;
	using std::endl;
	using std::string;
	using std::queue;
	
	trie t;
	t.insert("the");
	t.insert("the");
	t.insert("tee");
	t.insert("bth");
	t.insert("bte");
	queue <unsigned int> q;
	q.push(0), q.push(1);
	cout << t.get_rank("the", q) << endl;
	cout << t.get_word("__e", q, 0) << endl;
	return 0;
}
