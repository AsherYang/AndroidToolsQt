#ifndef STRUTIL_H
#define STRUTIL_H

#include <string>
#include <vector>

using namespace std;

class StrUtil
{
public:
    StrUtil();
    vector<string> split(const string &str, const string &delim);
    void replace_all(string &str, const string &old_value, const string &new_value);
};

#endif // STRUTIL_H
