/*------------------------------------------------------------------------------
Name: server.cpp
Description: Implemenation of the Cyberminer System
Course: SE6362
------------------------------------------------------------------------------*/
#include <httplib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Cyberminer.h"

using namespace std;

int main() {
  Cyberminer cyberminer;
  cyberminer.inputFileName = "../input_test.txt";

  httplib::Server server;
  server.set_base_dir("../public");

  server.Post("/process_form", [](const httplib::Request &req, httplib::Response &res)
  {
    string input_search = req.get_param_value("search");
    cout << "Received Text: " << input_search << endl;

    bool accept = cyberminer.URL_Verification(input_search)

    if (accept)
    {
      cyberminer.Save_Input(input_search, res);
    }

    else
    {
      res.status = 400;  // Bad Request
      res.set_content("Invalid URL or search term", "text/plain");
      return;
    }
  });

  server.Get("/home", [](const httplib::Request &req, httplib::Response &res)
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
    for (const auto& item : output_lines) {
      output += "<li>" + item + "</li>\n";  // Wrap each entry in <li>
    }

    // Load the index.html file
    ifstream ifs_index("../public/index.html");
    if (!ifs_index) {
      cerr << "Error opening index.html for reading." << endl;
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
      cerr << "<ul> tag not found in index.html!" << endl;
    }

    res.set_content(index_html, "text/html");
  });

  // Start the server on localhost:8080
  cout << "Server is running on http://localhost:8080\n";
  server.listen("localhost", 8080);

  return 0;
}
