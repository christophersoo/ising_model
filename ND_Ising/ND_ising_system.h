#ifndef ND_ISING_SYSTEM_H
#define ND_ISING_SYSTEM_H

// List of libraries

#include <iostream>
#include <fstream>
#include <random>
#include <vector>

// System class definition

class ND_System{
    public:
        // Constructor
        ND_System(int N, int DIM);

        // Class Methods
        void init();
        long long pow_int(int base, int exp);
        long long flatten(const long long idx_arr[]);
        inline void unflatten(long long idx, long long idx_arr[]);
        long long energy_tot();
        long long magnet_tot();
        void MC_SWEEP(float beta);

    private:
        // Class Variables
        std::vector<int> state;
        long long LEN;
        int N;
        int DIM;

        // Pseudo-Random Number Generators
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<long long> dist;
        std::uniform_real_distribution<double> prob;
};

#endif