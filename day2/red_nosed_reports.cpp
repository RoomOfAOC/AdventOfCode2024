//
// Created by Harold on 2024/12/2.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "../utils/str_op.h"

long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int cnt = 0;
    std::vector<int> numbers;
    numbers.reserve(5);
    bool check = true;
    while (std::getline(ifs, line))
    {
        numbers.clear();
        check = true;
        for (auto n : split(line))
        {
            auto nn = parse_num<int>(n);
            if (!numbers.empty() && (std::abs(nn - numbers.back()) > 3 || std::abs(nn - numbers.back()) < 1))
            {
                check = false;
                break;
            }
            numbers.push_back(nn);
        }
        if (check &&
            (std::is_sorted(numbers.cbegin(), numbers.cend()) || std::is_sorted(numbers.crbegin(), numbers.crend())))
            cnt++;
    }
    return cnt;
}

long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int cnt = 0;
    std::vector<int> numbers;
    numbers.reserve(5);
    auto check = [](std::vector<int> const& v) {
        if (!(std::is_sorted(v.cbegin(), v.cend()) || std::is_sorted(v.crbegin(), v.crend()))) return false;
        auto cpy = v;
        std::adjacent_difference(cpy.begin(), cpy.end(), cpy.begin());
        return std::all_of(cpy.cbegin() + 1, cpy.cend(), [](int x) { return std::abs(x) <= 3 && std::abs(x) >= 1; });
    };
    // auto check = [](std::vector<int> v)
    // {
    //     std::adjacent_difference(v.begin(), v.end(), v.begin());
    //     auto sign = *std::find_if(v.cbegin() + 1, v.cend(), [](int x) { return x != 0; });
    //     return std::all_of(v.cbegin() + 1, v.cend(), [sign](int x) { return std::abs(x) <= 3 && std::abs(x) >= 1 && x * sign >= 0; });
    // };
    int cnt_1 = 0;
    while (std::getline(ifs, line))
    {
        numbers.clear();
        for (auto n : split(line))
            numbers.push_back(parse_num<int>(n));
        if (check(numbers))
            cnt++;
        else
            for (auto i = 0; i < numbers.size(); i++)
            {
                auto cpy = numbers;
                cpy.erase(cpy.begin() + i);
                if (check(cpy))
                {
                    cnt++;
                    break;
                }
            }
    }
    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "How many reports are safe?\n" << part_1(argv[1]) << std::endl;
    std::cout << "How many reports are now safe?\n" << part_2(argv[1]) << std::endl;

    return 0;
}