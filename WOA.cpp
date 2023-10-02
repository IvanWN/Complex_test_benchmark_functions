#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "benchmarks.h"

std::pair<std::vector<double>, double> woa(int num_agents, int max_iter, const std::vector<std::pair<double, double>>& search_space,
                                           double (*benchmark_function)(const std::vector<double>&)) {
    int num_features = search_space.size();
    std::vector<std::vector<double>> agents(num_agents, std::vector<double>(num_features));

    for(int i = 0; i < num_agents; i++) {
        for(int j = 0; j < num_features; j++) {
            agents[i][j] = getRandomValue(search_space[j].first, search_space[j].second);
        }
    }

    std::vector<double> fitness(num_agents, std::numeric_limits<double>::infinity());

    for(int i = 0; i < num_agents; i++) {
        fitness[i] = benchmark_function(agents[i]);
    }

    for (int t = 0; t < max_iter; t++) {
        double a = 2.0 - t * ((2.0) / max_iter);

        for(int i = 0; i < num_agents; i++) {
            double r1 = getRandomValue(0, 1);
            double r2 = getRandomValue(0, 1);

            double A = 2.0 * a * r1 - a;
            double C = 2.0 * r2;

            double b = 1;
            double l = (getRandomValue(0, 1) * 2) - 1;

            double p = getRandomValue(0, 1);

            std::vector<double> X_rand = agents[getRandomValue(0, num_agents - 1)];

            std::vector<double> D_X_rand(num_features);
            std::vector<double> X_new(num_features);

            if(p < 0.5) {
                if(std::fabs(A) < 1) {
                    for(int j = 0; j < num_features; j++) {
                        D_X_rand[j] = std::fabs(C * X_rand[j] - agents[i][j]);
                        X_new[j] = X_rand[j] - A * D_X_rand[j];
                    }
                } else {
                    for(int j = 0; j < num_features; j++) {
                        X_new[j] = X_rand[j] - A * std::fabs(C * X_rand[j] - agents[i][j]);
                    }
                }
            } else {
                for(int j = 0; j < num_features; j++) {
                    D_X_rand[j] = std::fabs(X_rand[j] - agents[i][j]);
                    X_new[j] = D_X_rand[j] * std::exp(b * l) * std::cos(2 * M_PI * l) + X_rand[j];
                }
            }

            for(int j = 0; j < num_features; j++) {
                X_new[j] = std::clamp(X_new[j], search_space[j].first, search_space[j].second);
            }

            double new_fitness = benchmark_function(X_new);
            if(new_fitness < fitness[i]) {
                agents[i] = X_new;
                fitness[i] = new_fitness;
            }
        }
    }

    auto min_element_iter = std::min_element(fitness.begin(), fitness.end());
    int best_index = std::distance(fitness.begin(), min_element_iter);
    return {agents[best_index], *min_element_iter};
}
