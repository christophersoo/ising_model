// Libraries
#include "ND_ising_system.h"
using namespace std;

// Function to calculate powers
long long pow_int(long long base, long long exp) {
    long long result = 1;
    while (exp--) result *= base;
    return result;
}

// Main function
int main(int argc, char* argv[]){
    // Check if command line arguments are not 5 + 1 which is calling the executable
    if (argc != 6) {
        // return 1 (error) otherwise
        return 1;
    }

    // define 6 parameters from command line arguments
    int N = stoi(argv[1]);
    int DIM = stoi(argv[2]);
    int SWEEP = stoi(argv[3]);
    int CONFS = stoi(argv[4]);
    float BETA = stof(argv[5]);

    // initialise and define the system
    ND_System nd_sys(N, DIM, pow_int(N, DIM));

    // create file to dump data as a csv file
    ofstream file("./data/output.csv");

    // define columns of the csv
    file << "id,energy_total,magnetisation" << endl;

    // loop through each configuration iterations
    for (long long i = 0; i < CONFS; i++) {

        // loop through each sweeps
        for (long long j = 0; j < SWEEP; j++) {

            // performs monte carlo sweep
            nd_sys.MC_SWEEP(BETA);
        }

        // tracks and dumps energy and magnetisation into the csv file
        file << i << "," << nd_sys.energy_tot() << "," << nd_sys.magnet_tot() << endl;
    }

    // closes and saves the file
    file.close();
    return 0;
}
