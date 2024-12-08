#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

template <typename T> std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
{
    os << "[ ";
    for (auto const& e : v)
        os << e << " ";
    return os << "]";
}

template <typename K, typename V> std::ostream& operator<<(std::ostream& os, std::map<K, V> const& m)
{
    os << "{\n";
    for (auto const& [k, v] : m)
        os << "(" << k << ", " << v << ")\n";
    return os << "}";
}

template <typename T> std::ostream& operator<<(std::ostream& os, std::set<T> const& v)
{
    os << "{ ";
    for (auto const& e : v)
        os << e << " ";
    return os << "}";
}

template <typename K, typename V, typename H = std::hash<K>>
std::ostream& operator<<(std::ostream& os, std::unordered_map<K, V, H> const& m)
{
    os << "{\n";
    for (auto const& [k, v] : m)
        os << "(" << k << ", " << v << ")\n";
    return os << "}";
}

template <typename T, typename H = std::hash<K>>
std::ostream& operator<<(std::ostream& os, std::unordered_set<T, H> const& v)
{
    os << "{ ";
    for (auto const& e : v)
        os << e << " ";
    return os << "}";
}