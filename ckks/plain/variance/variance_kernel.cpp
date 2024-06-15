#include <iostream>
#include <ostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <chrono>
#include <random>
#include <numeric>
#include <fstream>

using namespace std;
using namespace chrono;

double inner_product(std::vector<double> a, std::vector<double> b);

int main() {
    // Setting
    int n = 10; // The number of data

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(10, 20); // Random distribution

    ofstream fout;
    fout.open("variance_kernel_plaintext.txt", ios::out | ios::app); // Open the file in append mode
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

        // Make kernel
        system_clock::time_point start_time_kernel = system_clock::now();
        std::vector<std::vector<double>> kernel(n, std::vector<double>(n, 0.0));

        for (int i = 0; i < n; i++) {
            kernel[i][i] = inner_product(dataset[i], dataset[i]);
        } // Diagonal elements

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                kernel[i][j] = inner_product(dataset[i], dataset[j]);
                kernel[j][i] = inner_product(dataset[i], dataset[j]);
            }
        } // Non-diagonal elements

        system_clock::time_point end_time_kernel = system_clock::now();
        nanoseconds nano1 = end_time_kernel - start_time_kernel;

        // Calculate the first term
        system_clock::time_point start_time_eval = system_clock::now();

        double first_term = 0.0;
        for (int i = 0; i < n; i++) {
            first_term += kernel[i][i];
        }
        first_term = first_term / (double)n;

        // Calculate the second term
        double second_term = 0.0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                second_term += kernel[i][j];
            }
        }
        second_term = second_term / (double)pow(n, 2);

        // Calculate variance
        double variance = first_term - second_term;

        system_clock::time_point end_time_eval = system_clock::now();
        nanoseconds nano2 = end_time_eval - start_time_eval;

        // Use the variance to avoid unused variable warning
        (void)variance;

        cout << "Time for dimension " << d << " (Kernel): " << nano1.count() << " ns, (Variance): " << nano2.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Kernel Time (ns): " << nano1.count() << ", Variance Time (ns): " << nano2.count() << endl;
    }
    fout.close();
    return 0;
}

double inner_product(std::vector<double> a, std::vector<double> b) {
    int n = a.size();
    int m = b.size();
    double result = 0.0;

    if (n != m) {
        std::cout << "Two input vectors have different sizes!" << std::endl;
        return 0.0;
    } else {
        for (int i = 0; i < n; i++) {
            result += a[i] * b[i];
        }
        return result;
    }
}

