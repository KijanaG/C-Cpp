/*
* Trie.cpp
* Project 1 : KJ Garrett
* October 31, 2018
*
*/

#include <iostream>
#include <fstream>
#include <iomanip>

#include "Trie.h"

using namespace std;


Trie::Trie()
{
    root = new TrieNode();
    line_count = 0;
    end_of_word = 0;
}

Trie::~Trie() 
{
    remove_all_after(root);
    delete root;
    root = NULL;
}


// Function: parse_string
// Input: line as string from query file & output location
// Returns: void
// Does: Parses string to get i/r/q and runs one of three
// main functions to perform grep on dna sequence
void Trie::parse_string(string &line, ofstream &out)
{ 
    line_count++;
    string delimeter = " ";
    int pos = 0;
    pos = line.find(delimeter);
    string command = line.substr(0, pos);
    string dna = line.substr(pos+1,line.length());

    if(command == "r") {
        if(contains(dna, root)){
            remove(dna, root);
            if(exists(dna)) {
                out << "Removed all sequences starting with " 
                    << dna << endl;
            } else {
                out << dna << " removed" << endl;
            }
        } else {
            out << dna << " not found" << endl;
        }
    } else if(command == "q") {
        query(dna, 0, dna, "", root, &out);
    } else if(command == "i") {
        insert(dna, root);
    } else {
        cerr << "Cannot read \'" << command << "\' in as a command."
        << " Line ~ " << line_count << "." << endl;
    }
}

// Function: insert
// Input: Takes in the dna sequence as a string
// Returns: void
// Does: Creates a new node if the letter in the sequence
// does not exist, otherwise recurses through Trie until 
// a letter needs to be added to the Trie and sets bool to true
void Trie::insert(string dna, TrieNode *node) 
{
    if(dna.length() == 0) {
        node->is_end = true;
        return;
    }

    int pos = get_pos_letter(dna[0]);

    if(pos < 0) {
        cerr << "Cannot insert character – " << dna[0] << endl;
        return;
    }

    if(node->children[pos] != NULL) {
        insert(remove_first_char(dna), node->children[pos]);
    } else {
        TrieNode* next = parametrize(dna[0]);
        node->children[pos] = next;
        if(dna.length() == 1) {
            next->is_end = true;
            end_of_word++;
            return;
        }
        insert(remove_first_char(dna), next);
    }
}

