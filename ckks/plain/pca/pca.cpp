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
vector<vector<double>> ptranspose(vector<vector<double>> m, int n, int d);
double pinverse(double ct);
double proot(double ct);

int main() {
    srand(time(NULL));
    ofstream fout;
    fout.open("pca_plaintext.txt", ios::out | ios::app); // Open the file in append mode
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

        system_clock::time_point start_time2 = system_clock::now();

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
        vector<vector<double>> cx = x;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < d; j++) {
                cx[i][j] -= ptmpv[j];
            }
        }

        auto cxt = ptranspose(cx, n, d);
        auto sigma = pmatrix_mult(cxt, cx, d, n, d);

        // Calculate covariance matrix
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++) {
                sigma[i][j] /= (double)n;
            }
        }

        // Perform power iteration to find the principal component
        for (int i = 0; i < d; i++) {
            double zero = 0.0;
            for (int j = 0; j < d; j++) {
                zero += sigma[i][j];
            }
            ptmpv[i] = zero;
        }

        vector<vector<double>> tmpv5;
        tmpv5.push_back(ptmpv);

        for (int i = 0; i < 4; i++) {
            auto tmp = pmatrix_vector_mult(sigma, tmpv5[i], d, d);
            tmpv5.push_back(tmp);
        }

        double u = 0.0;
        for (int i = 0; i < d; i++) {
            u += tmpv5[4][i] * tmpv5[4][i];
        }

        double du = 2.0 * u;
        u /= du;
        u = proot(u);
        u = pinverse(u);
        u /= sqrt(du);

        for (int i = 0; i < d; i++) {
            tmpv5[4][i] *= u;
        }

        auto ppca = pmatrix_vector_mult(cx, tmpv5[4], n, d);

        system_clock::time_point end_time2 = system_clock::now();

        cout << "pca: ";
        for (const auto& val : ppca) {
            cout << val << " ";
        }
        cout << endl;

        nanoseconds nano2 = end_time2 - start_time2;
        cout << "Time for dimension " << d << ": " << nano2.count() << " ns" << endl;

        fout << "Dimension: " << d << ", Time (ns): " << nano2.count() << endl;
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

vector<vector<double>> ptranspose(vector<vector<double>> m, int n, int d) {
    vector<vector<double>> transpose(d, vector<double>(n, 0.0));
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < n; j++) {
            transpose[i][j] = m[j][i];
        }
    }
    return transpose;
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

