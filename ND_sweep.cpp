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
    // Check if command line arguments are not 4 + 1 which is calling the executable
    if (argc != 5) {
        // return 1 (error) if it isn't
        return 1;
    }

    // Parameters we get from command line
    int N = stoi(argv[1]);
    int DIM = stoi(argv[2]);
    float BETA = stof(argv[3]);
    int MAX_SWEEP = stoi(argv[4]);

    // Define the system
    ND_System nd_sys(N, DIM, pow_int(N, DIM));

    // Creates a file to dump data as a csv
    ofstream file("./data/sweeps.csv");

    // Define column of the csv
    file << "sweep,energy,magnetisation" << endl;

    // We loop through the amount of sweeps and dump them into the csv file
    for (int i = 0; i < MAX_SWEEP; i++){
        nd_sys.MC_SWEEP(BETA);
        // Dumps the data we get after each sweep and track their energy and magnetisation
        file << i << "," << nd_sys.energy_tot() << "," << nd_sys.magnet_tot() << endl;
    }
    
    // Close and save the file
    file.close();
    return 0;
}