/*------------------------------------------------------------------------------
Name: Cyberminer.cpp
Description: Definition the Cyberminer class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <regex>
#include <httplib.h>
#include <ctime>

#include "Cyberminer.h"

using namespace std;

bool Cyberminer::Input_Verification(string input_search)
{
  regex descriptor("[a-zA-Z0-9][\\sa-zA-Z0-9]*");

  if (regex_match(input_search, descriptor))
  {
    return true;
  }

  else
  {
    return false;
  }
}

void Cyberminer::Call_KWIC()
{
    kwic.inputFileName = Cyberminer::inputFileName;
    kwic.outputFileName = "../output.txt";

    ofstream oFile(kwic.outputFileName);
    if (!oFile)
    {
        cerr << "Error opening " << kwic.outputFileName << " for writing." << endl;
        return;
    }

    kwic.Call_Input();
    kwic.Call_Circular_Shift();
    kwic.Call_Alphabetizer();
    kwic.Call_Output(oFile);
    oFile.close();
}

void Cyberminer::Search_Specification(string input_search, httplib::Response &res)
{
  stringstream splitLine(input_search);
  string token = " ";
  vector<string> words;

  while (splitLine >> token)
  {
    words.push_back(token.c_str());
  }

  int and_counter = 0;
  int or_counter = 0;
  int not_counter = 0;

  for(int i = 0; i < words.size(); i++)
  {
    if (words[i] == "AND")
    {
      and_counter++;
    }

    else if (words[i] == "OR")
    {
      or_counter++;
    }

    else if (words[i] == "NOT")
    {
      not_counter++;
    }
  }

  if ((and_counter > 0 && or_counter > 0) || (and_counter > 0 && not_counter > 0) || (or_counter > 0 && not_counter > 0))
  {
      cerr << "Mixed search operators were provided" << endl;
      res.status = 400;
      res.set_content("Mixed search operators (AND, OR, NOT) are not allowed", "text/plain");
      return;
  }

  else
  {
    if (and_counter >= 1)
    {
      Cyberminer::keyword_search = {true, "AND"};
    }

    else if (or_counter >= 1)
    {
      Cyberminer::keyword_search = {true, "OR"};
    }

    else if (not_counter >= 1)
    {
      Cyberminer::keyword_search = {true, "NOT"};
    }

    else
    {
      Cyberminer::keyword_search = {false, ""};
    }
  }
}

void Cyberminer::URL_Deletion(httplib::Response &res)
{
  ifstream file(Cyberminer::inputFileName);

  if (!file.is_open())
  {
    cout << "Error opening the file!" << endl;
    exit(1);
  }

  string line;
  vector<string> valid_urls;
  bool deleted = false;

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

    struct tm tm = {};
    strptime(line_parts[2].c_str(), "%a %b %d %H:%M:%S %Y", &tm);
    time_t last_visit = mktime(&tm);

    time_t now = time(0);

    double difference = difftime(now, last_visit);

    if (difference > 365 * 24 * 60 * 60)
    {
      deleted = true;
    }

    else
    {
      valid_urls.push_back(line_parts[0] + "$" + line_parts[1] + "$" + line_parts[2] + "$" + line_parts[3]);
    }
  }

  file.close();

  if (deleted)
  {
    ofstream ofs(Cyberminer::inputFileName);
    if (!ofs)
    {
        cout << "Error opening input_test.txt for writing." << endl;
        res.status = 500;
        res.set_content("Internal Server Error", "text/plain");
        return;
    }

    for (int i = 0; i < valid_urls.size(); i++)
    {
      ofs << valid_urls[i] << "\n";
    }

    ofs.close();
  }
}
//void Hyperlink_Enforcement();
//void Listing_Preference();
//void Results_Settings();
//void Autofill();
