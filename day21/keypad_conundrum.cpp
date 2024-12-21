//
// Created by Harold on 2024/12/21.
//

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "../utils/point2d.h"
#include "../utils/str_op.h"
#include "../utils/debug.h"

using point_t = Point2D<int>;

using map_t = std::unordered_map<char, point_t>;

map_t pad{
    // num pad
    {'7', {0, 0}},
    {'8', {0, 1}},
    {'9', {0, 2}},
    {'4', {1, 0}},
    {'5', {1, 1}},
    {'6', {1, 2}},
    {'1', {2, 0}},
    {'2', {2, 1}},
    {'3', {2, 2}},
    {'0', {3, 1}},
    {'A', {3, 2}},
    // key pad
    {'^', {0, 1}},
    {'@', {0, 2}},
    {'<', {1, 0}},
    {'v', {1, 1}},
    {'>', {1, 2}},
};

map_t dirs{
    {'^', {-1, 0}},
    {'<', {0, -1}},
    {'v', {1, 0}},
    {'>', {0, 1}},
};

std::vector<std::string> move(point_t from, point_t to, point_t invalid_pos)
{
    std::string moves;
    auto [dx, dy] = to - from;
    moves += std::string(abs(dx), (dx < 0) ? '^' : 'v');
    moves += std::string(abs(dy), (dy < 0) ? '<' : '>');
    std::sort(moves.begin(), moves.end());
    std::unordered_set<std::string> potential_seqs;
    do
    {
        potential_seqs.insert(moves);
    } while (std::next_permutation(moves.begin(), moves.end()));
    std::vector<std::string> seqs;
    seqs.reserve(potential_seqs.size());
    for (auto const& seq : potential_seqs)
    {
        bool valid = true;
        auto cur = from;
        for (auto m : seq)
            if (auto nxt = cur + dirs[m]; nxt == invalid_pos)
            {
                valid = false;
                break;
            }
            else
                cur = nxt;
        if (valid) seqs.push_back(seq + '@');
    }
    if (seqs.empty()) seqs.push_back("@");
    return seqs;
}

struct state_t
{
    std::string seq;
    int robot_idx;

    bool operator==(state_t const& other) const { return seq == other.seq && robot_idx == other.robot_idx; }

    struct hasher
    {
        size_t operator()(state_t const& s) const
        {
            return std::hash<std::string>()(s.seq) ^ (std::hash<int>()(s.robot_idx) << 1);
        }
    };
};

using state_map_t = std::unordered_map<state_t, size_t, state_t::hasher>;

size_t traverse(std::string code, state_t state, state_map_t& state_map, int const robots = 2)
{
    if (state_map.find(state) != state_map.end()) return state_map[state];

    point_t invalid_pos, cur;
    if (state.robot_idx == 0)
    {
        invalid_pos = point_t(3, 0);
        cur = pad['A'];
    }
    else
    {
        invalid_pos = point_t(0, 0);
        cur = pad['@'];
    }

    size_t cnt = 0;
    for (auto c : code)
    {
        auto nxt = pad[c];
        auto seqs = move(cur, nxt, invalid_pos);
        //std::cout << cur << ", " << nxt << ", " << invalid_pos << ", " << code << ", " << c << ", " << seqs << '\n';
        if (state.robot_idx == robots)
            cnt += std::min_element(seqs.cbegin(), seqs.cend(), [](auto const& seq1, auto const& seq2) {
                       return seq1.size() < seq2.size();
                   })->size();
        else
        {
            size_t min = std::numeric_limits<size_t>::max();
            for (auto const& seq : seqs)
                min = std::min(min, traverse(seq, state_t{seq, state.robot_idx + 1}, state_map, robots));
            cnt += min;
        }
        cur = nxt;
    }
    state_map[state] = cnt;
    return cnt;
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    size_t complexity = 0;
    state_map_t sm;
    while (std::getline(ifs, line))
    {
        auto n = parse_num<size_t>(line.substr(0, 3));
        auto l = traverse(line, state_t{line, 0}, sm);
        //std::cout << n << ", " << l << '\n';
        complexity += n * l;
    }
    return complexity;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    size_t complexity = 0;
    state_map_t sm;
    while (std::getline(ifs, line))
    {
        auto n = parse_num<size_t>(line.substr(0, 3));
        auto l = traverse(line, state_t{line, 0}, sm, 25);
        complexity += n * l;
    }
    return complexity;
}

int main(int argc, char* argv[])
{
    std::cout << "What is the sum of the complexities of the five codes on your list?\n"
              << part_1(argv[1]) << std::endl;
    std::cout << "What is the sum of the complexities of the five codes on your list?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}