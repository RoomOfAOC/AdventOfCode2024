//
// Created by Harold on 2024/12/15.
//

#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<char>;

void read_map_and_seq_and_robot_pos(std::ifstream& ifs, Mat<char>& mat, std::string& seq, Point& pos)
{
    std::string line;
    char c;
    int rows = 0, cols = 0;
    while (std::getline(ifs, line) && !line.empty())
    {
        cols = 0;
        for (auto c : line)
        {
            mat.data.push_back(c);
            if (c == '@')
            {
                pos.x = rows;
                pos.y = cols;
            }
            cols++;
        }
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);

    while (std::getline(ifs, line) && !line.empty())
        seq += line;
}

Point get_dir(char c)
{
    switch (c)
    {
    case '^':
        return Point(-1, 0);
    case 'v':
        return Point(1, 0);
    case '<':
        return Point(0, -1);
    case '>':
        return Point(0, 1);
    default:
        return {};
    }
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);

    map_t m;
    std::string seq;
    Point pos;
    read_map_and_seq_and_robot_pos(ifs, m, seq, pos);

    //std::cout << m << '\n' << seq << '\n' << pos << '\n';

    for (auto d : seq)
    {
        auto dir = get_dir(d);
        auto nxt = pos + dir;

        while (m(nxt) == 'O')
            nxt += dir;

        if (m(nxt) == '#') continue;

        m(nxt) = 'O';
        m(pos + dir) = '@';
        m(pos) = '.';
        pos += dir;
    }

    //std::cout << m << '\n';

    size_t sum = 0;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == 'O') sum += 100 * i + j;

    return sum;
}

void read_map_and_seq_and_robot_pos2(std::ifstream& ifs, Mat<char>& mat, std::string& seq, Point& pos)
{
    std::string line;
    char c;
    int rows = 0, cols = 0;
    while (std::getline(ifs, line) && !line.empty())
    {
        cols = 0;
        for (auto c : line)
        {
            if (c == '#' || c == '.')
            {
                mat.data.push_back(c);
                mat.data.push_back(c);
            }
            if (c == 'O')
            {
                mat.data.push_back('[');
                mat.data.push_back(']');
            }
            if (c == '@')
            {
                mat.data.push_back('@');
                mat.data.push_back('.');
                pos.x = rows;
                pos.y = cols;
            }
            cols += 2;
        }
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);

    while (std::getline(ifs, line) && !line.empty())
        seq += line;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);

    map_t m;
    std::string seq;
    Point pos;
    read_map_and_seq_and_robot_pos2(ifs, m, seq, pos);

    //std::cout << m << '\n' << seq << '\n' << pos << '\n';

    std::vector<Point> boxes{}, next_points{}, tmp{};

    for (auto d : seq)
    {
        auto dir = get_dir(d);
        auto nxt = pos + dir;
        if (dir.x == 0)
        {
            while (m(nxt) == '[' || m(nxt) == ']')
                nxt += dir;

            if (m(nxt) == '#') continue;

            for (auto j = nxt.y; j != pos.y + dir.y; j -= dir.y)
                m(nxt.x, j) = m(nxt.x, j - dir.y);
            m(pos) = '.';
            pos += dir;
            m(pos) = '@';

            //std::cout << m << '\n';
        }
        else
        {
            if (m(nxt) == '#') continue;

            if (m(nxt) == '.')
            {
                m(pos) = '.';
                m(nxt) = '@';
                pos += dir;
                continue;
            }

            boxes.clear();
            next_points.clear();
            next_points.push_back(nxt);
            bool move = true;
            while (!next_points.empty())
            {
                tmp.clear();

                for (auto const& n : next_points)
                {
                    if (!m.valid_pos(n)) continue;

                    if (m(n) == '#')
                    {
                        move = false;
                        tmp.clear();
                        break;
                    }
                    if (m(n) == '[')
                    {
                        boxes.push_back(n);
                        tmp.push_back(n + dir);
                        tmp.push_back(n + dir + Point(0, 1));
                    }
                    else if (m(n) == ']')
                    {
                        boxes.push_back(Point(n.x, n.y - 1));
                        tmp.push_back(n + dir);
                        tmp.push_back(n + dir + Point(0, -1));
                    }
                }

                std::swap(next_points, tmp);
            }

            if (!move) continue;

            for (auto it = boxes.rbegin(); it != boxes.rend(); it++)
            {
                auto b = *it;
                auto b1 = b + dir;
                m(b1) = '[';
                m(b1 + Point(0, 1)) = ']';
                m(b) = '.';
                m(b + Point(0, 1)) = '.';
            }

            m(pos) = '.';
            pos += dir;
            m(pos) = '@';

            //std::cout << m << '\n';
        }
    }

    //std::cout << m << '\n';

    size_t sum = 0;
    for (auto i = 0; i < m.rows; i++)
        for (auto j = 0; j < m.cols; j++)
            if (m(i, j) == '[') sum += 100 * i + j;

    return sum;
}

int main(int argc, char* argv[])
{
    std::cout << "what is the sum of all boxes' GPS coordinates?\n" << part_1(argv[1]) << std::endl;
    std::cout << "What is the sum of all boxes' final GPS coordinates?\n" << part_2(argv[1]) << std::endl;

    return 0;
}