#ifndef __MTF_H__
#define __MTF_H__

#include "encode.h"
#include <vector>
#include <string>
#include <utility>

class mtf {
	private:
		std::vector < std::pair<std::string, CompressedWords*> > dict;
	public:
		void insert(const std::string &word, CompressedWords * cp);
                void CPinsert(const std::string &word, CompressedWords * cp);
		unsigned int index(const std::string &word);
		std::string word(const unsigned int index);
		CompressedWords* findBest(const std::string &word);
		~mtf();
};

#endif
