#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ctime>
#include "benchmarks.h"

#include <vector>
#include <algorithm>
#include <random>

// Вспомогательные функции для реализации TLBO
std::vector<double> meanAlongAxis(const std::vector<std::vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<double> means(cols, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            means[j] += matrix[i][j];
        }
    }
    for (int j = 0; j < cols; ++j) {
        means[j] /= rows;
    }
    return means;
}

std::vector<double> calculateDifference(const std::vector<double>& a, const std::vector<double>& b) {
    std::vector<double> diff(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        diff[i] = a[i] - b[i];
    }
    return diff;
}

std::vector<double> calculateDifferenceRand(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c) {
    std::vector<double> diff(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        diff[i] = c[i] + (a[i] - b[i]);
    }
    return diff;
}

int getRandomIndex(int max_val) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, max_val - 1);
    return dis(gen);
}

// Главная функция TLBO
std::pair<std::vector<double>, double> tlbo(int num_students, int max_iter,
                                            const std::vector<std::pair<double, double>>& search_space,
                                            double (*benchmark_function)(const std::vector<double>&)) {

    int num_features = search_space.size();

    // Инициализация студентов (population) в пределах заданного пространства поиска
    std::vector<std::vector<double>> students(num_students, std::vector<double>(num_features));
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < num_students; ++i) {
        for (int j = 0; j < num_features; ++j) {
            std::uniform_real_distribution<> dis(search_space[j].first, search_space[j].second);
            students[i][j] = dis(gen);
        }
    }

    std::vector<double> fitness(num_students);
    for (int i = 0; i < num_students; ++i) {
        fitness[i] = benchmark_function(students[i]);
    }

    for (int h = 0; h < max_iter; ++h) {
        // Стадия учителя
        int best_index = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
        std::vector<double> teacher = students[best_index];
        std::vector<double> students_mean = meanAlongAxis(students);

        for (int i = 0; i < num_students; i++) {
            if (i != best_index) {
                std::vector<double> difference = calculateDifference(teacher, students_mean);
                for (int j = 0; j < num_features; j++) {
                    difference[j] += students[i][j];
                }

                double new_score = benchmark_function(difference);
                if (new_score < fitness[i]) {
                    students[i] = difference;
                    fitness[i] = new_score;
                }
            }
        }

        // Стадия ученика
        for (int i = 0; i < num_students; i++) {
            int random_index_1 = getRandomIndex(num_students);
            int random_index_2 = getRandomIndex(num_students);
            while (random_index_1 == random_index_2) {
                random_index_2 = getRandomIndex(num_students);
            }

            std::vector<double> new_student;
            if (fitness[random_index_1] < fitness[random_index_2]) {
                new_student = calculateDifferenceRand(students[random_index_1], students[random_index_2], students[i]);
            } else {
                new_student = calculateDifferenceRand(students[random_index_2], students[random_index_1], students[i]);
            }

            double new_score = benchmark_function(new_student);
            if (new_score < fitness[i]) {
                students[i] = new_student;
                fitness[i] = new_score;
            }
        }
    }

    int best_index = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
    return {students[best_index], fitness[best_index]};
}
