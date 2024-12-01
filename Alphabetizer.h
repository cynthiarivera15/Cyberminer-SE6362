/*------------------------------------------------------------------------------
Name: Alphabetizer.h
Description: Declaration of the class Alphabetizer
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Circular_Shift.h"

using namespace std;

class Alphabetizer {
  private:
    void Set_Char(int alpha, int word, int chrc, char p); // Stores in a vector each alphabetized line read character by character

    void Merge(vector<pair<string,int> > &shiftedLines, int left, int mid, int right);

    void MergeSort(vector<pair<string,int> > &shiftedLines, int left, int right);

    void Read_Line(Circular_Shift shiftedLine, vector<vector<pair<string,int> > >  &temp1, int line);

  public:
    vector<string> alphabetizedLines;
    vector<int> numWords;
    vector<int> matchURL;

    char Get_Char(int alpha, int word, int chrc); // Returns a character from a specific alphabetized line and word

    int Get_Word(int alpha); // Returns the number of words a specific alphabetized line has

    void Generate_Alpha(Circular_Shift &shiftedLine); // Calls the function Get_Char & Get_Word from the Circular Shifte class to get the shifted lines and alphabetized them
};
