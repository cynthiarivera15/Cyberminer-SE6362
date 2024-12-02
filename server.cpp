/*------------------------------------------------------------------------------
Name: server.cpp
Description: Implemenation of the Cyberminer System
Course: SE6362

NOTE: To help organize the ouput in html ChatGPT was used
------------------------------------------------------------------------------*/
#include <httplib.h>
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Master_Control.h"
#include "Cyberminer.h"

using namespace std;
using json = nlohmann::json;

int main() {
  Cyberminer cyberminer;
  cyberminer.inputFileName = "../input_test.txt";
  cyberminer.outputFileName = "../output.txt";

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

      if (!valid_input)
      {
        res.status = 400;
        res.set_content("Invalid URL or search term", "text/plain");
        return;
      }

      cyberminer.Search_Specification(input_search, res);

      if (res.status != 400)
      {
        cyberminer.search = input_search;
        res.set_redirect("/cyberminer", 303);
      }
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
        res.status = 400;
        res.set_content("Invalid URL or search term", "text/plain");
        return;
      }

      res.set_redirect("/kwic_system", 303);
    }
  });

  // Segment of code obtained from ChatGPT
  server.Post("/log_click", [&cyberminer](const httplib::Request &req, httplib::Response &res)
  {
    // Parse the JSON body to extract the URL
    auto body = json::parse(req.body);
    string clicked_url = body["url"];

    // Log or store the clicked URL (e.g., increment access count)
    cyberminer.Hyperlink_Enforcement(clicked_url, res);

    // Send an acknowledgment
    res.set_content("Click logged", "text/plain");
  });

  server.Get("/cyberminer", [&cyberminer](const httplib::Request &req, httplib::Response &res)
  {
    vector<string> urls = cyberminer.Sensitive_Search(res);

    vector<int> indexes = cyberminer.Results_Settings(urls, req);

    vector<string> organized_urls = cyberminer.Listing_Preference(urls, res, req);

    string output = "<h4>This are the results for: " + cyberminer.search + "</h4>\n";

    output += "<form method=\"get\" action=\"/cyberminer\" style=\"margin-left: 10px; font-size: 0.8em;\">\n";
    output += "  <label for=\"listing_preference\" style=\"margin-right: 5px;\">Listing Preference:</label>\n";
    output += "  <select id=\"listing_preference\" name=\"listing_preference\">\n";
    output += "    <option value=\"\"" + string((req.get_param_value("listing_preference").empty() ? "selected" : "")) + ">None</option>\n";
    output += "    <option value=\"Alphabetized\" " + string((req.get_param_value("listing_preference") == "Alphabetized" ? "selected" : "")) + ">Alphabetized</option>\n";
    output += "    <option value=\"Frequently accessed\" " + string((req.get_param_value("listing_preference") == "Frequently accessed" ? "selected" : "")) + ">Frequently accessed</option>\n";
    output += "  </select>\n";
    output += "  <input type=\"hidden\" name=\"page\" value=\"1\">";
    output += "  <input type=\"hidden\" name=\"items_per_page\" value=\"" + to_string(indexes[3]) + "\">\n";
    output += "  <button type=\"submit\" style=\"height: 20px; font-size: 0.8em;\">Choose</button>\n";
    output += "</form><br>\n";

    output += "<ul id=\"result-list\">";

    for (int i = indexes[0]; i < indexes[1]; i++) {
      output += "<li><a href=\"" + organized_urls[i] + "\" target=\"_blank\">" + organized_urls[i] + "</a></li>\n\n";
    }

    output += "</ul>";

    string pagination = "<div class=\"pagination\" id=\"pagination-links\">\n";
    if (indexes[2] > 1)
    {
      pagination += "<a href=\"/cyberminer?page=" + to_string(indexes[2] - 1) + "&items_per_page=" + to_string(indexes[3]) + "&listing_preference=" + req.get_param_value("listing_preference") + "\">Previous</a>\n";
    }

    pagination += "<form method=\"get\" action=\"/cyberminer\" style=\"margin-left: 10px; font-size: 0.8em;\">\n";
    pagination += "  <label for=\"items_per_page\" style=\"margin-right: 5px;\">Items/Page:</label>\n";
    pagination += "  <input type=\"number\" id=\"items_per_page\" name=\"items_per_page\" min=\"1\" style=\"width: 50px; height: 20px; font-size: 0.8em;\" placeholder=\"" + to_string(indexes[3]) + "\" required>\n";
    pagination += "  <input type=\"hidden\" name=\"page\" value=\"1\">\n";
    pagination += "  <input type=\"hidden\" name=\"listing_preference\" value=\"" + req.get_param_value("listing_preference") + "\">\n";
    pagination += "  <button type=\"submit\" style=\"height: 20px; font-size: 0.8em;\">Set</button>\n";
    pagination += "</form>\n";

    if (indexes[2] < indexes[4])
    {
      pagination += "<a href=\"/cyberminer?page=" + to_string(indexes[2] + 1) + "&items_per_page=" + to_string(indexes[3]) + "&listing_preference=" + req.get_param_value("listing_preference") + "\">Next</a>\n";
    }

    pagination += "</div>\n";

    ifstream ifs_index("../public/cyberminer_page.html");
    if (!ifs_index) {
      cout << "Error opening cyberminer_page.html for reading." << endl;
      res.status = 500;
      res.set_content("Internal Server Error", "text/plain");
      return;
    }

    stringstream ss_index;
    ss_index << ifs_index.rdbuf();
    string index_html = ss_index.str();

    size_t pos = index_html.find("<ul id=\"result-list\">");
    if (pos != string::npos)
    {
      size_t end_pos = index_html.find("</ul>", pos);
      if (end_pos != string::npos)
      {
        index_html.insert(end_pos, output);
      }
    }

    else
    {
      cout << "<ul id=\"result-list\"> tag not found in cyberminer_page.html!" << endl;
    }

    size_t pos_pagination = index_html.find("<div class=\"pagination\" id=\"pagination-links\">");
    if (pos_pagination != string::npos)
    {
      size_t end_pos_pagination = index_html.find("</div>", pos_pagination);
      if (end_pos_pagination != string::npos)
      {
        index_html.insert(end_pos_pagination, pagination);
      }
    }

    else
    {
      cout << "<div class=\"pagination\"> tag not found in cyberminer_page.html!" << endl;
    }

    cyberminer.URL_Deletion(res);

    res.set_content(index_html, "text/html");
  });

  server.Get("/kwic_system", [&cyberminer](const httplib::Request &req, httplib::Response &res)
  {
    cyberminer.Call_KWIC();

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

    while (getline(ifs, line)) {
      if (line.empty()) {
        output_lines.push_back("<br>");
      }

      output_lines.push_back(line);
    }

    string output;
    for (int i = 0; i < output_lines.size(); i++)
    {
      output += "<li class='list-group-item'>" + output_lines[i] + "</li>\n";  // Wrap each entry in <li>
    }

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

    size_t pos = index_html.find("<ul>");
    if (pos != string::npos)
    {
      index_html.insert(pos + 4, output);
    }

    else
    {
      cout << "<ul> tag not found in kwic_system_page.html!" << endl;
    }

    res.set_content(index_html, "text/html");
  });

  cout << "Server is running on http://localhost:8080\n";
  server.listen("localhost", 8080);

  return 0;
}
