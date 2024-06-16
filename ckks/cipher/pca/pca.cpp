#define PROFILE

#include "openfhe.h"
#include "operation.h"
#include <chrono>
#include <iostream>
#include <cmath>
#include <cstdlib> 
#include <ctime>

using namespace std;
using namespace lbcrypto;
using namespace chrono;


vector<vector<double>> pmatrix_mult(vector<vector<double>> m1,vector<vector<double>> m2, int n, int d, int q);
vector<double> pmatrix_vector_mult(vector<vector<double>> m1,vector<double> m2, int n, int d);
vector<vector<double>> ptranspose(vector<vector<double>> m, int n, int d);
double pinverse(double ct);
double proot(double ct);
double pinner_product(vector<double> m1, vector<double> m2, int d);
vector<vector<double>> kernelize(vector<vector<double>> x, int n, int d);
vector<vector<vector<double>>> qr_factorize(vector<vector<double>> sigma, int d);
int main() {
    
    srand(time(NULL));
    for(int o=1; o<5; o++){
    int n = 5*o;
    int d = 10;
    cout<<"n: "<<n <<endl;
    cout<<"d: "<<d <<endl;
    vector<double> z = {0.};
    vector<vector<double>> x;
    vector<double> ptmpv;
    for(int i=0; i<n; i++){
        for(int j=0; j<d; j++){
            double tmp = rand() % 2 - 1.;
            ptmpv.push_back(tmp);
        }
        x.push_back(ptmpv);
        ptmpv.clear();
    }
    vector<double> y;
    for(int i=0; i<n; i++){
        double tmp = rand() % 4 - 2.;
        y.push_back(tmp);
    }
    cout<<"x: "<<x<<endl;

    cout<<"y: "<<y<<endl;

    system_clock::time_point start_time2 = system_clock::now();
    for(int i=0; i<d; i++){
        double sum = 0.;
        for(int j=0; j<n; j++){
            sum += x[j][i];
        }
        sum /= (double)n;    
        ptmpv.push_back(sum);
    }
    
    auto cx = x;
    for(int i=0; i<n; i++){
        for(int j=0; j<d; j++){
            cx[i][j] -= ptmpv[j];
        }
    }
    ptmpv.clear();

    auto cxt = ptranspose(cx,n,d);    
    auto sigma = pmatrix_mult(cxt,cx,d,n,d);

    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            sigma[i][j] /= double(n);
        }
    }
    cout<<"sigma: "<<endl;
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            cout<<" "<<sigma[i][j];
        }
        cout<<endl;
    }
    for(int i=0; i<d; i++){
        double zero = 0.;
        for(int j=0; j<d; j++){
            zero += sigma[i][j];
        }
        ptmpv.push_back(zero);
    }

    vector<vector<double>> tmpv5;
    tmpv5.push_back(ptmpv);
    ptmpv.clear();

    for(int i=0; i<4; i++){
        auto tmp = pmatrix_vector_mult(sigma,tmpv5[i],d,d);
        tmpv5.push_back(tmp);
    }
    cout<<"tmpv5: "<<endl;
    for(int i=0; i<5; i++){
        for(int j=0; j<n; j++){
            cout<<" "<<tmpv5[i][j];
        }
        cout<<endl;
    }
    double u = 0.;
    for(int i = 0; i<d; i++){
        u += tmpv5[4][i] * tmpv5[4][i];
    }
    
    double du = 2.*u;
    u /= du;
    cout<<"u: "<<u<<endl;
    //ensure that data is normalized between 0. and 1.
    u = proot(u);
    cout<<"u: "<<u<<endl;
    u = pinverse(u);
    cout<<"u: "<<u<<endl;
    u /= sqrt(du);
    cout<<"u: "<<u<<endl;

    for(int i=0; i<d; i++){
        tmpv5[4][i] *= u;
    }
    cout<<"u: "<<tmpv5[4]<<endl;

    auto ppca = pmatrix_vector_mult(cx,tmpv5[4],n,d);

    system_clock::time_point end_time2 = system_clock::now();

    tmpv5.clear();
    cout<<"pca: "<<endl;
    for(int i=0; i<n; i++){
        cout<<" "<<ppca[i];
    }
    cout<<endl;
    nanoseconds nano2 = end_time2 - start_time2;
    microseconds micro2 = duration_cast<microseconds>(end_time2 - start_time2);
    cout << "time (Plain_nonkernel) : " <<nano2.count() << " nano second" << endl;
    cout << "time (Plain_nonkernel) : " <<micro2.count() << " micro second" << endl;
    
    //kernel pca
    for(int i=0; i<d; i++){
        double sum = 0.;
        for(int j=0; j<n; j++){
            sum += x[j][i];
        }
        sum /= (double)n;    
        ptmpv.push_back(sum);
    }
    
    auto kcx = x;
    for(int i=0; i<n; i++){
        for(int j=0; j<d; j++){
            kcx[i][j] -= ptmpv[j];
        }
    }
    ptmpv.clear();
    auto kernel = kernelize(kcx,n,d);

    system_clock::time_point start_time1 = system_clock::now();
    
    auto tmp = pmatrix_vector_mult(kernel,y,n,n);
    tmpv5.push_back(tmp);

    for(int i=0; i<4; i++){
        auto tmp = pmatrix_vector_mult(kernel,tmpv5[i],n,n);
        tmpv5.push_back(tmp);
    }
    cout<<"tmpv5: "<<endl;
    for(int i=0; i<5; i++){
        for(int j=0; j<n; j++){
            cout<<" "<<tmpv5[i][j];
        }
        cout<<endl;
    }
    
    double divisor1 = tmpv5[3][0] * 2.; 
    //ensure that data is normalized between 0. and 1.
    double lamda = tmpv5[4][0] * pinverse(tmpv5[3][0]/divisor1);
    lamda /= divisor1;
    cout<<"lamda: "<<lamda<<endl;
    double v = pinner_product(tmpv5[4],tmpv5[4],n);

    v *= lamda;   
    cout<<"v: "<<v<<endl;
    double divisor2 = v * 2.;
    v /= divisor2;
    //ensure that data is normalized between 0. and 1.
    v = pinverse(proot(v)); 
    v /= sqrt(divisor2); 
    cout<<"v: "<<v<<endl;
    for(int i=0; i<n; i++){
        tmpv5[4][i] *= v;
    }
    cout<<"eigenv: "<<tmpv5[4]<<endl;
    vector<double> pkernel_pca; 
    for(int i=0; i<n; i++){
        pkernel_pca.push_back(pinner_product(tmpv5[4],kernel[i],n));
    }           
    cout<<"kernel_pca: "<<endl;
    for(int i=0; i<n; i++){
        cout<<" "<<pkernel_pca[i];
    }
    cout<<endl;

    system_clock::time_point end_time1 = system_clock::now();

    nanoseconds nano1 = end_time1 - start_time1;
    microseconds micro1 = duration_cast<microseconds>(end_time1 - start_time1);
    cout << "time (Plain_kernel) : " <<nano1.count() << " nano s" << endl;
    cout << "time (Plain_kernel) : " <<micro1.count() << " micro s " << endl;
    uint32_t batchSize = 64;
    CCParams<CryptoContextCKKSRNS> parameters;
    SecretKeyDist secretKeyDist = UNIFORM_TERNARY;
    parameters.SetMultiplicativeDepth(39);
    parameters.SetScalingModSize(50);
    parameters.SetBatchSize(batchSize);
    parameters.SetSecretKeyDist(secretKeyDist);
    parameters.SetRingDim(1 << 16);
    parameters.SetSecurityLevel(HEStd_128_classic);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    // Enable the features that you wish to use
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

  
    auto keys = cc->KeyGen();

    cc->EvalMultKeyGen(keys.secretKey);

    int nd = max(n,d);
    double m = 1./(double)n;
    vector<int> ro;
    for(int i=1; i<nd+1; i++){
        ro.push_back(i);
        ro.push_back(-i);
    }
    cc->EvalRotateKeyGen(keys.secretKey, ro);

    vector<Plaintext> p;
    for(int i=0; i<nd; i++){
        Plaintext ptxt;
        p.push_back(ptxt);
    }

    Plaintext result;
    cout.precision(8);

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > ctxs;

    for(int i=0; i<n; i++){
        p[i] = cc->MakeCKKSPackedPlaintext(x[i]);
        auto c = cc->Encrypt(keys.publicKey, p[i]);
        ctxs.push_back(c);
    }

    p[0] = cc->MakeCKKSPackedPlaintext(y);
    auto yy = cc->Encrypt(keys.publicKey, p[0]);

    p[0] = cc->MakeCKKSPackedPlaintext(z);
    auto zz = cc->Encrypt(keys.publicKey, p[0]);
    
    auto mean = cc->EvalMult(add_many(ctxs,n,cc),m);

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > cctxs;
    for(int i=0; i<n; i++){
        auto tmp = cc->EvalSub(ctxs[i],mean);
        cctxs.push_back(tmp);
    }
    system_clock::time_point start_time3 = system_clock::now();

    vector<Plaintext> diag;
    for(int i=0; i<nd; i++){
        vector<double> s;
        for(int j=0; j<nd; j++){
            if(i == j){
                s.push_back(1.);
            }
            else{
                s.push_back(0.);
            }
        }
        p[i] = cc->MakeCKKSPackedPlaintext(s);
        diag.push_back(p[i]);
        s.clear();
    }

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > t;
    if (n<d){
        for(int i=0; i<d-n; i++){
            cctxs.push_back(zz);
        }
        t = transpose(cctxs,diag,d,cc);  // if n<d;
    }
    else {
        t = transpose(cctxs,diag,n,cc);
    }             
    
    auto cmatrix = matrix_mult(diagonalize(cctxs,diag,nd,cc),t,nd,cc);
    for(int i=0; i<nd; i++){
        cmatrix[i] = cc->EvalMult(cmatrix[i],m);
    }

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;

    auto eigenvector = power_method(cmatrix,d,cc);
    auto ev = cc->EvalMult(eigenvector[4],eigenvector[4]);
    tmpv.push_back(ev);
    for(int i=1; i<d; i++){
        tmpv.push_back(cc->EvalRotate(ev,i));
    }
    ev = add_many(tmpv,d,cc);
    tmpv.clear();
    ev = cc->EvalMult(ev,diag[0]);
    ev = cc->EvalMult(ev,1./du);
    ev = square_root(ev,cc);
    ev = inverse(ev,cc);
    ev = cc->EvalMult(ev,sqrt(1./du));
    ev = cc->EvalMult(ev,diag[0]);
    tmpv.push_back(ev);
    for(int i=1; i<d; i++){
        tmpv.push_back(cc->EvalRotate(ev,-i));
    }
    ev = add_many(tmpv,d,cc);
    tmpv.clear();
    auto eigenv = cc->EvalMult(eigenvector[4],ev);

    auto pca = matrix_vector_mult(diagonalize(t,diag,nd,cc),eigenv,nd,cc);
    system_clock::time_point end_time3 = system_clock::now();

    cout << "level: " << pca->GetLevel() << endl;
    cc->Decrypt(keys.secretKey, pca, &result);
    result->SetLength(n);
    cout << "pca = " << result;

    microseconds micro3 = duration_cast<microseconds>(end_time3 - start_time3);
    cout << "time: " <<micro3.count()/1000000. << " s" << endl;

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > ckernel;

    for(int i=0; i<n; i++){
        p[i] = cc->MakeCKKSPackedPlaintext(kernel[i]);
        auto c = cc->Encrypt(keys.publicKey, p[i]);
        ckernel.push_back(c);
    }
    system_clock::time_point start_time4 = system_clock::now();

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > keigenvector;
    auto c_diag = diagonalize(ckernel,diag,n,cc);

    auto ctmp = matrix_vector_mult(c_diag,yy,n,cc);
    keigenvector.push_back(ctmp);
    for(int i=0; i<4; i++){
        auto ctmp = matrix_vector_mult(c_diag,keigenvector[i],n,cc);
        keigenvector.push_back(ctmp);
    }

    auto clamda = cc->EvalMult(keigenvector[4],inverse(cc->EvalMult(cc->EvalMult(keigenvector[3],1./divisor1),diag[0]),cc));
    clamda = cc->EvalMult(clamda,1./divisor1);
    auto cv = inner_product(keigenvector[4],keigenvector[4],n,cc);
    cv = cc->EvalMult(cv,diag[0]);
    cv = cc->EvalMult(cv,clamda);
    cv = cc->EvalMult(cv,1./divisor2);
    cv = inverse(square_root(cv,cc),cc);
    cv = cc->EvalMult(cv,1./sqrt(divisor2)); 
    cv = cc->EvalMult(cv,diag[0]);
    tmpv.push_back(cv);
    for(int i=1; i<n; i++){
        tmpv.push_back(cc->EvalRotate(cv,-i));
    }
    cv = add_many(tmpv,n,cc);
    tmpv.clear();
    auto keigenv = cc->EvalMult(keigenvector[4],cv);

    auto kernel_pca = matrix_vector_mult(diagonalize(ckernel,diag,n,cc),keigenv,n,cc);

    system_clock::time_point end_time4 = system_clock::now();

    cout << "level: " << kernel_pca->GetLevel() << endl;
    cc->Decrypt(keys.secretKey, kernel_pca, &result);
    result->SetLength(n);
    cout << "kernel_pca = " << result;

    microseconds micro4 = duration_cast<microseconds>(end_time4 - start_time4);
    cout << "time: " <<micro4.count()/1000000. << " s" << endl;
    }
    return 0;
}

