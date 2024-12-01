/*------------------------------------------------------------------------------
Name: Master_Control.cpp
Description: Definition of the Master_Control class
Course: SE6362
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <httplib.h>

#include "Master_Control.h"

using namespace std;

void Master_Control::Call_Input()
{
  inputFile.fileName = Master_Control::inputFileName;

  inputFile.Read_File(readLine);
}

void Master_Control::Call_Circular_Shift()
{
  shiftedLine.Generate_Circular_Shift(readLine);
}

void Master_Control::Call_Alphabetizer()
{
  alphabetizedLine.Generate_Alpha(shiftedLine);
}

void Master_Control::Call_Output(ofstream &oFile)
{
  outputFile.Display_Lines(alphabetizedLine, oFile, Master_Control::inputFileName);
}

bool Master_Control::URL_Verification(string input_search, httplib::Response &res)
{
  size_t delimiter_pos = input_search.find('|');

  string url_part = input_search.substr(0, delimiter_pos);
  string descriptor_part = input_search.substr(delimiter_pos + 1);

  url_part.erase(0, url_part.find_first_not_of(" "));
  url_part.erase(url_part.find_last_not_of(" ") + 1);

  descriptor_part.erase(0, descriptor_part.find_first_not_of(" "));
  descriptor_part.erase(descriptor_part.find_last_not_of(" ") + 1);

  regex url("(http|https)://[a-zA-Z0-9-]+\\.[a-zA-Z0-9-]+\\.(edu|com|org|net|gov)/[a-zA-Z0-9-_/@\\(\\)\\?=&#]*");
  regex descriptor("[a-zA-Z0-9][\\sa-zA-Z0-9]*");

  if (regex_match(url_part, url) && regex_match(descriptor_part, descriptor))
  {
    return true;
  }

  else
  {
    return false;
  }
}

void Master_Control::Save_Input(string input_search, httplib::Response &res)
{
    size_t delimiter_pos = input_search.find('|');

    string url_part = input_search.substr(0, delimiter_pos);
    string descriptor_part = input_search.substr(delimiter_pos + 1);

    descriptor_part.erase(0, descriptor_part.find_first_not_of(" "));
    descriptor_part.erase(descriptor_part.find_last_not_of(" ") + 1);


    ofstream ofs(Master_Control::inputFileName, ios::app);
    if (!ofs)
    {
        cerr << "Error opening input_test.txt for writing." << endl;
        res.status = 500;
        res.set_content("Internal Server Error", "text/plain");
        return;
    }

    time_t now = time(0);
    string last_visit = ctime(&now);

    if (last_visit[last_visit.length() - 1] == '\n')
    {
      last_visit.pop_back();
    }

    ofs << url_part << "$" << descriptor_part << "$" << last_visit << "$" << "0" << "\n";

    ofs.close();
}
