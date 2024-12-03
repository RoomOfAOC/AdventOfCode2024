//
// Created by Harold on 2024/12/3.
//

#include <fstream>
#include <string>
#include <regex>
#include <iostream>

#include "../utils/str_op.h"

long part_1(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    std::string text(std::istreambuf_iterator<char>{ifs}, {});
    std::regex mul("mul\\((\\d{1,3}),(\\d{1,3})\\)", std::regex_constants::ECMAScript);
    for (auto i = std::sregex_iterator(text.begin(), text.end(), mul); i != std::sregex_iterator{}; i++)
    {
        std::smatch match = *i;
        auto n1 = parse_num<int>(match[1].str());
        auto n2 = parse_num<int>(match[2].str());
        res += n1 * n2;
    }
    return res;
}

long part_2(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    std::string text(std::istreambuf_iterator<char>{ifs}, {});
    //\note: need join the text into one line, since the regex matching `.*` does NOT work with multiline...
    text = std::regex_replace(text, std::regex("\n"), "");

    text = "do()" + text + "don't()";
    std::regex ddmul("do\\(\\).*?(mul\\((\\d{1,3}),(\\d{1,3})\\))+.*?don't\\(\\)", std::regex_constants::ECMAScript);
    std::regex mul("mul\\((\\d{1,3}),(\\d{1,3})\\)", std::regex_constants::ECMAScript);
    for (auto i = std::sregex_iterator(text.begin(), text.end(), ddmul); i != std::sregex_iterator{}; i++)
    {
        auto match_str = i->str();
        for (auto j = std::sregex_iterator(match_str.begin(), match_str.end(), mul); j != std::sregex_iterator{}; j++)
        {
            std::smatch match = *j;
            auto n1 = parse_num<int>(match[1].str());
            auto n2 = parse_num<int>(match[2].str());
            res += n1 * n2;
        }
    }

    //text += "do()";
    //std::regex dd("don't\\(\\).*?do\\(\\)", std::regex_constants::ECMAScript);
    //text = std::regex_replace(text, dd, "");
    //std::regex mul("mul\\((\\d{1,3}),(\\d{1,3})\\)", std::regex_constants::ECMAScript);
    //for (auto i = std::sregex_iterator(text.begin(), text.end(), mul); i != std::sregex_iterator{}; i++)
    //{
    //    std::smatch match = *i;
    //    auto n1 = parse_num<int>(match[1].str());
    //    auto n2 = parse_num<int>(match[2].str());
    //    res += n1 * n2;
    //}
    return res;
}

int main(int argc, char* argv[])
{
    std::cout << "What do you get if you add up all of the results of the multiplications?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "what do you get if you add up all of the results of just the enabled multiplications?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}