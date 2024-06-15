#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
using namespace chrono;

double pinner_product(vector<double> m1, vector<double> m2, int d);
double pinverse(double ct);

int main() {
    srand(time(NULL));
    int q = 2;
    int n = 10;
    double lr = 0.01;
    cout << "n: " << n << endl;

    ofstream fout;
    fout.open("svm_plaintext.txt", ios::out | ios::app); // Open the file in append mode
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int d = 5; d <= 15; d++) { // Test dimensions from 5 to 15
        cout << "d: " << d << " starts" << endl;

        // Generating dataset
        vector<vector<double>> x(n, vector<double>(d, 0.0));
        vector<double> y(n, 0.0);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                x[i][j] = rand() % 10 - 5.0;
            }
            y[i] = rand() % 20 - 10.0;
        }

        // SVM
        vector<double> divisors(n, 0.0);
        for (int i = 0; i < n; i++) {
            divisors[i] = 2.0 * pinner_product(x[i], x[i], d);
        }

        system_clock::time_point start_time1 = system_clock::now();

        vector<double> alpha(n, 0.0);
        vector<double> pinverse_product(n, 0.0);
        for (int i = 0; i < n; i++) {
            pinverse_product[i] = pinverse(pinner_product(x[i], x[i], d) / divisors[i]) / divisors[i];
        }

        for (int p = 0; p < q; p++) {
            for (int i = 0; i < n; i++) {
                double tmp = 0.0;
                for (int j = 0; j < n; j++) {
                    tmp += alpha[j] * y[j] * pinner_product(x[j], x[i], d);
                }
                alpha[i] += lr * (1.0 - y[i] * tmp);
                alpha[i] *= pinverse_product[i];
            }
        }

        vector<double> w(d, 0.0);
        for (int i = 0; i < d; i++) {
            double tmp = 0.0;
            for (int j = 0; j < n; j++) {
                tmp += (alpha[j] * y[j]) * x[j][i];
            }
            w[i] = tmp;
        }
        cout << "w: ";
        for (const auto& val : w) {
            cout << val << " ";
        }
        cout << endl;

        double b = 0.0;
        for (int i = 0; i < n; i++) {
            b += y[i] - pinner_product(w, x[i], d);
        }
        b /= (double)n;
        cout << "b: " << b << endl;

        system_clock::time_point end_time1 = system_clock::now();
        nanoseconds nano1 = end_time1 - start_time1;
        cout << "Time for dimension " << d << ": " << nano1.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano1.count() << endl;
    }

    fout.close();
    return 0;
}

double pinner_product(vector<double> m1, vector<double> m2, int d) {
    double ip = 0.0;
    for (int i = 0; i < d; i++) {
        ip += m1[i] * m2[i];
    }
    return ip;
}

double pinverse(double ct) {
    double c = 1.0 - ct;
    double v = 2.0 - ct;

    for (int i = 0; i < 5; i++) {
        c = c * c;
        v = v * (1.0 + c);
    }
    return v;
}

