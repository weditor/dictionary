#include "dawgdic/dawg.h"
#include "dawgdic/dawg-builder.h"
#include "dawgdic/dictionary-builder.h"
#include "dawgdic/guide-builder.h"
#include "dawgdic/ranked-completer.h"
#include "dawgdic/ranked-guide-builder.h"
#include "dictTypes.h"
#include <map>
// #include <string>
#include <vector>

// struct ResultType
// {
//     int value;
//     std::size_t length;
//     std::size_t start;
// };

class DawgDictionary
{
public:
    DawgDictionary();
    ~DawgDictionary();
    bool build(std::string fileName);

    ResultType exactMatchSearch(const char *key, std::size_t length = 0) const;

    std::vector<ResultType> commonPrefixSearch(const char *key, std::size_t length = 0) const;

    /** 
     * 从字符串中搜索, 
     * @param key: 等待搜索的字符串。
     * @param length: 搜索的最大长度。即截取字符串的 length 进行搜索
     * @param start_pos: 匹配的开始位置，跳过前面的字符。
     */
    std::vector<ResultType> search(const char *key, std::size_t length = 0) const;
    /** 
     * 和 search 一样，参数意义一样，但是返回值的 start/length 都是按照 utf8 长度计算 。
    */
    std::vector<ResultType> searchUtf8(const char *key, std::size_t length = 0) const;

    // value_type traverse(const char *key,
    //                     std::size_t &node_pos,
    //                     std::size_t &key_pos,
    //                     std::size_t length = 0) const;

private:
    std::map<std::string, std::size_t> readFromFile(std::string fileName);

private:
    // todo: 修改为智能指针
    dawgdic::Dictionary m_dict;
    dawgdic::RankedGuide m_guide;
};