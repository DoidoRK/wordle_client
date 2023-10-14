#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<uint8_t> stringToUint8Vector(const string& str) {
    vector<uint8_t> result;
    for (char c : str) {
        result.push_back(static_cast<uint8_t>(c));
    }
    return result;
}

string uint8VectorToString(const vector<uint8_t>& uint8Vector) {
    string result;
    for (uint8_t val : uint8Vector) {
        result.push_back(static_cast<char>(val));
    }
    return result;
}

void printUint8Vector(const vector<uint8_t>& uint8Vector) {
    for (const uint8_t val : uint8Vector) {
        cout << static_cast<int>(val) << " ";
    }
    cout << endl;
}

std::string toUpperCase(const std::string& input) {
    std::string result = input;

    for (char& c : result) {
        c = std::toupper(c);
    }

    return result;
}
#endif /* _STRING_UTILS_H_ */