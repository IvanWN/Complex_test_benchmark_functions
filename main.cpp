#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <string>

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
void run_and_evaluate(const string& algorithm_name, AlgorithmFunc algorithm, int num_runs, const std::vector<std::pair<double, double>>& search_space) {
    std::vector<double (*)(const std::vector<double>&)> benchmark_functions = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12};
    std::vector<std::string> function_names = {"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12"};

    double (*mean_function)(const std::vector<double>&);
    double (*stddev_function)(const std::vector<double>&, double);

    // Select appropriate functions based on algorithm_name
    if (algorithm_name == "ICA") {
        mean_function = [](const std::vector<double>& vec) {
            return std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
        };
        stddev_function = [](const std::vector<double>& vec, double mean_val) {
            double sq_sum = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0);
            double sq_mean = sq_sum / vec.size();
            return std::sqrt(sq_mean - mean_val * mean_val);
        };
    } else {
        mean_function = [](const std::vector<double>& values) {
            return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        };
        stddev_function = [](const std::vector<double>& values, double mean_val) {
            double variance = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double acc, double val) {
                return acc + (val - mean_val) * (val - mean_val);
            }) / values.size();
            return std::sqrt(variance);
        };
    }

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

        double mean_fitness = mean_function(fitness_results);
        double fitness_std_dev = stddev_function(fitness_results, mean_fitness);
        double mean_run_time = mean_function(run_times);

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
    std::vector<std::pair<double, double>> search_space(20, {-10.0, 10.0});

    run_and_evaluate("SSA", [&search_space](auto benchmark_function) {
        return ssa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);

    run_and_evaluate("ICA", [&search_space](auto benchmark_function) {
        return ica(100, 3000, search_space, 10, benchmark_function);
    }, num_runs, search_space);

    run_and_evaluate("TLBO", [&search_space](auto benchmark_function) {
        return tlbo(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);

    run_and_evaluate("SCA", [&search_space](auto benchmark_function) {
        return sca(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);

    run_and_evaluate("AOA", [&search_space](auto benchmark_function) {
        return aoa(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);
    
    run_and_evaluate("WOA", [&search_space](auto benchmark_function) {
        return tlbo(100, 3000, search_space, benchmark_function);
    }, num_runs, search_space);

    return 0;
}
