/*
* Trie.h
*
* Created On October 24th, 2018
*/

#ifndef TRIE_H_
#define TRIE_H_

#include <iostream>

using namespace std;

/*
* Trie Node:
* - letter in the dna sequence
* - children array of pointers to child nodes
* - is_end of word boolean
*/

struct TrieNode {
char letter;
TrieNode* children[5];
bool is_end;
};

class Trie {

public:
        // constructor
        Trie();

        // Destructor
        ~Trie();

        // Public function that parses the string and passes the dna sequence
        // to either the remove/query/insert functions
        void parse_string(string &line, ofstream &out);

        // Test functions that call helper functions and main functions
        // of program to check the integrity of the data throughout the
        // various function calls that take place
        void print() const;
        void test_insert(string &line);
        void test_query(string &line, ofstream &out);
        void test_remove(string &line);
        void contain_sequence(string dna);

private:
    // Pointer to root node of Trie
    TrieNode *root;

    // Keep count of lines in input file for error message
    int line_count;

    // insert() takes in the dna sequence and a TrieNode (intially the root)
    // it checks if the letter that it wants to insert is already there,
    // and if not it will insert a TrieNode with the letter and recurse down 
    // the tree to the next child, removing the beginning letter in the dna
    // sequence. If it is there, it will simply continue through the Trie
    // removing the first letter in the sequence before continuing. There is
    // a base case for when the sequence already exists (when dna.length()
    // == 0) and the function sets the node to true and returns. But when 
    // the sequence is unique, and the dna sequence has one letter left, 
    // the function creates the next TrieNode sets the is_end bool to true 
    // and returns.
    void insert(string dna, TrieNode *node);

    // remove() is only called if the dna sequence is in the Trie structure.
    // the function gets the position of the letter in the sequence, checks
    // the base case if the next child node is the end of the sequence and 
    // recurses through the Trie if not. The base case sets the next node 
    // to false or deletes the next node depending on whether it has children 
    // or not. It returns true when it does so, and then checks the previous
    // node from the recursive call to see whether to delete the node or not.
    // If not, the recursive function returns false, and ends. Prints whether
    // the sequence was removed or not in the database handler.
    bool remove(string dna, TrieNode *node);

    // query() goes forward similar to insert() and checks whether the next
    // letter in the sequence has a corresponding TrieNode. If there is one,
    // a bool match is set to true & # of common prefix is incremented by 1.
    // There's an initial check for no match and then a conditional with
    // match as a parameter is invoked. If there is a match it calls 
    // query() again until 1 character left (assuming a match), and then 
    // either finds the identical dna sequence or calls a helper to get the
    // rest of a sequence that shares the most common prefixes. If there is
    // no match, it will also call the helper function. It then prints out
    // to the database handler file whether or not a match was found (with %). 
    void query(string dna, int common_prefix, const string dna_const, 
               string sequence, TrieNode *node, ofstream *out);

    // Get position of letter in TrieNode array **Helper**
    int get_pos_letter(char letter) const;

    //Helper function that removes all sequences after given node **Helper**
    void remove_all_after(TrieNode *node);

    //Check if * is in dna sequence
    bool exists(string dna);

    // Return string without first character **Helper**
    string remove_first_char(string line);
    
    //Helper function to print all removed with *
    void print_all(string dna, TrieNode *node, ofstream *out);

    // Checks if Trie contains sequence, returns bool **Helper**
    bool contains(string dna, TrieNode *node);

    // Check if node has children **Helper**
    bool has_children(TrieNode *node);

    // Sets node's attributes **Helper**
    TrieNode* parametrize(char letter);

    // Return remaining sequence of dna with closest resemblance **Helper**
    string find_remaining_seq(TrieNode *node, string sequence);

    // Print all letters in Trie **Test**
    void print_all_nodes(TrieNode *node) const;

    // Count for # of words inputed **Test**
    int end_of_word;
};

#endif