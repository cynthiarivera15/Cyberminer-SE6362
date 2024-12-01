/*------------------------------------------------------------------------------
Name: Circular_Shift.h
Description: Declaration of the class Circular_Shift
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Line_Storage.h"

using namespace std;

class Circular_Shift {
  private:
    void Set_Char(int shift, int word, int chrc, char p); // Stores in a vector each shifted line read character by character

    string Read_Line(Line_Storage readLine, int line); // Calls the function Get_Char & Get_Word from the Line_Storage class to get the lines

    void Eliminate_Noise_Words(string &noiseWordLine); // After the line has been it circularly shifted it eliminates the noise words it might have

  public:
    vector<vector<string> > shiftedLines;
    vector<int> numWords;

    char Get_Char(int line, int shift, int word, int chrc); // Returns a character from a specific shifted line and word

    int Get_Word(int shift); // Returns the number of words a specific shifted line has

    void Generate_Circular_Shift(Line_Storage &readLine); // Generate the circular shifts
};
