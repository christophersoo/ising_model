#include <iostream>
#include "config.h"
#include <fstream>
#include <bitset>
#include <random>
using namespace std;

constexpr long long power(long long base, long long exp) {
    return (exp == 0) ? 1 : base * power(base, exp - 1);
}
constexpr long long LEN = power(N, DIM);

bitset<LEN> state;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<long long> dist(0, LEN - 1);
uniform_real_distribution<double> prob(0.0, 1.0);

long long flatten(long long idx_arr[]) {
    long long idx = 0, factor = 1;
    for (long long i = DIM - 1; i >= 0; i--) {
        idx += idx_arr[i] * factor;
        factor *= N;
    }
    return idx;
}

void unflatten(long long idx, long long idx_arr[]) {
    for (long long i = 0; i < DIM; i++) {
        idx_arr[i] = idx % N;
        idx /= N;
    }
}

long long energy_tot() {
    long long energy = 0;
    long long idx_arr[DIM];

    for (long long i = 0; i < LEN; i++) {
        unflatten(i, idx_arr);
        long long spin = 2 * state[i] - 1;
        long long sum_neighbors = 0;

        for (long long d = 0; d < DIM; d++) {
            idx_arr[d] = (idx_arr[d] + 1) % N;
            sum_neighbors += 2 * state[flatten(idx_arr)] - 1;

            idx_arr[d] = (idx_arr[d] - 2 + N) % N;
            sum_neighbors += 2 * state[flatten(idx_arr)] - 1;

            idx_arr[d] = (idx_arr[d] + 1) % N;
        }
        energy += -spin * sum_neighbors;
    }
    return energy / 2;
}

long long magnet_tot() {
    long long magnetisation = 0;
    for (long long i = 0; i < LEN; i++) {
        magnetisation += 2 * state[i] - 1;
    }
    return magnetisation;
}

void init() {
    for (long long i = 0; i < LEN; i++) {
        state[i] = dist(gen) % 2;
    }
}

void MC_SWEEP(double beta) {
    long long s_idxarr[DIM];
    long long s_idx = dist(gen);
    unflatten(s_idx, s_idxarr);
    long long sum_neighbors = 0;
    for (long long d = 0; d < DIM; d++) {
        s_idxarr[d] = (s_idxarr[d] + 1) % N;
        sum_neighbors += 2 * state[flatten(s_idxarr)] - 1;

        s_idxarr[d] = (s_idxarr[d] - 2 + N) % N;
        sum_neighbors += 2 * state[flatten(s_idxarr)] - 1;

        s_idxarr[d] = (s_idxarr[d] + 1) % N;
    }

    long long S_i = 2 * state[s_idx] - 1;
    long long delta_E = 2 * S_i * sum_neighbors;
    double p_e = exp(-delta_E * beta);
    if (prob(gen) < p_e) {
        state.flip(s_idx);
    }
}

int main() {
    init();
    long long avg_energy = 0;
    long long avg_magnet = 0;
    for (double beta = 0; beta < 5; beta=beta+0.1){
        ofstream file("confs/output" + to_string(beta * 10) + ".csv");
        file << "conf_num, energy_total, magnetisation" << endl;
        for (long long j = 0; j < CONFS; j++) {
            for (long long k = 0; k < SWEEP; k++) {
                MC_SWEEP(beta);
            }
            cout << "beta " << beta << ", conf" << j << " completed." << endl;
            file << j << ", " << energy_tot() << ", " << magnet_tot() << endl;
        }
        file.close();
    }
    cout << "Successfully created configurations." << endl;
    return 0;
}
