#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <numeric>

using namespace std;
using namespace chrono;

double inner_product(std::vector<double> a, std::vector<double> b);
double proot(double ct);

int main() {
    //Setting
    int n = 2; //The number of data

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.5, 1.0); //Random distribution

    ofstream fout;
    fout.open("distance_kernel_plaintext.txt", ios::out | ios::app); // Open the file in append mode
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int d = 5; d <= 15; d++) { //dimension
        cout << "d: " << d << " starts" << endl;

        //Generating dataset
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

        //Make kernel
        system_clock::time_point start_time1 = system_clock::now();

        std::vector<std::vector<double>> kernel(n, std::vector<double>(d, 0));

        for (int i = 0; i < n; i++) {
            kernel[i][i] = inner_product(dataset_original[i], dataset_original[i]);
        } //Diagonal elements

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                kernel[i][j] = inner_product(dataset_original[i], dataset_original[j]);
                kernel[j][i] = inner_product(dataset_original[i], dataset_original[j]);
            }
        } //Non-diagonal elements

        system_clock::time_point end_time1 = system_clock::now();
        nanoseconds nano1 = end_time1 - start_time1;

        //Calculate original distance
        system_clock::time_point start_time2 = system_clock::now(); //Measure time of evaluation

        double distance_square = kernel[0][0] + kernel[1][1] - 2 * kernel[0][1];

        double distance_original = proot(distance_square);

        system_clock::time_point end_time2 = system_clock::now(); //Measure time of evaluation
        nanoseconds nano2 = end_time2 - start_time2;

        // Use the distance_original to avoid unused variable warning
        (void)distance_original;

        cout << "Time for dimension " << d << " (Kernel): " << nano1.count() << " ns, (Distance): " << nano2.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Kernel Time (ns): " << nano1.count() << ", Distance Time (ns): " << nano2.count() << endl;
    }
    fout.close();
    return 0;
}

double inner_product(std::vector<double> a, std::vector<double> b) {
    int n = a.size();
    int m = b.size();
    double result = 0.0;

    if (n != m) {
        std::cout << "Two input vectors have different size!" << std::endl;
        return 0.0;
    } else {
        for (int i = 0; i < n; i++) {
            result += a[i] * b[i];
        }
        return result;
    }
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