// Function: remove
// Input: Takes in the dna sequence as a string & node pointer
// Returns: bool
// Does: Recursively iterates through function until reaching
// the base case & deletes last letter or is_end bool. Deletes nodes 
// in previous functions until another sequence uses that node then breaks
bool Trie::remove(string dna, TrieNode *node) 
{
    int pos = get_pos_letter(dna[0]);
    if(dna[0] == '*') {
        remove_all_after(node);
        return true;
    }
    if(node->children[pos]->is_end && dna.length() == 1) {
        if(has_children(node->children[pos])) {
            node->children[pos]->is_end = false;
            return true;
        } else {
            delete node->children[pos];
            node->children[pos] = NULL;
            return true;
        }
    } else {
        if(remove(remove_first_char(dna), node->children[pos])){
            if(!node->children[pos]->is_end && 
               !has_children(node->children[pos])) {
                delete node->children[pos];
                node->children[pos] = NULL;
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}

// Function: query
// Input: DNA string, common prefix count, const DNA string, 
// Trie sequence, TrieNode pointer
// Returns: void
// Does: Finds whether a letter, currently in the sequence, is in the Trie.
// Determines the resemblance of the inputed string and the closest matching
// sequence in the Trie by recursively moving through the data structure
void Trie::query(string dna, int common, string original, string sequence, 
                 TrieNode *node, ofstream *out) 
{
    bool match = false;
    int pos = get_pos_letter(dna[0]);
    TrieNode *current = node->children[pos];
    if(current != NULL) {
        match = true;
        common++;
    }
    if(node == root && !match) {
        *out << "No Match Found" << endl;
        return;
    }
    if(dna[1] == '*') {
        sequence += node->children[pos]->letter;
        sequence = find_remaining_seq(node->children[pos], sequence);
        float match_p = ((float) common/sequence.length()*100);
        *out << "Match Found " << sequence << " " << setprecision(4) 
             << match_p << "% match" << endl;
        return;
    }
    if(match) {
        sequence += current->letter;
        if(dna.length() > 1) {
            query(remove_first_char(dna), common, original,
            sequence, current, out);
        } else if(dna.length() == 1 && current->is_end) {
            *out << "Match Found " << sequence << " 100%" << endl;
            return;
        } else {
            sequence = find_remaining_seq(current, sequence);
            float match_p = ((float) common/sequence.length()*100);
            *out << "Match Found " << sequence << " " << setprecision(4) 
                 << match_p << "% match" << endl;
            return;
        }
    } else {
        for(int i=0;i<5;i++) {
            if(node->children[i] != NULL) {
                sequence = find_remaining_seq(node->children[i], sequence);
                break;
            }
        }
        float match_p;
        if(original.length() > sequence.length()) 
            match_p = (float) common/original.length() *100;
        else 
            match_p = (float) common/sequence.length() *100;
        *out << "Match Found " << sequence << " " << setprecision(4) 
             << match_p << "% match" << endl;
        return;
    }
}

// Function: get_pos_letter
// Input: Takes in a character
// Returns: integer
// Does: Helper function (for insert) to get first character in string
int Trie::get_pos_letter(char letter) const 
{
    if(letter == 'A'){
        return 0;
    } else if(letter == 'C'){
        return 1;
    } else if(letter == 'G'){
        return 2;
    } else if(letter == 'T'){
        return 3;
    } else if(letter == '?'){
        return 4;
    } else {
        return -1;
    }
}

// Function: parametrize
// Input: Takes in a character
// Returns: TrieNode
// Does: Helper function that returns a pointer to
// a Trienode with assigned attributes
TrieNode* Trie::parametrize(char letter) 
{
    TrieNode *node = new TrieNode();
    node->letter = letter;
    node->is_end = false;
    for(int i=0;i<5;i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Function: remove_first_char
// Input: string of dna sequence
// Returns: string
// Does: removes the first letter in a given string
string Trie::remove_first_char(string line) 
{
    string new_line = "";
    int num = line.length();
    for(int i=1;i<num;i++) {
        new_line += line[i];
    }
    return new_line;
}

// Function: find_remaining_seq
// Input: TrieNode & dna sequence
// Returns: string with the remaining dna sequence 
// Does: Helper for query function that returns the next
// closest sequence to the one being queried for
string Trie::find_remaining_seq(TrieNode *node, string sequence) 
{
    sequence += node->letter;
    if(node->is_end) {
        return sequence;
    }
    for(int i=0;i<5;i++) {
        if(node->children[i] != NULL) {
            return find_remaining_seq(node->children[i], sequence); 
        }
    }
    return sequence;
}

// Function: contains
// Input: TrieNode & dna sequence
// Returns: boolean
// Does: checks if dna sequence is in Trie before calling remove
bool Trie::contains(string dna, TrieNode *node) 
{
    if(dna[0] == '*') return true;
    int pos = get_pos_letter(dna[0]);
    if(dna.length() == 1) {
        if(node->children[pos]->is_end) return true;
        else return false;
    }
    if(node->children[pos] != NULL) {
        return contains(remove_first_char(dna), node->children[pos]);
    } else {
        return false;
    } 
}

// Function: has_children
// Input: TrieNode
// Returns: boolean
// Does: Checks if TrieNode has any children before 
// removing it from the Trie
bool Trie::has_children(TrieNode *node) 
{
    for(int i=0;i<5;i++) {
        if(node->children[i] != NULL){
            return true;
        }
    }
    return false;
}

// Function: remove_all_after
// Input: TrieNode
// Returns: void
// Does: recursively deletes all nodes after the
// the one passed in primarily
void Trie::remove_all_after(TrieNode *node)
{
    bool all_null = false;
    for(int i=0;i<5;i++) {
        if(node->children[i]) {
            all_null = true;
            remove_all_after(node->children[i]);
            delete node->children[i];
            node->children[i] = NULL;
        }
    }
    if(!all_null){
        return;
    }
}

// Function: exists
// Input: dna string
// Returns: bool
// Does: Checks if there is an asterisk in the string
bool Trie::exists(string dna)
{
    for(int i=0; i<dna.length();i++) {
        if(dna[i] == '*') {
            return true;
        }
    }
    return false;
}

/*******************************************************************/
/************* _____________TEST FUNCTIONS____________ *************/
/*******************************************************************/

// Function: test_insert
// Input: String 
// Returns: void
// Does: Calls insert function with test string & root
void Trie::test_insert(string &line) 
{
    insert(line, root);
}

// Function: test_query
// Input: String 
// Returns: void
// Does: Calls query function with all parameters
void Trie::test_query(string &line, ofstream &out) 
{
    query(line, 0, line, "", root, &out);
}

// Function: test_remove
// Input: String 
// Returns: void
// Does: Calls remove function with test string & root
void Trie::test_remove(string &line) 
{
    if(contains(line, root)) {
        remove(line, root);
        end_of_word--;
    } else {
        cout << line << " not found";
    }
}

// Function: print
// Input: nothing
// Returns: void
// Does: Calls private print function & passes it root
void Trie::print() const 
{
    print_all_nodes(root);
    cout << "Word count: " << end_of_word << endl;
}

// Function: print_all_nodes
// Input: nothing
// Returns: void
// Does: Prints all nodes in Trie for testing purposes
void Trie::print_all_nodes(TrieNode *node) const 
{
    for(int i=0;i<5;i++) {
        if(node->children[i]) {
            cout << node->children[i]->letter << endl;
            print_all_nodes(node->children[i]);
        }
    }
    return;
}

// void Trie::print_all(string sequence, TrieNode *node, ofstream *out) 
// {
//     for(int i=0;i<5;i++) {
//         string temp = sequence;
//         if(node->children[i]) {
//             temp += node->children[i]->letter;
//             if(node->children[i]->is_end) {
//                 *out << temp << " removed" << endl;
//                 if(has_children(node->children[i])) {
//                     print_all(temp, node->children[i], out);
//                 }
//             }
//         }
//     }
//     return;
// }


// Function: contain_sequence
// Input: dna string
// Returns: void
// Does: Prints to console whether a string is contained 
// within the Trie or not
void Trie::contain_sequence(string dna) 
{
    if(contains(dna, root)){
        cout << "Contains == TRUE <> " << dna << endl;
    } else {
        cout << "Contains == FALSE <> " << dna << endl;
    }
}
