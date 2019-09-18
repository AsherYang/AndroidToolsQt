#include "strutil.h"

StrUtil::StrUtil()
{

}

/**
 * 字符串分割
 * see {https://blog.csdn.net/glmushroom/article/details/80690881} 
 *
 * @brief strutil::split
 * @param str
 * @param delim
 * @return
 */
vector<string> StrUtil::split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str) return res;
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p) {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }
    return res;
}

/**
 * 字符串替换
 * see {https://blog.csdn.net/qq_34392405/article/details/79699606}
 *
 * @brief strutil::replace_all
 * @param str
 * @param old_value
 * @param new_value
 */
void StrUtil::replace_all(string &str, const string &old_value, const string &new_value)
{
    string::size_type pos = str.find(old_value), t_size = old_value.size(), r_size = new_value.size();
    while (pos != std::string::npos) {
        str.replace(pos, t_size, new_value);
        pos = str.find(old_value, pos + r_size);
    }
}
