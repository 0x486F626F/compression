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
	t.insert("tbe");

	queue <unsigned int> q, p;
	q.push(1);
	cout << t.get_rank("the", q) << endl;
	return 0;
}
