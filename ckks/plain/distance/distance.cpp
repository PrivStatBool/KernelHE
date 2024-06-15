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

double calculator_mean(std::vector<double> input);
double calculator_standard_deviation(std::vector<double> input);
double proot(double ct);

int main() {
    //Setting
    int n = 2; //The number of data

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.5, 1.0); //Random distribution

    ofstream fout;
    fout.open("distance_plaintext.txt"); //Save the results of time
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int trials = 20000;

    for (int d = 5; d <= 15; d++) { //dimension
        std::vector<double> result;
        cout << "d: " << d << " starts with trials " << trials << endl;
        for (int testcase = 0; testcase < trials; testcase++) { //Test trials case for each dimension
            //Generating dataset
            std::vector<std::vector<double>> dataset_original(n, std::vector<double> (d, 0));

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

            //Calculate original distance
            double distance_square = 0.0;
            std::vector<double> x = dataset_original[0];
            std::vector<double> y = dataset_original[1];

            system_clock::time_point start_time = system_clock::now(); //Measure time of evaluation

            for (int i = 0; i < d; i++) {
                distance_square += (x[i] - y[i]) * (x[i] - y[i]);
            }
            double distance_original = proot(distance_square);

            system_clock::time_point end_time = system_clock::now(); //Measure time of evaluation
            nanoseconds nano = end_time - start_time;

            result.push_back(nano.count());

            // Use the distance_original to avoid unused variable warning
            (void)distance_original;
        }
        double mean = calculator_mean(result);
        double standard_deviation = calculator_standard_deviation(result);
        cout << "The mean of time of evaluation: " << mean << " and the standard deviation of time: " << standard_deviation << endl;

        fout << "Dimension: " << d << ", Mean Time (ns): " << mean << ", Standard Deviation (ns): " << standard_deviation << endl;
    }
    fout.close();
    return 0;
}

double calculator_mean(std::vector<double> input) {
    int n = input.size();
    double sum = accumulate(input.begin(), input.end(), 0.0);
    double mean = sum / double(n);
    return mean;
}

double calculator_standard_deviation(std::vector<double> input) {
    int n = input.size();
    double mean = calculator_mean(input);
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double increment = pow(input[i] - mean, 2);
        sum = sum + increment;
    }

    double standard_deviation = sqrt(sum / double(n));

    return standard_deviation;
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

