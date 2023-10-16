#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <iomanip>
#include "SSA2.0.cpp"
#include "benchmarks.h"
#include <fstream>

using namespace std;

struct BenchmarkConfig {
    double (*benchmark_func)(const std::vector<double>&);
    std::vector<std::pair<double, double>> search_space;
};

template <typename AlgorithmFunc>
vector<vector<double>> run_and_evaluate(const string& algorithm_name, AlgorithmFunc algorithm, int num_runs, const std::vector<BenchmarkConfig>& configs) {
    std::vector<vector<double>> all_results(configs.size(), vector<double>(3, 0.0));

    double (*mean_function)(const std::vector<double>&) = [](const std::vector<double>& values) {
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    };
    double (*stddev_function)(const std::vector<double>&, double) = [](const std::vector<double>& values, double mean_val) {
        double variance = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double acc, double val) {
            return acc + (val - mean_val) * (val - mean_val);
        }) / values.size();
        return std::sqrt(variance);
    };

    for (size_t config_idx = 0; config_idx < configs.size(); ++config_idx) {
        auto& benchmark_function = configs[config_idx].benchmark_func;
        auto& search_space = configs[config_idx].search_space;

        std::vector<double> fitness_results;
        std::vector<double> run_times;

        for (int i = 0; i < num_runs; ++i) {
            auto start_time = std::chrono::high_resolution_clock::now();
            auto [best_solution, fitness] = algorithm(benchmark_function, search_space);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> run_time = end_time - start_time;

            fitness_results.push_back(fitness);
            run_times.push_back(run_time.count());
        }
        all_results[config_idx][0] = mean_function(fitness_results);
        all_results[config_idx][1] = stddev_function(fitness_results, all_results[config_idx][0]);
        all_results[config_idx][2] = mean_function(run_times);
    }
    return all_results;
}

std::pair<std::vector<double>, double> ssa_with_logging(int num_salps, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                                        double (*benchmark_function)(const std::vector<double>&), int benchmark_number) {
    auto [solution, fitness] = ssa(num_salps, max_iter, search_space, benchmark_function, benchmark_number);
    ofstream out("results.txt", ios::app);
    out << "Final Solution: ";
    for (const auto& s : solution) {
        out << s << " ";
    }
    out << "\nFinal Fitness: " << fitness << "\n";
    out.close();
    return {solution, fitness};
}

int main() {
    const int num_runs = 30;

    std::vector<BenchmarkConfig> configs = {
            {f1, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f2, std::vector<std::pair<double, double>>(20, {-10.0, 10.0})},
            {f3, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f4, std::vector<std::pair<double, double>>(20, {-100.0, 100.0})},
            {f5, std::vector<std::pair<double, double>>(20, {-30,30})},
            {f6, std::vector<std::pair<double, double>>(20, {-10.0, 10.0})},
            {f7, std::vector<std::pair<double, double>>(20, {-1.28,1.28})},
            {f8, std::vector<std::pair<double, double>>(20, {-500,500})},
            {f9, std::vector<std::pair<double, double>>(20, {-5.12,5.12})},
            {f10, std::vector<std::pair<double, double>>(20, {-32,32})},
            {f11, std::vector<std::pair<double, double>>(20, {-600,600})},
            {f12, std::vector<std::pair<double, double>>(20, {-50,50})},
            {f13, std::vector<std::pair<double, double>>(20, {-50,50})},
            {f14, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f15, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f16, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f17, std::vector<std::pair<double, double>>(10, {-5,5})},
            {f18, std::vector<std::  pair<double, double>>(10, {-5,5})},
            {f19, std::vector<std::pair<double, double>>(10, {-5,5})},
    };

    ofstream main_out("results.txt", ios::app);
    for (size_t config_idx = 0; config_idx < configs.size(); ++config_idx) {
        auto& benchmark_function = configs[config_idx].benchmark_func;
        auto& search_space = configs[config_idx].search_space;

        main_out << "Testing on f" << config_idx + 1 << ":\n";
        auto [solution, fitness] = ssa_with_logging(100, 3000, search_space, benchmark_function, config_idx + 1);
        main_out << "\n";
    }
    main_out.close();
    return 0;
}
