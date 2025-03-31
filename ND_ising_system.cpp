// Include Header File
#include "ND_ising_system.h"

// Constructor Implementation
// Constructor assigns values and populates the state
ND_System::ND_System(int N, int DIM, long long LEN)
    : N(N), DIM(DIM), LEN(LEN), state(LEN), 
    gen(rd()), dist(0, LEN - 1), prob(0.0, 1.0) {
    // Populate the state
    this->init();
}

// Init Implementation
// Init populates the state with 0s and 1s
void ND_System::init(){
    // Loop through each entry in state
    for (long long i = 0; i < LEN; i++) {
        // Assigns 0 or 1 with p = 0.5
        state[i] = dist(gen) % 2;
    }
}

// Inline function to make it faster, Implementation for flatten
// flatten returns the 1d index from a set of multi-dimensional indices
inline long long ND_System::flatten(const long long idx_arr[]) {
    long long idx = 0, factor = 1;
    // Loop through each entry in idx_arr
    for (long long i = DIM - 1; i >= 0; i--) {
        // finds the index by multiplying it by each factor 
        idx += idx_arr[i] * factor;
        factor *= N;
    }
    return idx;
}

// Unflatten function Implementation
// Unflatten returns a set of multi-dimensional indices from a 1d flattened index
inline void ND_System::unflatten(long long idx, long long idx_arr[]) {
    // Loop through each entry in idx_arr
    for (long long i = 0; i < DIM; i++) {
        // updates the array by decomposing the 1d index into multi-dimensional indices
        // uses divide & mod to find the respective indices
        idx_arr[i] = idx % N;
        idx /= N;
    }
}

// Implementation for magnet total
// magnet total calculates the total magnetisation of the current state
long long ND_System::magnet_tot() {
    long long magnetisation = 0;
    // loops through each entry in state
    for (long long i = 0; i < LEN; i++) {
        // cummulates the values
        magnetisation += 2 * state[i] - 1;
    }
    return magnetisation;
}

// Implementation for energy total
// energy total calculates the total energy of the current state
long long ND_System::energy_tot() {
    // helper variables
    long long energy = 0;
    long long idx_arr[DIM];

    // loop through each entry
    for (long long i = 0; i < LEN; i++) {

        // finds the multi-dimensional indices for each entry
        unflatten(i, idx_arr);
        long long spin = 2 * state[i] - 1;
        long long sum_neighbors = 0;

        // finds all the neighbors by finding combinations of shifting +1 / -1 for each entry in idx_arr
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
        // cumulates the energy
        energy += -spin * sum_neighbors;
    }
    return energy / 2;
}

// Implementation of the Metropolis-Hastings Algorithm
// accpet / reject flips based on probability affected by beta
void ND_System::MC_SWEEP(float beta) {
    // finds a random entry in s_idx
    long long s_idx = this->dist(gen);
    long long s_idxarr[DIM];

    // finds the multi-dimensional indices to find neighbors
    unflatten(s_idx, s_idxarr);
    long long sum_neighbors = 0;

    // finds its neighbors and calculates energy change if it was flipped
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

    // calculates energy change
    long long S_i = 2 * state[s_idx] - 1;
    long long delta_E = 2 * S_i * sum_neighbors;

    // calculates and accepts / rejects flip based on probability
    if (delta_E <= 0 || prob(gen) < exp(-delta_E * beta)) {
        state[s_idx] = !state[s_idx];  // Flip the spin
    }
}