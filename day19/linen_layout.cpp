//
// Created by Harold on 2024/12/19.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../utils/str_op.h"
#include "../utils/debug.h"

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    auto patterns = split(line, ", ");
    std::getline(ifs, line);
    std::unordered_map<std::string, bool> designs;
    auto rec = [&patterns, &designs](std::string design, auto&& rec) {
        if (design.empty()) return true;
        if (designs.find(design) != designs.end()) return designs[design];
        bool can = false;
        for (auto const& p : patterns)
            if (auto l = p.size(); p == design.substr(0, l)) can |= rec(design.substr(l), rec);
        designs[design] = can;
        return can;
    };
    size_t cnt = 0;
    while (std::getline(ifs, line))
        cnt += rec(line, rec);
    return cnt;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    auto patterns = split(line, ", ");
    std::getline(ifs, line);
    std::unordered_map<std::string, size_t> designs;
    auto rec = [&patterns, &designs](std::string design, auto&& rec) {
        if (design.empty()) return size_t{1};
        if (designs.find(design) != designs.end()) return designs[design];
        size_t can = 0;
        for (auto const& p : patterns)
            if (auto l = p.size(); p == design.substr(0, l)) can += rec(design.substr(l), rec);
        designs[design] = can;
        return can;
    };
    size_t cnt = 0;
    while (std::getline(ifs, line))
        cnt += rec(line, rec);
    return cnt;
}

int main(int argc, char* argv[])
{
    std::cout << "How many designs are possible?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What do you get if you add up the number of different ways you could make each design?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}