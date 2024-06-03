Small databases in plain text.

`linedb-to-csv` was written with ChatGPT 4o. I was prepared for a few rounds of clarification and fixing, but to my pleasant surprise, ChatGPT got it right first time:

Q: Write a C++ program to convert from LineDB to CSV format. The program should work on the command line, taking the name of the LineDB file as command line argument, and printing the CSV to standard output. LineDB is a format of my invention, whose properties are:

The database is contained in a plain text file.

One line per field.

Records are separated by blank line.

Each line contains field name, `:`, zero or more spaces, then field data.

The first character of each field name should be case folded to uppercase, as should the first character of each value. You may assume both are always ASCII.

A record may omit a certain field altogether; that's treated as though it contained the empty string.

If a line is non-blank, but does not contain a `:`, it should be treated as a comment for the current record, and ignored.

Example input:

title: The Hitchhiker's Guide to the Galaxy
author: Douglas Adams
published: 1979
The first of a trilogy in five parts

Title: The Colour of Magic
author:Terry Pratchett
published: 1983

A: Below is a C++ program that converts from LineDB format to CSV format. This program reads a LineDB file provided as a command line argument and prints the CSV output to standard output.

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

std::string caseFold(const std::string& str) {
    std::string result = str;
    if (!result.empty()) {
        result[0] = toupper(result[0]);
    }
    return result;
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

void parseLineDB(std::ifstream& file, std::vector<std::map<std::string, std::string>>& records) {
    std::string line;
    std::map<std::string, std::string> record;

    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!record.empty()) {
                records.push_back(record);
                record.clear();
            }
        } else {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string fieldName = caseFold(trim(line.substr(0, colonPos)));
                std::string fieldValue = caseFold(trim(line.substr(colonPos + 1)));
                record[fieldName] = fieldValue;
            }
        }
    }

    if (!record.empty()) {
        records.push_back(record);
    }
}

void printCSV(const std::vector<std::map<std::string, std::string>>& records) {
    if (records.empty()) return;

    std::vector<std::string> headers;
    for (const auto& record : records) {
        for (const auto& field : record) {
            if (std::find(headers.begin(), headers.end(), field.first) == headers.end()) {
                headers.push_back(field.first);
            }
        }
    }

    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << headers[i];
        if (i < headers.size() - 1) std::cout << ",";
    }
    std::cout << std::endl;

    for (const auto& record : records) {
        for (size_t i = 0; i < headers.size(); ++i) {
            if (record.find(headers[i]) != record.end()) {
                std::cout << record.at(headers[i]);
            }
            if (i < headers.size() - 1) std::cout << ",";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <LineDB file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<std::map<std::string, std::string>> records;
    parseLineDB(inputFile, records);
    inputFile.close();

    printCSV(records);

    return 0;
}
```

This program performs the following steps:
1. Reads the LineDB file.
2. Parses the file into records, ignoring comments and case-folding the first characters of field names and values.
3. Collects unique headers from all records.
4. Prints the headers as the first row of the CSV.
5. Prints each record as a row in the CSV, ensuring that each record has values in the correct columns based on the headers.

To run the program, compile it with a C++ compiler and provide the LineDB file as an argument. For example:
```sh
g++ -o linedb_to_csv linedb_to_csv.cpp
./linedb_to_csv input.linedb
```
