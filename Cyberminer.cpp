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

void Cyberminer::Merge(vector<pair<string,string> > &content, int left, int mid, int right)
{
  int size1;
  int size2;

  size1 = mid - left + 1;
  size2 = right - mid;

  vector<pair<string,string> > leftDatabaseContent(size1);
  vector<pair<string,string> > rightDatabaseContent(size2);

  for (int i = 0; i < size1; i++)
  {
    leftDatabaseContent[i] = content[left + i];
  }

  for (int j = 0; j < size2; j++)
  {
    rightDatabaseContent[j] = content[mid + 1 + j];
  }

  int leftIndex = 0;
  int rightIndex = 0;
  int pointer = left;

  while (leftIndex < size1 && rightIndex < size2)
  {
    string temp1 = "";
    string temp2 = "";

    for (int i = 0; i < leftDatabaseContent[leftIndex].second.length(); i++)
    {
      temp1 += toupper(leftDatabaseContent[leftIndex].second[i]);
    }

    for (int i = 0; i < rightDatabaseContent[rightIndex].second.length(); i++)
    {
      temp2 += toupper(rightDatabaseContent[rightIndex].second[i]);
    }

    if (temp1 <= temp2)
    {
      content[pointer] = leftDatabaseContent[leftIndex];
      leftIndex++;
    }

    else
    {
      content[pointer] = rightDatabaseContent[rightIndex];
      rightIndex++;
    }

    pointer++;
  }

  while (leftIndex < size1)
  {
    content[pointer] = leftDatabaseContent[leftIndex];
    leftIndex++;
    pointer++;
  }

  while (rightIndex < size2)
  {
    content[pointer] = rightDatabaseContent[rightIndex];
    rightIndex++;
    pointer++;
  }
}

