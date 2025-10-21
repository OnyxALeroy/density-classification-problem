#pragma once
#include <functional>
#include <stdexcept>
#include <vector>

// Base interface for a local rule
class BaseLocalRule {
   public:
    explicit BaseLocalRule(int v) : v(v) {}

    virtual ~BaseLocalRule() = default;

    // Apply the rule to a vector of 0s and 1s
    int apply(const std::vector<int>& neighborhood) const { return evaluate(neighborhood); }

    int size() const { return v; }

   protected:
    int v;

    // Derived classes implement this
    virtual int evaluate(const std::vector<int>& neighborhood) const = 0;
};

class LocalRule : public BaseLocalRule {
   public:
    LocalRule(int v, std::function<int(const std::vector<int>&)> func)
        : BaseLocalRule(v), func(std::move(func)) {}

   protected:
    int evaluate(const std::vector<int>& neighborhood) const override { return func(neighborhood); }

   private:
    std::function<int(const std::vector<int>&)> func;
};

// Return the encoded rule output pattern as a binary string
inline std::string encode_rule(const BaseLocalRule& rule) {
    const int v = rule.size();
    const int combinations = 1 << v;
    std::string bits;
    bits.reserve(combinations);

    // Iterate over all binary inputs (from 0 to 2^v - 1)
    for (int mask = 0; mask < combinations; ++mask) {
        std::vector<int> neighborhood(v);
        for (int i = 0; i < v; ++i) neighborhood[v - 1 - i] = (mask >> i) & 1;

        int output = rule.apply(neighborhood);
        bits.push_back(output ? '1' : '0');
    }
    return bits;
}

/*
    You can then create a Local Rule like this:
        FunctionalLocalRule r(3, [](const std::vector<int>& n) {
            return (n[0] & n[1]) | n[2]; // some arbitrary logic
        });
*/