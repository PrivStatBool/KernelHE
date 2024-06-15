#include <iostream>
#include <ostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include <random>
#include <numeric>
#include <fstream>

using namespace std;
using namespace chrono;

int main() {
    // Setting
    int n = 10; // The number of data

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(10, 20); // Random distribution

    ofstream fout;
    fout.open("variance_plaintext.txt", ios::out | ios::app); // Open the file in append mode
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int d = 5; d <= 15; d++) { // dimension
        cout << "d: " << d << " starts" << endl;

        // Generating dataset
        std::vector<std::vector<double>> dataset(n, std::vector<double>(d, 0.0));

        for (int z = 0; z < n; z++) {
            std::vector<double> x;
            int count = 0;
            while (count < d) {
                double data = dist(gen);
                x.push_back(data);
                count += 1;
            }
            dataset[z] = x;
        }

        // Calculate mean vector
        system_clock::time_point start_time_eval = system_clock::now();

        std::vector<double> mean(d, 0.0);

        for (int i = 0; i < d; i++) { // select ith element of vectors
            double tmp = 0.0;
            for (int j = 0; j < n; j++) {
                tmp += dataset[j][i];
            } // Summation of all ith elements
            mean[i] = tmp / (double)n; // Update ith element of mean vector
        }

        // Calculate variance
        double variance_square = 0.0;

        for (int i = 0; i < n; i++) { // Select data
            for (int j = 0; j < d; j++) { // Select jth element of vectors
                variance_square += (mean[j] - dataset[i][j]) * (mean[j] - dataset[i][j]);
            }
        }

        double variance = variance_square / (double)n;

        system_clock::time_point end_time_eval = system_clock::now();
        nanoseconds nano = end_time_eval - start_time_eval;

        // Use the variance to avoid unused variable warning
        (void)variance;

        cout << "Time for dimension " << d << ": " << nano.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano.count() << endl;
    }

    fout.close();
    return 0;
}

