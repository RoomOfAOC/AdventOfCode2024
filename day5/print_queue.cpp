//
// Created by Harold on 2024/12/5.
//

#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

#include "../utils/str_op.h"
#include "../utils/debug.h"

long part_1(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    std::string line;
    std::map<int, std::set<int>> q;
    std::vector<int> seq;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;
        if (line.size() == 5)
        {
            auto l = parse_num<int>(line.substr(0, 2));
            auto r = parse_num<int>(line.substr(3, 2));
            q[l].insert(r);
        }
        else
        {
            seq.clear();
            for (auto const& s : split(line, ","))
                seq.push_back(parse_num<int>(s));
            bool is_ordered = true;
            for (auto i = 0; i < seq.size(); i++)
                if (is_ordered)
                    for (auto j = i + 1; j < seq.size(); j++)
                        if (q[seq[i]].find(seq[j]) == q[seq[i]].end())
                        {
                            is_ordered = false;
                            break;
                        }
            if (is_ordered) res += seq[seq.size() / 2];
        }
    }
    //std::cout << q << '\n';

    return res;
}

long part_2(char const* fn)
{
    long res = 0;
    std::ifstream ifs(fn);
    std::string line;
    std::map<int, std::set<int>> q;
    std::vector<int> seq;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;
        if (line.size() == 5)
        {
            auto l = parse_num<int>(line.substr(0, 2));
            auto r = parse_num<int>(line.substr(3, 2));
            q[l].insert(r);
        }
        else
        {
            seq.clear();
            for (auto const& s : split(line, ","))
                seq.push_back(parse_num<int>(s));
            bool is_ordered = true;
            for (auto i = 0; i < seq.size(); i++)
                if (is_ordered)
                    for (auto j = i + 1; j < seq.size(); j++)
                        if (q[seq[i]].find(seq[j]) == q[seq[i]].end())
                        {
                            is_ordered = false;
                            break;
                        }
            if (!is_ordered)
            {
                std::sort(seq.begin(), seq.end(), [&q](int a, int b) { return (q[a].find(b) != q[a].end()); });
                res += seq[seq.size() / 2];
            }
        }
    }

    return res;
}

int main(int argc, char* argv[])
{
    std::cout << "What do you get if you add up the middle page number from those correctly-ordered updates?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "What do you get if you add up the middle page numbers after correctly ordering just those updates?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}