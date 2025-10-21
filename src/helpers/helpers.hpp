#pragma once
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

#include "eca.hpp"

inline std::string configuration_to_string(const std::vector<int>& config) {
    std::string s;
    s.reserve(config.size());
    for (int x : config) s.push_back(x ? '1' : '0');
    return s;
}

inline std::vector<int> string_to_configuration(const std::string& s) {
    std::vector<int> config;
    config.reserve(s.size());
    for (char c : s) {
        if (c == '0')
            config.push_back(0);
        else if (c == '1')
            config.push_back(1);
        else
            throw std::invalid_argument("String contains invalid character: must be '0' or '1'");
    }
    return config;
}

// ---------------------------
// Functions for std::vector<int>
// ---------------------------

inline bool is_all_zeros(const std::vector<int>& config) {
    for (int x : config)
        if (x != 0) return false;
    return true;
}

inline bool is_all_ones(const std::vector<int>& config) {
    for (int x : config)
        if (x != 1) return false;
    return true;
}

inline double density(const std::vector<int>& config) {
    if (config.empty()) throw std::invalid_argument("Configuration is empty");
    int sum = std::accumulate(config.begin(), config.end(), 0);
    return static_cast<double>(sum) / config.size();
}

// Generate a random configuration of size n (0s and 1s)
inline std::vector<int> random_configuration(int n, unsigned int seed = 0) {
    if (n <= 0) throw std::invalid_argument("Size must be positive");

    std::vector<int> config(n);
    std::mt19937 rng(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 1);

    for (int& x : config) x = dist(rng);
    return config;
}

// ---------------------------
// Overloads for ElementaryCellularAutomaton
// ---------------------------

inline bool is_all_zeros(const ElementaryCellularAutomaton& eca) {
    return is_all_zeros(eca.get_configuration());
}

inline bool is_all_ones(const ElementaryCellularAutomaton& eca) {
    return is_all_ones(eca.get_configuration());
}

inline double density(const ElementaryCellularAutomaton& eca) {
    return density(eca.get_configuration());
}

// Generate and set a random configuration directly in an ECA
inline void random_configuration(ElementaryCellularAutomaton& eca, unsigned int seed = 0) {
    int n = eca.get_configuration().size();
    auto config = random_configuration(n, seed);
    eca.set_configuration(config);
}