vector<vector<double>> pmatrix_mult(vector<vector<double>> m1,vector<vector<double>> m2, int n, int d, int q){

    vector<vector<double>> m;
    vector<double> tmp;
    for(int p=0; p<n; p++){
        for(int i=0; i<q; i++){
            double zero = 0.;
            for(int j=0; j<d; j++){
                zero += m1[p][j] * m2[j][i];
            }
            tmp.push_back(zero);
        }
        m.push_back(tmp);
        tmp.clear();
    }

    return m;
}

vector<double> pmatrix_vector_mult(vector<vector<double>> m1,vector<double> m2, int n, int d){

    vector<double> tmp;
    for(int p=0; p<n; p++){
        double zero = 0.;
        for(int i=0; i<d; i++){
           zero += m1[p][i] * m2[i];
        }
        tmp.push_back(zero);
    }

    return tmp;
}

vector<vector<double>> ptranspose(vector<vector<double>> m, int n, int d){

    vector<vector<double>> transpose(d,vector<double>(n,0.));
    for(int i=0; i<d; i++){
        for(int j=0; j<n; j++){
            transpose[i][j] = m[j][i];
        }
    }

    return transpose;
}

double pinverse(double ct){

    double c = 1. - ct;
    double v = 2. - ct;

    for(int i=0; i<5; i++){
        c = c * c;
        v = v * (1. + c);
    }

    return v;
}

