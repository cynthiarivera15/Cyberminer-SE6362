/*------------------------------------------------------------------------------
Name: Cyberminer.h
Description: Declaration of the class Cyberminer
Course: SE6362
------------------------------------------------------------------------------*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Master_Control.h"

using namespace std;

class Cyberminer
{
  private:
    pair<bool, string> keyword_search;

  public:
    Master_Control kwic;
    string inputFileName;
    string search;

    bool Input_Verification(string input_search); // Checks whether the input matches the descriptor conditions

    void Call_KWIC(); // Invokes the KWIC System

    void Search_Specification(string input_search, httplib::Response &res); // Searches for the keyword-based search

    void URL_Deletion(httplib::Response &res); // Deletes from the input file those hyperlinks that have not been accessed in the last year

    

    //void Hyperlink_Enforcement();
    //void Listing_Preference();
    //void Results_Settings();
    //void Autofill();
};
