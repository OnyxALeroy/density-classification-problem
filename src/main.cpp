#include <iostream>

#include "ECAEvaluator.hpp"
#include "helpers/eca.hpp"
#include "helpers/local_rule.hpp"

int main() {
    try {
        // Example 1: simple parity rule
        LocalRule parity_rule(V, [](const std::vector<int>& n) {
            int sum = 0;
            for (int x : n) sum += x;
            return sum % 2;  // even → 0, odd → 1
        });

        ElementaryCellularAutomaton eca1(N);
        eca1.set_rule(&parity_rule);

        ECAEvaluator eval1(eca1, "ParityRule");
        eval1.run_all();
        std::cout << "✅ ParityRule evaluation done.\n";

        // Example 2: majority rule
        LocalRule majority_rule(V, [](const std::vector<int>& n) { return 0; });

        ElementaryCellularAutomaton eca2(N);
        eca2.set_rule(&majority_rule);

        ECAEvaluator eval2(eca2, "MajorityRule");
        eval2.run_all();
        std::cout << "✅ MajorityRule evaluation done.\n";

        std::cout << "\nAll results have been saved to the output folder: " << OUTPUT_FOLDER
                  << "\n";
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
