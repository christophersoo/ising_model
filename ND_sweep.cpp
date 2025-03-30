#include "ND_ising_system.h"
using namespace std;

long long pow_int(long long base, long long exp) {
    long long result = 1;
    while (exp--) result *= base;
    return result;
}

int main(int argc, char* argv[]){
    if (argc != 5) {
        return 1;
    }

    int N = stoi(argv[1]);
    int DIM = stoi(argv[2]);
    float BETA = stof(argv[3]);
    int MAX_SWEEP = stoi(argv[4]);

    ND_System nd_sys(N, DIM, pow_int(N, DIM));
    ofstream file("./data/sweeps.csv");
    file << "sweep,energy,magnetisation" << endl;
    for (int i = 0; i < MAX_SWEEP; i++){
        nd_sys.MC_SWEEP(BETA);
        file << i << "," << nd_sys.energy_tot() << "," << nd_sys.magnet_tot() << endl;
    }
    file.close();
    return 0;
}