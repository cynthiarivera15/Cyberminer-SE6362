/*------------------------------------------------------------------------------
Name: server.cpp
Description: Implemenation of the Cyberminer System
Course: SE6362

NOTE: To help organize the ouput in html ChatGPT was used
------------------------------------------------------------------------------*/
#include <httplib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Master_Control.h"
#include "Cyberminer.h"

using namespace std;

int main() {
  Cyberminer cyberminer;
  cyberminer.inputFileName = "../input_test.txt";

  Master_Control kwic_system;
  kwic_system.inputFileName = "../input_test.txt";

  httplib::Server server;
  server.set_base_dir("../public");

  server.Post("/process_form", [&cyberminer, &kwic_system](const httplib::Request &req, httplib::Response &res)
  {
    string input_search = req.get_param_value("search");
    string chosen_system = req.get_param_value("system");

    cout << "Received Text: " << input_search << endl;

    if (chosen_system == "cyberminer")
    {

      bool valid_input = cyberminer.Input_Verification(input_search);
      cyberminer.Search_Specification(input_search, res);

      if (!valid_input)
      {
        res.status = 400;  // Bad Request
        res.set_content("Invalid URL or search term", "text/plain");
        return;
      }

      cyberminer.search = input_search;
      res.set_redirect("/cyberminer", 303);
    }

    else
    {
      bool valid_input = kwic_system.URL_Verification(input_search, res);

      if (valid_input)
      {
        kwic_system.Save_Input(input_search, res);
      }

      else
      {
        res.status = 400;  // Bad Request
        res.set_content("Invalid URL or search term", "text/plain");
        return;
      }

      res.set_redirect("/kwic_system", 303);
    }
  });

  server.Get("/cyberminer", [&cyberminer](const httplib::Request &req, httplib::Response &res)
  {
    // Load the output.txt file
    ifstream ifs("../output.txt");
    if (!ifs) {
      cerr << "Error opening output.txt for reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return;
    }

    stringstream ss;
    string line;
    vector<string> output_lines;

    // Read lines and prepare them for HTML list items
    while (getline(ifs, line)) {
      if (line.empty()) {
        output_lines.push_back("<br>");
      }

      output_lines.push_back(line);
    }

    // Create the HTML for the unordered list
    string output;
    for (const auto& item : output_lines) {
      output += "<li>" + item + "</li>\n";  // Wrap each entry in <li>
    }

    // Load the index.html file
    ifstream ifs_index("../public/cyberminer_page.html");
    if (!ifs_index) {
      cerr << "Error opening cyberminer_page.html for reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return;
    }

    stringstream ss_index;
    ss_index << ifs_index.rdbuf();
    string index_html = ss_index.str();

    // Inject the output into the index.html file
    size_t pos = index_html.find("<ul>");
    if (pos != string::npos) {
      index_html.insert(pos + 4, output);  // Insert after the <ul> tag
    }
    else {
      cerr << "<ul> tag not found in cyberminer_page.html!" << endl;
    }

    cyberminer.URL_Deletion(res);

    res.set_content(index_html, "text/html");
  });

  server.Get("/kwic_system", [&cyberminer](const httplib::Request &req, httplib::Response &res)
  {
    cyberminer.Call_KWIC();

    // Load the output.txt file
    ifstream ifs("../output.txt");
    if (!ifs) {
      cerr << "Error opening output.txt for reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return;
    }

    stringstream ss;
    string line;
    vector<string> output_lines;

    // Read lines and prepare them for HTML list items
    while (getline(ifs, line)) {
      if (line.empty()) {
        output_lines.push_back("<br>");
      }

      output_lines.push_back(line);
    }

    // Create the HTML for the unordered list
    string output;
    for (int i = 0; i < output_lines.size(); i++)
    {
      output += "<li class='list-group-item'>" + output_lines[i] + "</li>\n";  // Wrap each entry in <li>
    }

    // Load the index.html file
    ifstream ifs_index("../public/kwic_system_page.html");
    if (!ifs_index) {
      cout << "Error opening for kwic_system_page.html reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return;
    }

    stringstream ss_index;
    ss_index << ifs_index.rdbuf();
    string index_html = ss_index.str();

    // Inject the output into the index.html file
    size_t pos = index_html.find("<ul>");
    if (pos != string::npos)
    {
      index_html.insert(pos + 4, output);  // Insert after the <ul> tag
    }

    else
    {
      cout << "<ul> tag not found in kwic_system_page.html!" << endl;
    }

    res.set_content(index_html, "text/html");
  });

  // Start the server on localhost:8080
  cout << "Server is running on http://localhost:8080\n";
  server.listen("localhost", 8080);

  return 0;
}
