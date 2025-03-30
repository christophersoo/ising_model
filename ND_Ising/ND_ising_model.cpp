#include "ND_ising_system.h"
using namespace std;

int main(int argc, char* argv[]){
    if (argc != 6) {
        return 1;
    }

    int N = stoi(argv[1]);
    int DIM = stoi(argv[2]);
    int SWEEP = stoi(argv[3]);
    int CONFS = stoi(argv[4]);
    float BETA = stof(argv[5]);

    ND_System nd_sys(N, DIM);
    ofstream file("output.csv");
    file << "id,energy_total,magnetisation" << endl;

    for (long long i = 0; i < CONFS; i++) {
        for (long long j = 0; j < SWEEP; j++) {
            nd_sys.MC_SWEEP(BETA);
        }
        cout << "Conf number: " << i << " completed." << endl;
        file << i << "," << nd_sys.energy_tot() << "," << nd_sys.magnet_tot() << endl;
    }

    file.close();
    cout << "Successfully created configurations." << endl;
    return 0;
}
