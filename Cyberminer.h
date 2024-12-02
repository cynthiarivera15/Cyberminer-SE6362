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

    void Merge(vector<pair<string,string> > &databaseContent, int left, int mid, int right); // Reorganizes selected URLs into alphabetical asceding order based on the descriptor in the database

    void MergeSort(vector<pair<string,string> > &databaseContent, int left, int right); // Divides the list of URLs into smaller lists so they can be alphabetized

  public:
    Master_Control kwic;
    string inputFileName;
    string outputFileName;
    string search;

    bool Input_Verification(string input_search); // Checks whether the input matches the descriptor conditions

    void Call_KWIC(); // Invokes the KWIC System

    void Search_Specification(string input_search, httplib::Response &res); // Searches for the keyword-based search

    void URL_Deletion(httplib::Response &res); // Deletes from the input file those hyperlinks that have not been accessed in the last year

    vector<string> Sensitive_Search(httplib::Response &res); // Looks through all the shifted alphabetized lines and return the hyperlinks of those that match the user request

    vector<int> Results_Settings(vector<string> urls, httplib::Request req); // Lets the user decide the amount of results shown per page

    vector<string> Listing_Preference(vector<string> urls, httplib::Response &res, httplib::Request req); // Lets the user decide the preference on which order to display the URLs

    void Hyperlink_Enforcement(string url, httplib::Response &res); // Updates the date and visit times of an entry in the input file when its URL is clicked
};