double proot(double ct){

    double a = ct;
    double b = ct - 1.;
    for(int i=0; i<5; i++){
        a = a * (1. - 0.5 * b);
        b = (b*b) * ((b - 3.)/4.);
    }
    a = a * (1. - 0.5 * b);

    return a;
}

double pinner_product(vector<double> m1, vector<double> m2, int d){

    double ip = 0.;
    for(int i=0; i<d; i++){
        ip += m1[i] * m2[i];
    }

    return ip;
}

vector<vector<double>> kernelize(vector<vector<double>> x, int n, int d){

    vector<vector<double>> kernel;
    vector<double> tmpv;
    for(int i=0; i<n; i++){
        for(int j=0; j<i; j++){
            tmpv.push_back(x[0][0]);
        }
        for(int j=i; j<n; j++){
            tmpv.push_back(pinner_product(x[i],x[j],d));
        }
        kernel.push_back(tmpv);
        tmpv.clear();
    }
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            kernel[j][i] = kernel[i][j];
        }
    }

    return kernel;
}

vector<vector<vector<double>>> qr_factorize(vector<vector<double>> sigma, int d){

    vector<vector<vector<double>>> qr;
    vector<vector<double>> u;
    vector<double> ptmpv;
    auto sigmac = sigma;
    for(int i=0; i<d; i++){
        auto tmp = sigma[0][i]/sqrt(pinner_product(sigma[0],sigma[0],d));
        ptmpv.push_back(tmp);
    }
    u.push_back(ptmpv);
    ptmpv.clear();
    for(int i=1; i<d; i++){
        for(int j=0; j<d; j++){
            for(int q=0; q<i; q++){
                sigmac[i][j] -= pinner_product(sigma[i],u[q],d)*u[q][j];   
            }
        }
        auto div = sqrt(pinner_product(sigmac[i],sigmac[i],d));
        for(int j=0; j<d; j++){
            sigmac[i][j] /= div;
        }
        u.push_back(sigmac[i]);
    }
    auto ut = ptranspose(u,d,d);
    auto r = pmatrix_mult(ut,sigma,d,d,d);
    qr.push_back(u);
    qr.push_back(r);

    return qr;
}
