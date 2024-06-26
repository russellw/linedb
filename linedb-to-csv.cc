#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

// Function to case-fold the first character of a string
std::string caseFold(const std::string& str) {
    std::string result = str;
    if (!result.empty()) {
        result[0] = toupper(result[0]);
    }
    return result;
}

// Function to trim leading and trailing spaces
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to parse LineDB format
void parseLineDB(std::ifstream& file, std::vector<std::map<std::string, std::string>>& records, std::vector<std::string>& headers) {
    std::string line;
    std::map<std::string, std::string> record;
    std::set<std::string> headerSet;

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
                if (headerSet.find(fieldName) == headerSet.end()) {
                    headers.push_back(fieldName);
                    headerSet.insert(fieldName);
                }
            }
        }
    }

    if (!record.empty()) {
        records.push_back(record);
    }
}

// Function to print records in CSV format
void printCSV(const std::vector<std::map<std::string, std::string>>& records, const std::vector<std::string>& headers) {
    if (records.empty()) return;

    // Print headers
    for (size_t i = 0; i < headers.size(); ++i) {
        std::cout << headers[i];
        if (i < headers.size() - 1) std::cout << ",";
    }
    std::cout << std::endl;

    // Print records
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
    std::vector<std::string> headers;
    parseLineDB(inputFile, records, headers);
    inputFile.close();

    printCSV(records, headers);

    return 0;
}
