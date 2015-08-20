CXX = g++ -fopenmp -O2
main: main.o create_suffix.o suffix_trie.o wordclass.o mtf.o encode.o decode.o
	g++ -fopenmp -O2 main.o create_suffix.o suffix_trie.o wordclass.o mtf.o encode.o decode.o -o main

clean:
	rm *.o main

zip:
	zip compression Makefile main.cc \
		create_suffix.cc create_suffix.h \
		suffix_trie.cc suffix_trie.h \
		wordclass.cc wordclass.h \
		decode.cc decode.h \
		encode.cc encode.h \
		mtf.cc mtf.h 
