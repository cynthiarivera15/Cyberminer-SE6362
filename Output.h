/*------------------------------------------------------------------------------
Name: Output.h
Description: Declaration of the class Display_Lines
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

#include "Alphabetizer.h"

using namespace std;

class Output {
  private:
    void Print(Alphabetizer alphabetizedLine, ofstream &oFile, string iFile);

  public:
    void Display_Lines(Alphabetizer alphabetizedLine, ofstream &oFile, string iFile);
};
