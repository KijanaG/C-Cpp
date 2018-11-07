/*
* test.cpp
* Created on: October 27, 2018
*   Author: KJ Garrett
*
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "Trie.h"

using namespace std;

int main()
{
    //Create infile & outfile stream for test driver
    string query = "query.txt";
    ifstream infile(query);
    string line;
    string output = "test.txt";
    ofstream out(output);

    //Instantiate Trie
    Trie *test_t = new Trie();
    
    //Declare sequences for Grep
    // string hello = "AAAAA";
    // string goodbye = "TTTTT";
    string happy = "ATTATA";
    string gatsby = "ATATAT";
    string good = "CGCGCGC";
    string great = "CGCAGAG";
    string hello_a = "ATCAAAA";
    // string hello_b = "AAGA";
    string hello_c = "AT*";
    
    //Open ofstream
    if(out.is_open())
    {
        //Query, Insert, Remove, Print
        // cout << "\n\nQuerying " << hello_a << endl;
        // test_t->test_query(hello_a, out);
        // cout << "Querying " << hello_b << endl;
        // test_t->test_query(hello_b, out);

        cout << "Inserting " << happy << endl;
        test_t->test_insert(happy);
        cout << "Inserting " << gatsby << endl;
        test_t->test_insert(gatsby);
        cout << "Inserting " << good << endl;
        test_t->test_insert(good);
        cout << "Inserting " << great << endl;
        test_t->test_insert(great);

        // cout << "Querying " << gatsby << endl;
        // test_t->test_query(gatsby, out);
        // cout << "Querying " << happy << endl;
        // test_t->test_query(happy, out);

        cout << "Inserting " << hello_a << endl;
        test_t->test_insert(hello_a);

        test_t->print();
        cout << "\n\n*****************\n\n";
        cout << "Removing " << hello_c << endl;
        test_t->test_remove(hello_c);
        cout << "\n\n*****************\n\n";
        test_t->print();

        // cout << "Removing " << happy << endl;
        // test_t->test_remove(happy);

        // cout << "Inserting " << gatsby << endl;
        // test_t->test_insert(gatsby);

        // cout << "Removing " << hello_c << endl;
        // test_t->test_remove(hello_c);

        // test_t->print();

        out.close();
    }
    
    //Check if contains function works
    // test_t->contain_sequence(hello);
    // test_t->contain_sequence(good);
    // test_t->contain_sequence("ATTACGTCA");
    
    //Print all nodes left
    // test_t->print();
    return 0;
}
