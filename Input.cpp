/*------------------------------------------------------------------------------
Name: Input.cpp
Description: Definition of the Input class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <map>

#include "Input.h"

using namespace std;
void Input::Store_Input(Line_Storage &readLine)
{
  readLine.storedLines.push_back("");
  string key = "descriptor";

  for (int i = 0; i < readLine.inputLines.size(); i++)
  {
    map <string, string> temp = readLine.inputLines[i];
    string fullLine = string(temp[key]);
    int word = 1;

    for (int j = 0; j < fullLine.length(); j++) {
      readLine.Set_Char(i, word, j, fullLine[j]);

      if (fullLine[j] == ' ') {
        word++;
      }
    }

    readLine.numWords.push_back(word);

    if (i != readLine.inputLines.size() - 1) {
      readLine.storedLines.push_back("");
    }
  }
}

void Input::Read_File(Line_Storage &readLine)
{
  ifstream file(fileName);

  if (!file.is_open())
  {
    cout << "Error opening the file!" << endl;
    exit(1);
  }

  string line;

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

    map <string, string> temp;

    temp.insert({"url", line_parts[0]});
    temp.insert({"descriptor", line_parts[1]});
    temp.insert({"last_vist", line_parts[2]});
    temp.insert({"visits_number", line_parts[3]});

    readLine.inputLines.push_back(temp);
  }

  file.close();

  Input::Store_Input(readLine);
}
