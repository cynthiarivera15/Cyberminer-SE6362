## Introduction
This repository contains the source code for the Cyberminer search engine which uses a Key Word in Context (KWIC) index software system. The way the system was build it lets the user either use the Cyberminer search engine or the KWIC system.

### Setup
- Save the `httplib.h` file to `include/httplib.h` directory. See [here](https://github.com/yhirose/cpp-httplib/blob/master/httplib.h)
- Download the `nlohmann` folder and save it in `include/nlohmann`. See [here](https://github.com/nlohmann/json/tree/develop/include/nlohmann)
- Move the `json.hpp` file inside the `nlohmann` folder to `include/json.hpp` directory.
- Create build directory: `mkdir build`
- cd into build directory: `cd build`
- Compile: `cmake .. && make`
- Run server: `./server`
- Visit `http://localhost:8080`

### Main Page
- Decide whether to use the KWIC index system or the Cyberminer

### KWIC system
- The system only accepts string of the following format: URL|Description, where
  * URL part:_
    `URL ::= ‘http://’ url_identifier ‘.’ url_identifier ‘.’ [‘edu’ | ‘com’ | ‘org’ | ‘net’ | ‘gov’]’/’[identifier]*`_
    `url_identifier ::= {letter | digit | special characters}+`_
    `letter ::= [‘a’ | ‘b’ | ... | ‘y’ | ‘z’ | ‘A’ | ‘B’ | ... | ‘Y’ | ‘Z’]`_
    `digit ::= [‘1’ | ‘2’ | ... | ‘9’ | ‘0’]`_
    `special characters ::= [‘-’ | ‘_’ | ‘@’ | ‘/’ | ‘(‘ | ‘)’ | ‘?’ | ‘=’ | ‘&’ | ‘#’]`
  * Descriptor part:
    `Description ::= descriptor_identifier {‘ ‘ descriptor_identifier}*`
    `descriptor_identifier ::= {letter | digit}+`

### Cyberminer
- The system only accepts strings in the format of the descriptor part (do not add any noise words because is not program to ignore them yet which means that it will return every url where the descriptor has a noise word)
  * The list of noise words is as following: `a`, `an`, `and`, `at`, `be`, `but`, `can`, `in`, `it`, `is`, `of`, `or`, `so`, `that`, `the`, `then`, `there`, `they`, `this`, `was`, `yet`
- Additionally the user can decide whether to add a keyword (`AND`, `OR`, and `NOT`) to the descriptor part.
  * Only one type of keyword at a time is accepted.
  * `NOT` is the only keyword accepted at the beginning of the string, and always has to be followed by another search word
  * `AND` and `OR` must have at least one search word before and after
