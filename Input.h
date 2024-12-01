/*------------------------------------------------------------------------------
Name: Input.h
Description: Declaration of the class Input
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

class Input {
  private:
    void Store_Input(Line_Storage &readLine); // Divides the input and calls the function Set_Char from the class Line_Storage

  public:
    string fileName;

    void Read_File(Line_Storage &readLine); // Reads the file that was input by the user
};
