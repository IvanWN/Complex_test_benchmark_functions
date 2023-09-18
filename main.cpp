#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <string>

// Including files
#include "SSA.cpp"
#include "ICA.cpp"
#include "benchmarks.h"


using namespace std;

double mean(const std::vector<double>& values) {
    return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
}

double stddev(const std::vector<double>& values, double mean_val) {
    double variance = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double acc, double val) {
        return acc + (val - mean_val) * (val - mean_val);
    }) / values.size();
    return std::sqrt(variance);
}

template <typename AlgorithmFunc>
void run_and_evaluate(const string& algorithm_name, AlgorithmFunc algorithm, int num_runs, const std::vector<std::pair<double, double>>& search_space) {
    std::vector<double (*)(const std::vector<double>&)> benchmark_functions = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12};
    std::vector<std::string> function_names = {"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12"};

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

        double mean_fitness = mean(fitness_results);
        double fitness_std_dev = stddev(fitness_results, mean_fitness);
        double mean_run_time = mean(run_times);

        // Display results for each benchmark function
        std::cout << "Algorithm: " << algorithm_name << " | Benchmark function: " << function_names[func_idx] << std::endl;
        std::cout << "Average fitness: " << mean_fitness << std::endl;
        std::cout << "Standard deviation: " << fitness_std_dev << std::endl;
        std::cout << "Average run time: " << mean_run_time << " sec" << std::endl;
        std::cout << "---------------------------------------------\n";
        std::cout << std::endl;
    }
}

int main() {
    const int num_runs = 30;
    std::vector<std::pair<double, double>> search_space(20, {-10.0, 10.0});  // For now using a sample dimension of 20 for all functions

    run_and_evaluate("SSA", [&search_space](auto benchmark_function) {
        return ssa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);

    run_and_evaluate("ICA", [&search_space](auto benchmark_function) {
        return ica(100, 3000, search_space, 10, benchmark_function);  // 3 is a placeholder for num_empires, you can change it.
    }, num_runs, search_space);

    return 0;
}
