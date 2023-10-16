#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ctime>
#include "benchmarks.h"
#include <fstream>

// Salp Swarm Algorithm (SSA)
std::pair<std::vector<double>, double> ssa(int num_salps, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                           double (*benchmark_function)(const std::vector<double>&), int benchmark_number) {
    int num_dimensions = search_space.size();
    std::vector<std::vector<double>> salps(num_salps, std::vector<double>(num_dimensions));
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::ofstream log_file("results.txt", std::ios_base::app);

    // Initialize salp positions randomly within the search space
    for (int i = 0; i < num_salps; ++i) {
        for (int j = 0; j < num_dimensions; ++j) {
            salps[i][j] = search_space[j].first + (search_space[j].second - search_space[j].first) * static_cast<double>(std::rand()) / RAND_MAX;
        }
    }

    // Calculate fitness for each salp
    std::vector<double> fitness(num_salps);
    for (int i = 0; i < num_salps; ++i) {
        fitness[i] = benchmark_function(salps[i]);
    }

    for (int t = 0; t < max_iter; ++t) {
        // Get the best salp
        int best_index = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
        std::vector<double> best_salp = salps[best_index];

        // Update positions with adaptive parameter
        double w = 1.0 - (static_cast<double>(t) / max_iter);
        double best_current_fitness = *std::min_element(fitness.begin(), fitness.end());
        for (int i = 0; i < num_salps; ++i) {
            for (int j = 0; j < num_dimensions; ++j) {
                if (i == 0) {
                    salps[i][j] = best_salp[j]; // The first salp follows the lead
                } else {
                    // Subsequent salps follow their predecessor
                    salps[i][j] = (salps[i][j] + salps[i - 1][j]) / 2;

                    // Introduce randomization for the latter half of iterations
                    if (t > max_iter / 2) {
                        salps[i][j] += w * (2.0 * static_cast<double>(std::rand()) / RAND_MAX - 1.0); // random value in [-1,1]
                    }

                    // Boundary check
                    if (salps[i][j] < search_space[j].first) {
                        salps[i][j] = search_space[j].first;
                    } else if (salps[i][j] > search_space[j].second) {
                        salps[i][j] = search_space[j].second;
                    }
                }
                fitness[i] = benchmark_function(salps[i]);
            }

            double current_best_fitness = *std::min_element(fitness.begin(), fitness.end());
            std::cout << "\rBenchmark " << benchmark_number << ", Iteration " << t << ": Best Fitness = " << std::setprecision(5) << current_best_fitness << std::flush;
            log_file << "Benchmark " << benchmark_number << ", Iteration " << t << ": Best Fitness = " << std::setprecision(5) << current_best_fitness << std::endl;

            if (*std::min_element(fitness.begin(), fitness.end()) < 1e-9) {
                break;
            }
        }

        // Update fitness values
        for (int i = 0; i < num_salps; ++i) {
            fitness[i] = benchmark_function(salps[i]);
        }

        if (*std::min_element(fitness.begin(), fitness.end()) < 1e-9) {
            break;
        }

    }

    log_file.close();
    int best_index = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
    return {salps[best_index], fitness[best_index]};
}
