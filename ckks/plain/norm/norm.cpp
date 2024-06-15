#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

double proot(double ct);

int main() {
    int n = 1; // The number of data points
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(10, 20); // Random distribution

    ofstream fout;
    fout.open("norm_plaintext.txt", ios::out | ios::app); // Open the file in append mode
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int d = 5; d <= 15; d++) { // dimension
        cout << "d: " << d << " starts" << endl;

        // Generating dataset
        std::vector<std::vector<double>> dataset_original(n, std::vector<double>(d, 0));

        for (int z = 0; z < n; z++) {
            std::vector<double> x;
            int count = 0;
            while (count < d) {
                double data = dist(gen);
                x.push_back(data);
                count += 1;
            }
            dataset_original[z] = x;
        }

        // Calculate original norm
        double norm_square = 0.0;
        std::vector<double> x = dataset_original[0];

        system_clock::time_point start_time = system_clock::now();

        for (int i = 0; i < d; i++) {
            norm_square += x[i] * x[i];
        }
        double norm_original = proot(norm_square);

        system_clock::time_point end_time = system_clock::now(); // Measure time of evaluation
        nanoseconds nano = end_time - start_time;

        // Use the norm_original to avoid unused variable warning
        (void)norm_original;

        cout << "Time for dimension " << d << ": " << nano.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano.count() << endl;
    }
    fout.close();
    return 0;
}

double proot(double ct) {
    double a = ct;
    double b = ct - 1.0;
    for (int i = 0; i < 5; i++) {
        a = a * (1.0 - 0.5 * b);
        b = (b * b) * ((b - 3.0) / 4.0);
    }
    a = a * (1.0 - 0.5 * b);

    return a;
}

