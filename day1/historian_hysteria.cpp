//
// Created by Harold on 2024/12/1.
//

#include <fstream>
#include <string>
#include <string_view>
#include <set>
#include <numeric>
#include <iostream>

#include "../utils/str_op.h"

long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::multiset<int> ll, rr;
    while (std::getline(ifs, line))
    {
        auto numbers = split(line);
        ll.insert(parse_num<int>(numbers[0]));
        rr.insert(parse_num<int>(numbers[1]));
    }
    long distance = 0;
    for (auto i(ll.cbegin()), j(rr.cbegin()); i != ll.cend() && j != rr.cend(); i++, j++)
        distance += std::abs((*i) - (*j));
    return distance;
}

long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<int> ll;
    std::multiset<int> rr;
    while (std::getline(ifs, line))
    {
        auto numbers = split(line);
        ll.push_back(parse_num<int>(numbers[0]));
        rr.insert(parse_num<int>(numbers[1]));
    }
    return std::reduce(ll.cbegin(), ll.cend(), 0L,
                       [&rr](long score, int i) { return score + i * static_cast<long>(rr.count(i)); });
}

int main(int argc, char* argv[])
{
    std::cout << "What is the total distance between your lists?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What is their similarity score?\n" << part_2(argv[1]) << std::endl;

    return 0;
}