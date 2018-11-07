#
# Makefile for Trie 
#
# Project 1 : KJ Garrett
#

CXX      = clang++
CXXFLAGS = -g3 -std=c++11 -Wall -Wextra
LDFLAGS  = -g3

SeqMatch: main.o Trie.o
	${CXX} ${LDFLAGS} -o SeqMatch main.o Trie.o

main.o:  main.cpp Trie.h
Trie.o:  Trie.cpp Trie.h

test: test.o Trie.o
	${CXX} ${LDFLAGS} -o test test.o Trie.o

test.o: test.cpp Trie.h
Trie.o: Trie.cpp Trie.h

clean:
	rm -rf SeqMatch test *.o *.dSYMs