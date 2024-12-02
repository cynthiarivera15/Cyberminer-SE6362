/*------------------------------------------------------------------------------
Name: Circular_Shift.cpp
Description: Definition of the Circular_Shift class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Circular_Shift.h"

using namespace std;

void Circular_Shift::Set_Char(int shift, int word, int chrc, char p)
{
  Circular_Shift::shiftedLines[shift][word].push_back(p);
}

char Circular_Shift::Get_Char(int line, int shift, int word, int chrc)
{
  char p;
  string token = " ";
  vector<string> words;
  stringstream splitLine(Circular_Shift::shiftedLines[line][shift]);

  while (splitLine >> token)
  {
    words.push_back(token.c_str());
  }

  p = words[word][chrc];

  return p;
}

int Circular_Shift::Get_Word(int shift)
{
  return Circular_Shift::numWords[shift];
}

string Circular_Shift::Read_Line(Line_Storage readLine, int line) {
  int numWord;
  char p;

  vector<int> wordLengths;

  string newLine = "";

  numWord = readLine.Get_Word(line);

  Circular_Shift::numWords.push_back(numWord);

  Circular_Shift::shiftedLines.push_back(vector<string>());

  stringstream splitLine(readLine.storedLines[line]);
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
      p = readLine.Get_Char(line, j, wl);
      newLine += p;

      wl++;
    }

    if (j != numWord - 1)
    {
      newLine += " ";
    }
  }

  return newLine;
}

void Circular_Shift::Eliminate_Noise_Words(string &noiseWordLine)
{
  vector<string> noise_words = {"a", "an", "and", "at", "be", "but", "can", "in", "it", "is", "of", "or", "so", "that", "the", "then", "there", "they", "this", "was", "yet"};

  stringstream splitLine(noiseWordLine);
  string token = " ";
  vector<string> words;

  while (splitLine >> token)
  {
    words.push_back(token.c_str());
  }

  string temp = "";

  for (int i = 0; i < words[0].size(); i++)
  {
    temp += tolower(words[0][i]);
  }

  for (int i = 0; i < noise_words.size(); i++)
  {
    if (temp == noise_words[i])
    {
      noiseWordLine = "";

      words.erase(words.begin());

      for(int j = 0; j < words.size(); j++)
      {
        if (j != words.size() - 1)
        {
          noiseWordLine.append(words[j]);
          noiseWordLine.append(" ");
        }

        else
        {
          noiseWordLine.append(words[j]);
        }
      }

      stringstream splitLine2(noiseWordLine);

      while (splitLine2 >> token)
      {
        words.push_back(token.c_str());
      }

      temp = "";

      for (int j = 0; j < words[0].size(); j++)
      {
        temp += tolower(words[0][j]);
      }

      for (int j = 0; j < noise_words.size(); j++)
      {
        if (temp == noise_words[j])
        {
          Circular_Shift::Eliminate_Noise_Words(noiseWordLine);
          break;
        }
      }

      break;
    }
  }

}

void Circular_Shift::Generate_Circular_Shift(Line_Storage &readLine)
{
  int numWord;
  char p;

  for (int i = 0; i < readLine.storedLines.size(); i++)
  {
    string newLine = Circular_Shift::Read_Line(readLine, i);
    string noiseWordLine = newLine;
    cout << newLine << endl;
    numWord = readLine.Get_Word(i);

    stringstream splitLine(readLine.storedLines[i]);
    string token = " ";
    vector<string> words;
    vector<int> wordLengths;

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
      Circular_Shift::shiftedLines[i].push_back("");

      if (j == 0)
      {
        Circular_Shift::Eliminate_Noise_Words(noiseWordLine);

        for (int k = 0; k < noiseWordLine.length(); k++)
        {
          Circular_Shift::Set_Char(i, j, k, noiseWordLine[k]);
        }

        Circular_Shift::shiftedLines[i].push_back("");

        newLine += " ";

        int counter = 1;

        while (counter <= wordLengths[j])
        {
          p = newLine[0];
          newLine += p;

          newLine.erase(newLine.begin());

          if (counter == wordLengths[j])
          {
            newLine.erase(newLine.begin());
          }

          counter++;
        }

        noiseWordLine = newLine;
        Circular_Shift::Eliminate_Noise_Words(noiseWordLine);

        for (int k = 0; k < noiseWordLine.length(); k++)
        {
          Circular_Shift::Set_Char(i, j + 1, k, noiseWordLine[k]);
        }
      }

      else
      {
        newLine += " ";

        int counter = 1;

        while (counter <= wordLengths[j])
        {
          p = newLine[0];
          newLine += p;

          newLine.erase(newLine.begin());

          if (counter == wordLengths[j])
          {
            newLine.erase(newLine.begin());
          }

          counter++;
        }

        noiseWordLine = newLine;
        Circular_Shift::Eliminate_Noise_Words(noiseWordLine);

        if (j != numWord - 1 && j != numWord)
        {
          for (int k = 0; k < noiseWordLine.length(); k++)
          {
            Circular_Shift::Set_Char(i, j + 1, k, noiseWordLine[k]);
          }
        }
      }
    }

    Circular_Shift::shiftedLines[i].pop_back();
  }
}
