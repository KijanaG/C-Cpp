/*
* main.cpp
* Created on: October 24, 2018
*   Author: KJ Garrett
*
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include "Trie.h"

using namespace std;

int main(int argc, char const *argv[]) //Where argc is count of arguments 
{                                // & argv is the array of arguments

    if(argc != 3)
    {   //Validate that required parameters were passed 
        //in as arguments via command line
        cout << "ERROR: Function takes two 2 parameters: \n";
        cout << "1 – The query file\n";
        cout << "2 – The output location\n";
        cout << "Execute program with ./SeqMatch\n";
    }
    else
    {
        //Define & assign execution parameters
        string query_file = argv[1];
        string output_location = argv[2];

        //Create stream class to read lines from query file
        std::ifstream infile(query_file);
        std::string line;
        
        //Write to file specified in output parameter
        std::ofstream out(output_location);

        //Check if outfile is open
        if(out.is_open()) 
        {   
            //Create instantiation of Trie class and call parsing operation
            Trie *dna_sequencer = new Trie();

            //Iterate through infile & parse string in Trie.cpp
            while(std::getline(infile, line))
            {
                cout << line << endl;
                dna_sequencer->parse_string(line, out);
            }
            out.close();
        }
        else 
        {
            cout << "Unable to create & open db handler." << endl;
        }
    }
    return 0;
}
