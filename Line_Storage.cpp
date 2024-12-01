/*------------------------------------------------------------------------------
Name: Line_Storage.cpp
Description: Definition of the Line_Storage class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Line_Storage.h"

using namespace std;

void Line_Storage::Set_Char(int line, int word, int chrc, char p) {
  Line_Storage::storedLines[line] += p;
}

char Line_Storage::Get_Char(int line, int word, int chrc) {
  char p;
  string token = " ";
  vector<string> words;
  stringstream splitLine(Line_Storage::storedLines[line]);

  while (splitLine >> token) {
    words.push_back(token.c_str());
  }

  p = words[word][chrc - 1];

  return p;
}

int Line_Storage::Get_Word(int line) {
  return Line_Storage::numWords[line];
}
