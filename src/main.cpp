#include "dictionaryInterface.h"
#include "dawgInterface.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void showResult(vector<ResultType> items)
{
    cout << "total: " << items.size() << std::endl;
    for (auto item : items)
    {
        std::cout << "<Ret value=" << item.value << " start=" << item.start << " length=" << item.length << ">" << std::endl;
    }
}

int main(int argc, char **argv)
{
    DartsDictionary dict;
    dict.build("../data/test.json");
    // dict.build(argv[1]);
    vector<ResultType> result = dict.search("我浙江大学城市", 0);
    showResult(result);
    result = dict.searchUtf8("我浙江大学城市", 0);
    showResult(result);
    result = dict.searchUtf8("我浙江大学", 0);
    showResult(result);

    DawgDictionary dawg;
    dawg.build("../data/test.json");
    // dict.build(argv[1]);
    // vector<ResultType> result = dict.search("我浙江大学城市", 0);
    // showResult(result);
    // result = dict.searchUtf8("我浙江大学城市", 0);
    // showResult(result);
    // result = dict.searchUtf8("我浙江大学", 0);
    // showResult(result);

    // try
    // {
    //     Darts::DartsConfig config;
    //     config.parse(argc, argv);

    //     Darts::DoubleArray dic;
    //     if (dic.open(config.dic_file_name()) != 0)
    //     {
    //         std::cerr << "error: failed to open dictionary file: "
    //                   << config.dic_file_name() << std::endl;
    //         std::exit(1);
    //     }

    //     if (std::strcmp(config.lexicon_file_name(), "-") != 0)
    //     {
    //         std::ifstream file(config.lexicon_file_name());
    //         if (!file)
    //         {
    //             std::cerr << "error: failed to open lexicon file: "
    //                       << config.lexicon_file_name() << std::endl;
    //             std::exit(1);
    //         }
    //         darts_search(config, dic, &file);
    //     }
    //     else
    //     {
    //         darts_search(config, dic, &std::cin);
    //     }
    // }
    // catch (const std::exception &ex)
    // {
    //     std::cerr << "exception: " << ex.what() << std::endl;
    //     throw ex;
    // }

    return 0;
}
