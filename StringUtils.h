#ifndef StringUtils_
#define StringUtils_

#include <sstream>
#include <string>
using namespace std;

int stringToInt(const string& str) {
    stringstream ss(str);
    int result;
    ss >> result;
    return result;
}

string intToString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

string trim(const string& str) {
    if (str.empty()) return str;

    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(" \t\n\r");

    return str.substr(first, (last - first + 1));
}

// Truncate string to maxWidth and add "..." if needed
string truncateString(const string& str, int maxWidth) {
    if ((int)str.length() <= maxWidth) {
        return str;
    }
    if (maxWidth <= 3) {
        return str.substr(0, maxWidth);
    }
    return str.substr(0, maxWidth - 3) + "...";
}

#endif
