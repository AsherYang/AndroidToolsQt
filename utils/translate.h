#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <string>
#include <vector>

using namespace std;


class Translate
{
public:
    Translate();
    ~Translate();

private:
    string encrypt(const string &sign);     // sign
    string truncate(const string &text);
    vector<string> translate(const string &text);   // translate

private:
    const string YOUDAO_URL = "http://openapi.youdao.com/api";
    const string YOUDAO_API_KEY = "1cfc98fa253d2112";
    const string YOUDAO_APP_SECRET = "YCBsopB8IIW3yqyUpqauykT0R2nwmNGK";
};

#endif // TRANSLATE_H
