/*------------------------------------------------------------------------------
Name: Line_Storage.h
Description: Declaration of the class Line_Storage
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class Line_Storage {
  public:
    vector<map <string, string> > inputLines;
    vector<string> storedLines;
    vector<int> numWords;

    void Set_Char(int line, int word, int chrc, char p); // Stores in a vector each line read character by character

    char Get_Char(int line, int word, int chrc); // Returns a character from a specific line and word

    int Get_Word(int line); // Returns the number of words a specific line has
};
