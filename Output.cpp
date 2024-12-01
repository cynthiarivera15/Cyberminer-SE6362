/*------------------------------------------------------------------------------
Name: Output.cpp
Description: Definition of the Output class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Output.h"

using namespace std;

void Output::Print(Alphabetizer alphabetizedLine, ofstream &oFile, string iFile)
{
  ifstream file(iFile);

  if (!file.is_open())
  {
    cout << "Error opening the file!" << endl;
    exit(1);
  }

  string line;
  vector<string> urls_list;

  while (getline(file, line))
  {
    stringstream splitLine(line);
    string token;
    vector<string> line_parts;

    while (getline(splitLine, token, '$'))
    {
      token.erase(0, token.find_first_not_of(" "));
      token.erase(token.find_last_not_of(" ") + 1);
      line_parts.push_back(token);
    }

    urls_list.push_back(line_parts[0]);
  }

  file.close();

  int numWord;
  char p;

  for (int i = 0; i < alphabetizedLine.alphabetizedLines.size(); i++)
  {
    vector<int> wordLengths;
    string newLine = "";
    numWord = alphabetizedLine.Get_Word(i);

    stringstream splitLine(alphabetizedLine.alphabetizedLines[i]);
    string token = " ";
    vector<string> words;

    while (splitLine >> token)
    {
      words.push_back(token.c_str());
    }

    for (int j = 0; j < words.size(); j++)
    {
      wordLengths.push_back(words[j].length());
    }

    for (int j = 0; j < numWord; j++)
    {
      int wl = 1;

      while (wl <= wordLengths[j])
      {
        p = alphabetizedLine.Get_Char(i, j, wl);
        newLine += p;

        wl++;
      }

      if (j != numWord - 1)
      {
        newLine += " ";
      }
    }

    int url_loc = alphabetizedLine.matchURL[i];
    string url = urls_list[url_loc];

    oFile << newLine << ": " << url << endl;
  }
}

void Output::Display_Lines(Alphabetizer alphabetizedLine, ofstream &oFile, string iFile)
{
  Output::Print(alphabetizedLine, oFile, iFile);
}
