#include <iostream>
#include "config.h"
#include <bitset>
#include <random>
using namespace std;

constexpr int power(int base, int exp) {return (exp == 0) ? 1 : base * power(base, exp - 1);}
constexpr int LEN = power(N, DIM);

bitset<LEN> state;
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist(0, LEN);

int energy_tot(bitset<LEN>& state){
    int energy = 0;
    for (int i=0; i<LEN-1; i++){
        energy += -(4 * state[i] * state[i+1] - 2 * state[i] - 2 * state[i+1] + 1);
    }
    return energy;
}

int magnet_tot(bitset<LEN>& state){
    int magnetisation = 0;
    for (int i=0; i<LEN; i++){
        magnetisation += 2 * state[i] - 1;
    }
}

int get(bitset<LEN>& state, int idx_arr[]){
    int idx = 0, factor = 1;
    for (int i=DIM-1; i>=0; i--){idx += idx_arr[i] * factor; factor *= N;}
    return 2 * state[idx] - 1;
}

void flip(bitset<LEN>& state, int idx_arr[]){
    int idx = 0, factor = 1;
    for (int i=DIM-1; i>=0; i--){idx += idx_arr[i] * factor; factor *= N;}
    state.flip(idx);
}

void init(bitset<LEN>& state){
    for (int i = 0; i<LEN; i++){state[i] = dist(gen) % 2;}
}

void MC_SWEEP(bitset<LEN>& state){
    int s_idx = dist(gen);
    
}

int main(){
    init(state);
    return 0;    
}