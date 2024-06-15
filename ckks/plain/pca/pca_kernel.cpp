#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
using namespace chrono;

vector<vector<double>> pmatrix_mult(vector<vector<double>> m1, vector<vector<double>> m2, int n, int d, int q);
vector<double> pmatrix_vector_mult(vector<vector<double>> m1, vector<double> m2, int n, int d);
double pinverse(double ct);
double proot(double ct);
double pinner_product(vector<double> m1, vector<double> m2, int d);
vector<vector<double>> kernelize(vector<vector<double>> x, int n, int d);

int main() {
    srand(time(NULL));
    ofstream fout;
    fout.open("pca_kernel_plaintext.txt", ios::out | ios::app); // Open the file in append mode
    if (!fout) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int d = 5; d <= 15; d++) { // Test dimensions from 5 to 15
        int n = 10; // Number of data points
        cout << "n: " << n << endl;
        cout << "d: " << d << endl;

        vector<vector<double>> x(n, vector<double>(d, 0.0));
        vector<double> ptmpv(d, 0.0);

        // Generate dataset
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                x[i][j] = rand() % 2 - 1.0;
            }
        }

        // Calculate mean of each dimension
        for (int i = 0; i < d; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                sum += x[j][i];
            }
            sum /= (double)n;
            ptmpv[i] = sum;
        }

        // Center the data
        vector<vector<double>> kcx = x;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                kcx[i][j] -= ptmpv[j];
            }
        }

        ptmpv.clear();
        auto kernel = kernelize(kcx, n, d);

        system_clock::time_point start_time1 = system_clock::now();

        vector<vector<double>> tmpv5;
        auto tmp = pmatrix_vector_mult(kernel, kcx[0], n, n);
        tmpv5.push_back(tmp);

        for (int i = 0; i < 4; i++) {
            auto tmp = pmatrix_vector_mult(kernel, tmpv5[i], n, n);
            tmpv5.push_back(tmp);
        }

        double divisor1 = tmpv5[3][0] * 2.0;
        double lamda = tmpv5[4][0] * pinverse(tmpv5[3][0] / divisor1);
        lamda /= divisor1;

        double v = pinner_product(tmpv5[4], tmpv5[4], n);
        v *= lamda;
        double divisor2 = v * 2.0;
        v /= divisor2;
        v = pinverse(proot(v));
        v /= sqrt(divisor2);

        for (int i = 0; i < n; i++) {
            tmpv5[4][i] *= v;
        }

        vector<double> pkernel_pca;
        for (int i = 0; i < n; i++) {
            pkernel_pca.push_back(pinner_product(tmpv5[4], kernel[i], n));
        }

        system_clock::time_point end_time1 = system_clock::now();

        cout << "kernel_pca: ";
        for (const auto& val : pkernel_pca) {
            cout << val << " ";
        }
        cout << endl;

        nanoseconds nano1 = end_time1 - start_time1;
        cout << "Time for dimension " << d << ": " << nano1.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano1.count() << endl;
    }

    fout.close();
    return 0;
}

vector<vector<double>> pmatrix_mult(vector<vector<double>> m1, vector<vector<double>> m2, int n, int d, int q) {
    vector<vector<double>> m;
    vector<double> tmp;
    for (int p = 0; p < n; p++) {
        for (int i = 0; i < q; i++) {
            double zero = 0.0;
            for (int j = 0; j < d; j++) {
                zero += m1[p][j] * m2[j][i];
            }
            tmp.push_back(zero);
        }
        m.push_back(tmp);
        tmp.clear();
    }
    return m;
}

vector<double> pmatrix_vector_mult(vector<vector<double>> m1, vector<double> m2, int n, int d) {
    vector<double> tmp;
    for (int p = 0; p < n; p++) {
        double zero = 0.0;
        for (int i = 0; i < d; i++) {
            zero += m1[p][i] * m2[i];
        }
        tmp.push_back(zero);
    }
    return tmp;
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

double pinner_product(vector<double> m1, vector<double> m2, int d) {
    double ip = 0.0;
    for (int i = 0; i < d; i++) {
        ip += m1[i] * m2[i];
    }
    return ip;
}

vector<vector<double>> kernelize(vector<vector<double>> x, int n, int d) {
    vector<vector<double>> kernel;
    vector<double> tmpv;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            tmpv.push_back(x[0][0]);
        }
        for (int j = i; j < n; j++) {
            tmpv.push_back(pinner_product(x[i], x[j], d));
        }
        kernel.push_back(tmpv);
        tmpv.clear();
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            kernel[j][i] = kernel[i][j];
        }
    }
    return kernel;
}

