#pragma once
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <numeric>
#include <string>
#include <vector>

#include "config.hpp"
#include "helpers/eca.hpp"
#include "helpers/helpers.hpp"
using json = nlohmann::json;

struct EvaluationResult {
    std::string initial_config;
    std::vector<int> final_config;
    double initial_density;
    bool converged_to_empty;
    bool converged_to_full;
    bool correct;
    int iterations;
};

inline EvaluationResult evaluate_eca(ElementaryCellularAutomaton& eca, int m, double d) {
    EvaluationResult result;
    result.initial_config = configuration_to_string(eca.get_configuration());
    result.initial_density = density(string_to_configuration(result.initial_config));

    result.converged_to_empty = false;
    result.converged_to_full = false;
    result.correct = false;
    result.iterations = 0;

    for (int t = 0; t < m; ++t) {
        if (is_all_zeros(eca)) {
            result.converged_to_empty = true;
            break;
        }
        if (is_all_ones(eca)) {
            result.converged_to_full = true;
            break;
        }
        eca.step();
        result.iterations++;
    }

    result.final_config = eca.get_configuration();

    // Evaluate correctness
    if (result.initial_density < d)
        result.correct = result.converged_to_empty;
    else
        result.correct = result.converged_to_full;

    return result;
}

// ------------------------------------------------------------------------------------------------

class ECAEvaluator {
   public:
    ECAEvaluator(ElementaryCellularAutomaton& eca, const std::string& name)
        : eca(eca), name(name) {}

    void run_all() {
        std::filesystem::create_directories(OUTPUT_FOLDER);
        results.clear();

        int correct_count = 0;
        int total_converged_time = 0;
        int converged_count = 0;

        for (int i = 0; i < S; ++i) {
            // Generate and record initial config
            auto initial = random_configuration(N, i * 9876 + 42);
            double dens = density(initial);
            eca.set_configuration(initial);

            auto res = evaluate_eca(eca, M, D);
            res.initial_density = dens;
            res.initial_config = configuration_to_string(initial);
            results.push_back(res);

            if (res.correct) correct_count++;
            if (res.converged_to_empty || res.converged_to_full) {
                total_converged_time += res.iterations;
                converged_count++;
            }
        }

        accuracy = static_cast<double>(correct_count) / S;
        mean_time =
            converged_count > 0 ? static_cast<double>(total_converged_time) / converged_count : M;

        save_json();
    }

   private:
    ElementaryCellularAutomaton& eca;
    std::string name;
    std::vector<EvaluationResult> results;
    double accuracy = 0.0;
    double mean_time = 0.0;

    void save_json() const {
        json j;
        j["eca_name"] = name;
        j["encoded_rule"] = encode_rule(*eca.get_rule());
        j["metrics"] = {{"accuracy", accuracy}, {"mean_convergence_time", mean_time}};

        nlohmann::json trials = nlohmann::json::array();
        for (const auto& r : results) {
            trials.push_back({{"initial_configuration", r.initial_config},
                              {"initial_density", r.initial_density},
                              {"iterations", r.iterations},
                              {"converged_to_empty", r.converged_to_empty},
                              {"converged_to_full", r.converged_to_full},
                              {"correct", r.correct}});
        }
        j["trials"] = trials;

        std::filesystem::path out_path = std::filesystem::path(OUTPUT_FOLDER) / (name + ".json");

        std::ofstream out(out_path);
        out << std::setw(4) << j;
    }
};
