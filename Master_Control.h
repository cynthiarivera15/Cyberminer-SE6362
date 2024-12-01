/*------------------------------------------------------------------------------
Name: KWIC_Master_Control.h
Description: Declaration of the class Master_Control
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <httplib.h>

#include "Input.h"
#include "Line_Storage.h"
#include "Circular_Shift.h"
#include "Alphabetizer.h"
#include "Output.h"

using namespace std;

class Master_Control
{
  public:
    string outputFileName;
    string inputFileName;

    // Create objects of all the classes that Master_Control need to invoke
    Input inputFile;
    Line_Storage readLine;
    Circular_Shift shiftedLine;
    Alphabetizer alphabetizedLine;
    Output outputFile;

    void Call_Input(); // Calls Read_File in Input

    void Call_Circular_Shift(); // Calls Setup in Circular_Shift

    void Call_Alphabetizer(); // Calls Alpha in Alphabetizer

    void Call_Output(ofstream &oFile); // Calls Display_Lines in Output

    bool URL_Verification(string input_search, httplib::Response &res); // Checks whether the URLs input by the user meet the necessary conditions

    void Save_Input(string input_search, httplib::Response &res); // Saves the URL in the input file if it meets the conditions
};
