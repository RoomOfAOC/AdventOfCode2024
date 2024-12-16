//
// Created by Harold on 2024/12/16.
//

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <iostream>

#include "../utils/mat.h"
#include "../utils/debug.h"

using map_t = Mat<char>;

void parse_map_and_start_end(std::ifstream& ifs, Mat<char>& mat, Point& start, Point& end)
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
            if (c == 'S')
            {
                start.x = rows;
                start.y = cols;
            }
            else if (c == 'E')
            {
                end.x = rows;
                end.y = cols;
            }
            cols++;
        }
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);
}

struct state_t
{
    Point pos{};
    Point dir{};
    size_t score{};

    bool operator==(state_t const& other) const { return pos == other.pos && dir == other.dir; }

    bool operator<(state_t const& other) const { return score > other.score; }

    struct hasher
    {
        size_t operator()(state_t const& s) const { return Point::PointHasher()(s.pos) ^ Point::PointHasher()(s.dir); }
    };
};

std::ostream& operator<<(std::ostream& os, state_t const& s)
{
    return os << "{" << s.pos << "; " << s.dir << "; " << s.score << "}";
}

Point get_next_dir(Point const& dir, bool cw = true)
{
    static std::vector<Point> cw_dirs{Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0), Point(0, 1)};
    static std::vector<Point> ccw_dirs{Point(0, 1), Point(-1, 0), Point(0, -1), Point(1, 0), Point(0, 1)};

    auto it =
        cw ? std::find(cw_dirs.cbegin(), cw_dirs.cend(), dir) : std::find(ccw_dirs.cbegin(), ccw_dirs.cend(), dir);
    std::advance(it, 1);
    //std::cout << std::boolalpha << cw << ", " << dir << ", dir: " << *it << '\n';
    return *it;
}

std::vector<Point> get_next_dirs(Point const& dir)
{
    std::vector<Point> dirs;
    dirs.reserve(3);
    dirs.push_back(dir);
    if (dir == Point(0, 1) || dir == Point(0, -1))
    {
        dirs.push_back(Point(-1, 0));
        dirs.push_back(Point(1, 0));
    }
    else if (dir == Point(1, 0) || dir == Point(-1, 0))
    {
        dirs.push_back(Point(0, -1));
        dirs.push_back(Point(0, 1));
    }
    return dirs;
}

std::vector<state_t> get_next_states(state_t const& state, map_t const& m)
{
    std::vector<state_t> next_states;
    next_states.reserve(3);
    for (auto dir : get_next_dirs(state.dir))
    {
        auto nxt = state.pos + dir;
        if (m.valid_pos(nxt) && m(nxt) != '#')
            next_states.push_back(state_t{nxt, dir, state.score + ((dir == state.dir) ? size_t{1} : size_t{1001})});
    }
    return next_states;
}

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    Point start, end;
    parse_map_and_start_end(ifs, m, start, end);
    //std::cout << m << '\n' << start << ", " << end << '\n';

    state_t init_state{start, {0, 1}, 0};
    std::unordered_set<state_t, state_t::hasher> visited;
    std::priority_queue<state_t> pq;
    pq.push(init_state);
    while (!pq.empty())
    {
        auto state = pq.top();
        pq.pop();

        //std::cout << state << '\n';

        if (state.pos == end) return state.score;

        if (visited.find(state) != visited.end()) continue;

        visited.insert(state);

        for (auto const& new_state : get_next_states(state, m))
            if (visited.find(new_state) == visited.end()) pq.push(new_state);
    }

    return 0;
}

struct state_t_2
{
    state_t state;

    std::vector<Point> path;

    bool operator<(state_t_2 const& other) const { return state.score > other.state.score; }
};

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    map_t m;
    Point start, end;
    parse_map_and_start_end(ifs, m, start, end);
    //std::cout << m << '\n' << start << ", " << end << '\n';

    state_t_2 init_state{state_t{start, {0, 1}, 0}, {start}};
    std::unordered_map<state_t, size_t, state_t::hasher> visited;
    std::priority_queue<state_t_2> pq;
    pq.push(init_state);
    std::vector<std::pair<std::vector<Point>, size_t>> paths;
    while (!pq.empty())
    {
        auto state = pq.top();
        pq.pop();

        if (state.state.pos == end)
        {
            paths.push_back(std::make_pair(state.path, state.state.score));
            continue;
        }

        if (visited.find(state.state) != visited.end() && visited[state.state] < state.state.score) continue;
        visited[state.state] = state.state.score;

        for (auto const& new_state : get_next_states(state.state, m))
        {
            auto ns_2 = state;
            ns_2.state = new_state;
            ns_2.path.push_back(new_state.pos);
            pq.push(ns_2);
        }
    }
    //std::cout << paths << '\n';
    auto min_score = std::min_element(paths.cbegin(), paths.cend(), [](auto const& p1, auto const& p2) {
                         return p1.second < p2.second;
                     })->second;
    std::unordered_set<Point, Point::PointHasher> titles;
    for (auto const& [p, s] : paths)
        if (s == min_score)
            for (auto const& pt : p)
                titles.insert(pt);
    return titles.size();
}

int main(int argc, char* argv[])
{
    std::cout << "What is the lowest score a Reindeer could possibly get?\n" << part_1(argv[1]) << std::endl;
    std::cout << "How many tiles are part of at least one of the best paths through the maze?\n"
              << part_2(argv[1]) << std::endl;

    return 0;
}