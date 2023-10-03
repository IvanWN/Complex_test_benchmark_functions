#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <fstream>
#include <iomanip>

// Including files
#include "SSA.cpp"
#include "ICA.cpp"
#include "TLBO.cpp"
#include "SCA.cpp"
#include "AOA.cpp"
#include "WOA.cpp"
#include "benchmarks.h"

using namespace std;

template <typename AlgorithmFunc>
vector<vector<double>> run_and_evaluate(const string& algorithm_name, AlgorithmFunc algorithm, int num_runs, const std::vector<std::pair<double, double>>& search_space) {
    std::vector<double (*)(const std::vector<double>&)> benchmark_functions = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13};
    std::vector<vector<double>> all_results(benchmark_functions.size(), vector<double>(3, 0.0));

    double (*mean_function)(const std::vector<double>&) = [](const std::vector<double>& values) {
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    };
    double (*stddev_function)(const std::vector<double>&, double) = [](const std::vector<double>& values, double mean_val) {
        double variance = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double acc, double val) {
            return acc + (val - mean_val) * (val - mean_val);
        }) / values.size();
        return std::sqrt(variance);
    };

    for (size_t func_idx = 0; func_idx < benchmark_functions.size(); ++func_idx) {
        auto& benchmark_function = benchmark_functions[func_idx];
        std::vector<double> fitness_results;
        std::vector<double> run_times;

        for (int i = 0; i < num_runs; ++i) {
            auto start_time = std::chrono::high_resolution_clock::now();
            auto [best_solution, fitness] = algorithm(benchmark_function);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> run_time = end_time - start_time;

            fitness_results.push_back(fitness);
            run_times.push_back(run_time.count());
        }
        all_results[func_idx][0] = mean_function(fitness_results);
        all_results[func_idx][1] = stddev_function(fitness_results, all_results[func_idx][0]);
        all_results[func_idx][2] = mean_function(run_times);
    }
    return all_results;
}

int main() {
    const int num_runs = 30;
    std::vector<std::pair<double, double>> search_space(20, {-10.0, 10.0});

    std::vector<std::string> algorithm_names = {"WOA", "AOA", "SSA", "ICA", "TLBO", "SCA"};
    std::vector<std::vector<vector<double>>> all_algorithm_results;

    all_algorithm_results.push_back(run_and_evaluate("WOA", [&search_space](auto benchmark_function) {
        return woa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space));

    all_algorithm_results.push_back(run_and_evaluate("AOA", [&search_space](auto benchmark_function) {
        return aoa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space));

    all_algorithm_results.push_back(run_and_evaluate("SSA", [&search_space](auto benchmark_function) {
        return ssa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space));

    all_algorithm_results.push_back(run_and_evaluate("ICA", [&search_space](auto benchmark_function) {
        return ica(100, 3000, search_space, 10, benchmark_function); // 10 - это пример значения для num_empires
    }, num_runs, search_space));

    all_algorithm_results.push_back(run_and_evaluate("TLBO", [&search_space](auto benchmark_function) {
        return tlbo(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space));

    all_algorithm_results.push_back(run_and_evaluate("SCA", [&search_space](auto benchmark_function) {
        return sca(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space));

    std::ofstream outfile("results.txt");

    if (!outfile.is_open()) {
        std::cerr << "Error opening results.txt for writing" << std::endl;
        return 1;
    }

    outfile << std::fixed << std::setprecision(2);
    for (const auto& name : algorithm_names)
        outfile << std::setw(15) << name;
    outfile << '\n';

    for (size_t func_idx = 0; func_idx < 13; ++func_idx) {
        outfile << "f" << func_idx + 1 << ": ";
        for (const auto& alg_results : all_algorithm_results) {
            for (const auto& result : alg_results[func_idx])
                outfile << std::setw(15) << result;
        }
        outfile << '\n';
    }

    outfile.close();

    return 0;
}
