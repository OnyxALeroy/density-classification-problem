#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../config.hpp"
#include "local_rule.hpp"

class ElementaryCellularAutomaton {
   public:
    static constexpr int NEIGHBORHOOD = V;  // fixed neighborhood size

    ElementaryCellularAutomaton(int n = N) : n(n), config(n, 0), next_config(n, 0), rule(nullptr) {}

    // Set the initial configuration
    void set_configuration(const std::vector<int>& initial) {
        if ((int)initial.size() != n)
            throw std::invalid_argument("Initial configuration size mismatch");
        for (int x : initial) {
            if (x != 0 && x != 1)
                throw std::invalid_argument("Configuration values must be 0 or 1");
        }
        config = initial;
    }

    // Set the local rule (any object derived from BaseLocalRule)
    void set_rule(const BaseLocalRule* r) {
        if (r->size() != NEIGHBORHOOD)
            throw std::invalid_argument("Local rule size must match neighborhood size");
        rule = r;
    }

    // Apply one step of the automaton
    void step() {
        if (!rule) throw std::runtime_error("Local rule not set");

        for (int i = 0; i < n; ++i) {
            std::vector<int> neighborhood = get_neighborhood(i);
            next_config[i] = rule->apply(neighborhood);
        }
        config.swap(next_config);
    }

    // Access current configuration
    const std::vector<int>& get_configuration() const { return config; }

    // Print configuration to stdout
    void print() const {
        for (int x : config) std::cout << (x ? '1' : '0');
        std::cout << "\n";
    }

    // Return a pointer to the current rule (can be nullptr if not set)
    const BaseLocalRule* get_rule() const { return rule; }

   private:
    int n;
    std::vector<int> config;
    std::vector<int> next_config;
    const BaseLocalRule* rule;

    // Return the 7-cell neighborhood around position i (cyclic)
    std::vector<int> get_neighborhood(int i) const {
        std::vector<int> neighborhood(NEIGHBORHOOD);
        int half = NEIGHBORHOOD / 2;
        for (int d = -half; d <= half; ++d) {
            int pos = (i + d + n) % n;  // wrap-around; FIXME: correct this if needed
            neighborhood[d + half] = config[pos];
        }
        return neighborhood;
    }
};
