# Benchmarking Metaheuristics

  This repository is a tool for creating a unified register of benchmarks and running metaheuristic functions on them, as well as displaying the results of their work. The task is to conduct a comparative analysis of the performance of metaheuristic algorithms on various benchmark functions.
  
- benchmarks/
  - benchmarks.cpp           # Файл с реализацией бенчмарк функций
  - benchmarks.h
- metaheuristics/
  - SSA.cpp                  # Файл с реализацией метаэвристики SSA
  - ICA.cpp                  # Файл с реализацией метаэвристики ICA
  - SCA.cpp                  # Файл с реализацией метаэвристики SCA
  - TLBO.cpp                  # Файл с реализацией метаэвристики TLBO
  - AOA.cpp                  # Файл с реализацией метаэвристики AOA
- main.cpp                   # Основной файл для запуска метаэвристик на бенчмарках
- README.md                  # Этот файл

Benchmark Functions

  There is a file in the benchmarks folder benchmarks.cpp , which contains implementations of various benchmark functions. In total, 12 benchmark functions are implemented here. Functions provide an interface for calculating their values and can be used to evaluate the performance of metaheuristic algorithms.

Metaheuristics

  The metaheuristics folders contain files SSA.cpp and ICA.cpp , in which the metaheuristics SSA (Social Spider Algorithm) and ICA (Imperialist Competitive Algorithm) are implemented, respectively. Each metaheuristic contains its own parameters for the run and calculates the mean and standard deviation (stddev) inside its own implementation.

Main File

  File main.cpp represents the entry point to the program. This is where metaheuristic algorithms are called to run on benchmark functions. Parameters for running metaheuristics are set inside the metaheuristics themselves, not in main.cpp . Thus, each metaheuristic has its own fixed parameters.
