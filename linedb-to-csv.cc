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
