
#include "dawgInterface.h"
#include <co/json.h>
#include <co/str.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>
#include <iostream>
// #include "dawgdic/dawg-builder.h"
// #include "dawgdic/dictionary-builder.h"

using namespace std;

static size_t utf8len(const char *s)
{
    int len = 0;
    while (*s)
        len += (*s++ & 0xc0) != 0x80;
}

static size_t toUtfPos(const char *s, size_t pos)
{
    int len = 0;
    const char *end = &s[pos];
    while (*s && s <= end)
        len += (*s++ & 0xc0) != 0x80;
    if (!*s)
    {
        len += 1;
    }
    return len - 1;
}

static size_t fromUtfPos(const char *s, size_t utf8Pos)
{
    int len = -1;
    const char *start = s;
    while (*s)
    {
        len += (*s++ & 0xc0) != 0x80;
        if (len >= utf8Pos)
        {
            break;
        }
    }
    if (!*s)
    {
        return strlen(start);
    }
    return s - start - 1;
}

DawgDictionary::DawgDictionary()
{
}

DawgDictionary::~DawgDictionary()
{
}

bool DawgDictionary::build(std::string fileName)
{
    map<string, size_t> wordMap = this->readFromFile(fileName);

    dawgdic::DawgBuilder builder;
    for (const auto &it : wordMap)
    {
        if (!builder.Insert(it.first.c_str(), (dawgdic::ValueType)(it.second)))
        {
            return false;
        }
    }
    dawgdic::Dawg dawg;
    if (!builder.Finish(&dawg))
    {
        return false;
    }

    if (!dawgdic::DictionaryBuilder::Build(dawg, &m_dict))
    {
        return false;
    }
    if (!dawgdic::RankedGuideBuilder::Build(dawg, m_dict, &m_guide))
    {
        return false;
    }
    return true;
}

vector<ResultType> DawgDictionary::commonPrefixSearch(const char *key, std::size_t length) const
{
    // const size_t max_num_results = 64;
    // InnerResultType tmpResults[max_num_results];
    // const size_t retSize = m_dict.commonPrefixSearch(key, tmpResults, max_num_results, length, 0);

    // vector<ResultType> results;
    // results.reserve(retSize);
    // for (size_t i = 0; i < retSize; i++)
    // {
    //     const auto &item = tmpResults[i];
    //     results.push_back({item.value, item.length, 0});
    // }
    // return results;
}

vector<ResultType> DawgDictionary::search(const char *key, size_t length) const
{
    // const size_t max_num_results = 64;
    // InnerResultType tmpResults[max_num_results];

    // size_t end_pos = (length > 0 ? length : strlen(key));

    // vector<ResultType> results;

    // for (size_t start = 0; start < end_pos; start++)
    // {
    //     // std::cout << "content: " << key << ", total: " << strlen(key) << ", start: " << start << ", len: " << 0 << endl;
    //     const size_t retSize = m_dict.commonPrefixSearch(key + start, tmpResults, max_num_results, end_pos - start, 0);
    //     for (size_t i = 0; i < retSize; i++)
    //     {
    //         const auto &item = tmpResults[i];

    //         // std::cout << "value:" << item.value << ", length:" << item.length << ",text:" << test.substr(start, item.length) << endl;
    //         results.push_back({item.value, item.length, start});
    //     }
    // }
    // return results;
}

vector<ResultType> DawgDictionary::searchUtf8(const char *key, size_t length) const
{
    // const size_t max_num_results = 64;
    // InnerResultType tmpResults[max_num_results];

    // length = (length > 0 ? length : strlen(key));

    // vector<ResultType> results;
    // size_t utf8_pos = 0;
    // for (size_t start = 0; start < length; start++)
    // {
    //     if ((key[start] & 0xc0) == 0x80)
    //     {
    //         continue;
    //     }
    //     std::cout << "start: " << start << std::endl;
    //     const size_t retSize = m_dict.commonPrefixSearch(key + start, tmpResults, max_num_results, length - start, 0);
    //     for (size_t i = 0; i < retSize; i++)
    //     {
    //         const auto &item = tmpResults[i];
    //         const size_t utf8Len = toUtfPos(&key[start], item.length);
    //         std::cout << "start: " << start << ", len: " << item.length << ", utf8len:" << utf8Len << std::endl;
    //         results.push_back({item.value, utf8Len, utf8_pos});
    //     }
    //     utf8_pos += 1;
    // }
    // return results;
}

map<string, size_t> DawgDictionary::readFromFile(string fileName)
{
    const size_t bufSize = 1024 * 4;
    char line[bufSize];
    memset(line, 0, sizeof(line));

    std::fstream inFile(fileName, ios::in);
    map<string, size_t> wordMap;
    Json jsLine;
    cout << "read from:" << fileName << endl;
    while (!inFile.eof())
    {
        inFile.getline(line, bufSize - 1);

        if (!jsLine.parse_from(line))
        {
            cout << "line json invalid:" << line << endl;
            continue;
        }
        if (!jsLine.is_object() || !jsLine.has_member("word") || !jsLine["word"].is_string() || !jsLine.has_member("value") || !jsLine["value"].is_int())
        {
            cout << "line json invalid2:" << line << endl;
            continue;
        }

        string act("add");
        if (!jsLine.has_member("act"))
        {
            ;
        }
        else if (!jsLine["act"].is_string())
        {
            // todo: raise
        }
        else if (jsLine["act"].get_string() == "add")
        {
            ;
        }
        else if (jsLine["act"].get_string() == "del")
        {
            act = "del";
        }
        else
        {
            // raise
        }
        if (act == "add")
        {
            wordMap[jsLine["word"].get_string()] = jsLine["value"].get_int32();
        }
        else if (act == "del")
        {
            // 如果 key 不存在会不会报错？
            wordMap.erase(jsLine["word"].get_string());
            // delete wordMap[jsLine["word"].get_string()];
        }
    }

    return wordMap;
}