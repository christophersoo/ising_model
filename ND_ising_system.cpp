#include "ND_ising_system.h"

ND_System::ND_System(int N, int DIM, long long LEN)
    : N(N), DIM(DIM), LEN(LEN), state(LEN), 
    gen(rd()), dist(0, LEN - 1), prob(0.0, 1.0) {
    this->init();
}

void ND_System::init(){
    for (long long i = 0; i < LEN; i++) {
        state[i] = dist(gen) % 2;
    }
}


inline long long ND_System::flatten(const long long idx_arr[]) {
    long long idx = 0, factor = 1;
    for (long long i = DIM - 1; i >= 0; i--) {
        idx += idx_arr[i] * factor;
        factor *= N;
    }
    return idx;
}

inline void ND_System::unflatten(long long idx, long long idx_arr[]) {
    for (long long i = 0; i < DIM; i++) {
        idx_arr[i] = idx % N;
        idx /= N;
    }
}

long long ND_System::magnet_tot() {
    long long magnetisation = 0;
    for (long long i = 0; i < LEN; i++) {
        magnetisation += 2 * state[i] - 1;
    }
    return magnetisation;
}

long long ND_System::energy_tot() {
    long long energy = 0;
    long long idx_arr[DIM];

    for (long long i = 0; i < LEN; i++) {
        unflatten(i, idx_arr);
        long long spin = 2 * state[i] - 1;
        long long sum_neighbors = 0;

        for (long long d = 0; d < DIM; d++) {
            long long neighbor_idx = idx_arr[d];
            
            // Right neighbor
            idx_arr[d] = (neighbor_idx + 1) % N;
            sum_neighbors += 2 * state[flatten(idx_arr)] - 1;

            // Left neighbor
            idx_arr[d] = (neighbor_idx - 1 + N) % N;
            sum_neighbors += 2 * state[flatten(idx_arr)] - 1;

            // Restore original index
            idx_arr[d] = neighbor_idx;
        }
        energy += -spin * sum_neighbors;
    }
    return energy / 2;
}


void ND_System::MC_SWEEP(float beta) {
    long long s_idx = this->dist(gen);
    long long s_idxarr[DIM];

    unflatten(s_idx, s_idxarr);
    long long sum_neighbors = 0;

    for (long long d = 0; d < DIM; d++) {
        long long neighbor_idx = s_idxarr[d];

        // Right neighbor
        s_idxarr[d] = (neighbor_idx + 1) % N;
        sum_neighbors += 2 * state[flatten(s_idxarr)] - 1;

        // Left neighbor
        s_idxarr[d] = (neighbor_idx - 1 + N) % N;
        sum_neighbors += 2 * state[flatten(s_idxarr)] - 1;

        // Restore original index
        s_idxarr[d] = neighbor_idx;
    }

    long long S_i = 2 * state[s_idx] - 1;
    long long delta_E = 2 * S_i * sum_neighbors;
    if (delta_E <= 0 || prob(gen) < exp(-delta_E * beta)) {
        state[s_idx] = !state[s_idx];  // Flip the spin
    }
}