#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <numeric>

using namespace std;
using namespace chrono;

int main() {
    // Setting
    int n = 2; // The number of data

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.5, 1.0); // Random distribution

    ofstream fout;
    fout.open("similarity_plaintext.txt", ios::out | ios::app); // Open the file in append mode
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

        // Calculate original similarity
        double dot = 0.0, x_norm_square = 0.0, y_norm_square = 0.0;
        std::vector<double> x = dataset_original[0];
        std::vector<double> y = dataset_original[1];

        system_clock::time_point start_time = system_clock::now(); // Measure time of evaluation

        for (int i = 0; i < d; i++) {
            dot += x[i] * y[i];
            x_norm_square += x[i] * x[i];
            y_norm_square += y[i] * y[i];
        }
        double similarity_original = dot / (sqrt(x_norm_square) * sqrt(y_norm_square));

        system_clock::time_point end_time = system_clock::now(); // Measure time of evaluation
        nanoseconds nano = end_time - start_time;

        // Use the similarity_original to avoid unused variable warning
        (void)similarity_original;

        cout << "Time for dimension " << d << ": " << nano.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano.count() << endl;
    }
    fout.close();
    return 0;
}

