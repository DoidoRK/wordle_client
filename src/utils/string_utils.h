#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

string toUpperCase(const string& input) {
    string result = input;

    for (char& c : result) {
        c = toupper(c);
    }

    return result;
}

#endif /* _STRING_UTILS_H_ */