void Cyberminer::MergeSort(vector<pair<string,string> > &content, int left, int right)
{
  if (left < right)
  {
    int mid;
    mid = left + (right - left) / 2;

    MergeSort(content, left, mid);
    MergeSort(content, mid + 1, right);

    Merge(content, left, mid, right);
  }
}

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
      if(i == 0 || i == words.size() - 1)
      {
        cerr << "Keyword location is not valid" << endl;
        res.status = 400;
        res.set_content("Keyword location is not valid", "text/plain");
        return;
      }

      else
      {
        and_counter++;
      }
    }

    else if (words[i] == "OR")
    {
      if(i == 0 || i == words.size() - 1)
      {
        cerr << "Keyword location is not valid" << endl;
        res.status = 400;
        res.set_content("Keyword location is not valid", "text/plain");
        return;
      }

      else
      {
        or_counter++;
      }
    }

    else if (words[i] == "NOT")
    {
      if(i == words.size() - 1)
      {
        cerr << "Keyword location is not valid" << endl;
        res.status = 400;
        res.set_content("Keyword location is not valid", "text/plain");
        return;
      }

      else
      {
        not_counter++;
      }
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
    cout << "Error opening input_test.txt for reading." << endl;
    res.status = 500;
    res.set_content("Internal Server Error", "text/plain");
    return;
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

vector<string> Cyberminer::Sensitive_Search(httplib::Response &res)
{
  stringstream splitLine(Cyberminer::search);
  string token = " ";
  vector<string> words;
  vector<string> matching_urls;

  while (splitLine >> token)
  {
    words.push_back(token.c_str());
  }

  ifstream file(Cyberminer::outputFileName);

  if (!file.is_open())
  {
    cout << "Error opening output.txt for reading." << endl;
    res.status = 500;
    res.set_content("Internal Server Error", "text/plain");
    return {};
  }

  string line;

  while (getline(file, line))
  {
    stringstream splitLine(line);
    string token;
    vector<string> line_parts;

    while (getline(splitLine, token, ':'))
    {
      token.erase(0, token.find_first_not_of(" "));
      token.erase(token.find_last_not_of(" ") + 1);
      line_parts.push_back(token);
    }

    string descriptor = line_parts[0];
    string url = line_parts[1] + ":" + line_parts[2];

    stringstream splitLine2(descriptor);
    string token2 = " ";
    vector<string> words2;

    while (splitLine2 >> token2)
    {
      words2.push_back(token2.c_str());
    }

    if (Cyberminer::keyword_search.first && Cyberminer::keyword_search.second != "OR")
    {
      bool append_url = true;

      if (Cyberminer::keyword_search.second == "AND")
      {
        for (int i = 0; i < words.size(); i++)
        {
          if (words[i] != "AND")
          {
            if (find(words2.begin(), words2.end(), words[i]) == words2.end())
            {
                append_url = false;
                break;
            }
          }
        }

        if (append_url)
        {
            if (find(matching_urls.begin(), matching_urls.end(), url) == matching_urls.end())
            {
                matching_urls.push_back(url);
                cout << "Found URL: " << url << endl;
            }
        }
      }

      else
      {
        size_t not_pos = find(words.begin(), words.end(), "NOT") - words.begin();
        vector<string> before_not(words.begin(), words.begin() + not_pos);
        vector<string> after_not(words.begin() + not_pos + 1, words.end());

        bool in_before_not = false;

        for (int i = 0; i < before_not.size(); i++)
        {
          if (find(words2.begin(), words2.end(), before_not[i]) != words2.end())
          {
            in_before_not = true;
            break;
          }
        }

        if (in_before_not || before_not.size() == 0)
        {
          for (int i = 0; i < after_not.size(); i++)
          {
            if (find(words2.begin(), words2.end(), after_not[i]) != words2.end())
            {
              append_url = false;
              break;
            }
          }
        }

        else
        {
          append_url = false;
        }

        if (append_url)
        {
            if (find(matching_urls.begin(), matching_urls.end(), url) == matching_urls.end())
            {
                matching_urls.push_back(url);
                cout << "Found URL: " << url << endl;
            }
        }
      }
    }

    else
    {
      for(int i = 0; i < words.size(); i++)
      {
        for(int j = 0; j < words2.size(); j++)
        {
          if(words[i] == words2[j])
          {
            auto it = find(matching_urls.begin(), matching_urls.end(), url);

            if (it == matching_urls.end())
            {
              matching_urls.push_back(url);
            }

            break;
          }
        }
      }
    }
  }

  file.close();

  return matching_urls;
}

vector<int> Cyberminer::Results_Settings(vector<string> urls, httplib::Request req)
{
  int current_page = 1;
  if (req.has_param("page"))
  {
      current_page = stoi(req.get_param_value("page"));
  }

  int items_per_page = 5;
  if (req.has_param("items_per_page")) {
      items_per_page = stoi(req.get_param_value("items_per_page"));
  }

  int total_items = urls.size();
  int total_pages = (total_items + items_per_page - 1) / items_per_page;

  int start_index = (current_page - 1) * items_per_page;
  int end_index = min(start_index + items_per_page, total_items);

  return {start_index, end_index, current_page, items_per_page, total_pages};
}

vector<string> Cyberminer::Listing_Preference(vector<string> urls, httplib::Response &res, httplib::Request req)
{
  string listing_preference = req.has_param("listing_preference") ? req.get_param_value("listing_preference") : "";
  vector<string> organized_urls;

  if (listing_preference.empty())
  {
    return urls;
  }

  else
  {
    ifstream file(Cyberminer::inputFileName);

    if (!file.is_open())
    {
      cout << "Error opening input_test.txt for reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return {};
    }

    string line;

    vector<pair<string, string> > databaseContent;
    vector<pair<string, string> > accessRate;

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

      if (listing_preference == "Alphabetized")
      {
        databaseContent.push_back({line_parts[0], line_parts[1]});
      }

      else
      {
        accessRate.push_back({line_parts[0], line_parts[3]});
      }
    }

    file.close();
    vector<string> all_urls;

    if (listing_preference == "Alphabetized")
    {
      int databaseSize = databaseContent.size();
      Cyberminer::MergeSort(databaseContent, 0, databaseSize - 1);

      for(int i = 0; i < databaseContent.size(); i++)
      {
        all_urls.push_back(databaseContent[i].first);
      }

      for (int i = 0; i < all_urls.size(); i++)
      {
        if (find(urls.begin(), urls.end(), all_urls[i]) != urls.end())
        {
          organized_urls.push_back(all_urls[i]);
        }
      }
    }

    else
    {
      int databaseSize = accessRate.size();
      Cyberminer::MergeSort(accessRate, 0, databaseSize - 1);

      for(int i = 0; i < accessRate.size(); i++)
      {
        int index  = databaseSize - 1 - i;
        all_urls.push_back(accessRate[index].first);
      }

      for (int i = 0; i < all_urls.size(); i++)
      {
        if (find(urls.begin(), urls.end(), all_urls[i]) != urls.end())
        {
          organized_urls.push_back(all_urls[i]);
        }
      }
    }
  }

  return organized_urls;
}

void Cyberminer::Hyperlink_Enforcement(string url, httplib::Response &res)
{
  ifstream file(Cyberminer::inputFileName);

  if (!file.is_open())
  {
    cout << "Error opening input_test.txt for reading." << endl;
    res.status = 500;
    res.set_content("Internal Server Error", "text/plain");
    return;
  }

  string line;
  vector<string> valid_urls;

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


    if (url != line_parts[0])
    {
      valid_urls.push_back(line_parts[0] + "$" + line_parts[1] + "$" + line_parts[2] + "$" + line_parts[3]);
    }

    else
    {
      time_t now = time(0);
      string last_visit = ctime(&now);

      if (last_visit[last_visit.length() - 1] == '\n')
      {
        last_visit.pop_back();
      }

      int num_visits = stoi(line_parts[3]) + 1;

      valid_urls.push_back(line_parts[0] + "$" + line_parts[1] + "$" + last_visit + "$" + to_string(num_visits));
    }
  }

  file.close();

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
