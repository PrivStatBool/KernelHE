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
double pinverse(double ct);
double proot(double ct);
double pinner_product(vector<double> m1, vector<double> m2, int d);
vector<vector<double>> kernelize(vector<vector<double>> x, int n, int d);

int main() {
    
    srand(time(NULL));
    int q = 2;
    int n = 10;
    int d = 5;
    int nd = max(n,d);
    double lr = 0.01;
    cout<<"n: "<<n <<endl;
    cout<<"d: "<<d <<endl;

    vector<double> z = {0.};
    vector<vector<double>> x;
    vector<double> ptmpv;
    for(int i=0; i<n; i++){
        for(int j=0; j<d; j++){
            double tmp = rand() % 10 - 5.;
            ptmpv.push_back(tmp);
        }
        x.push_back(ptmpv);
        ptmpv.clear();
    }
    vector<double> y;
    for(int i=0; i<n; i++){
        double tmp = rand() % 20 - 10.;
        y.push_back(tmp);
    }
    cout<<"x: "<<x<<endl;
    cout<<"y: "<<y<<endl;
    //svm
    vector<double> divisors;
    for(int i=0; i<n; i++){
        divisors.push_back(2.*pinner_product(x[i],x[i],d));
    }
    system_clock::time_point start_time1 = system_clock::now();
    vector<double> alpha;
    for(int i=0; i<n; i++){
        alpha.push_back(0.);
    }
    vector<double> pinverse_product;
    for(int i=0; i<n; i++){
        pinverse_product.push_back(pinverse(pinner_product(x[i],x[i],d)/divisors[i])/divisors[i]);
    }
    for(int p=0; p<q; p++){
        for(int i=0; i<n; i++){
            double tmp = 0.;
            for(int j=0; j<n; j++){
                tmp += alpha[j]*y[j]*pinner_product(x[j],x[i],d);
            }
            alpha[i] += lr*(1.-y[i]*tmp); 
            alpha[i] *= pinverse_product[i];
        }
    }
    vector<double> w;
    for(int i=0; i<d; i++){
        double tmp = 0.;
        for(int j=0; j<n; j++){
            tmp += (alpha[j]*y[j])*x[j][i];
        }
        w.push_back(tmp);
    }
    cout<<"w: "<<w<<endl;
    double b = 0.;
    for(int i=0; i<n; i++){
        b += y[i] - pinner_product(w,x[i],d);
    }
    b /= (double)n;
    cout<<"b: "<<b<<endl;
   
    system_clock::time_point end_time1 = system_clock::now();

    nanoseconds nano1 = end_time1 - start_time1;
    microseconds micro1 = duration_cast<microseconds>(end_time1 - start_time1);
    cout << "time (Plain_nonkernel) : " <<nano1.count() << " nanos" << endl;
    cout << "time (Plain_nonkernel) : " <<micro1.count() << " micros " << endl;
    
    //kernel svm
    auto kernel = kernelize(x,n,d);
    vector<vector<vector<double>>> naive_kernel;
    vector<vector<double>> tmpvv;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            ptmpv.push_back(kernel[i][j]);
            tmpvv.push_back(ptmpv);
            ptmpv.clear();
        }
        naive_kernel.push_back(tmpvv);
        tmpvv.clear();
    }
    system_clock::time_point start_time2 = system_clock::now();
    
    vector<double> kalpha;
    for(int i=0; i<n; i++){
        kalpha.push_back(0.);
    }
    vector<double> kpinverse_product;
    for(int i=0; i<n; i++){
        kpinverse_product.push_back(pinverse(kernel[i][i]/divisors[i])/divisors[i]);
    }
    for(int p=0; p<q; p++){
        for(int i=0; i<n; i++){
            double tmp = 0.;
            for(int j=0; j<n; j++){
                tmp += kalpha[j]*y[j]*kernel[j][i];
            }
            kalpha[i] += lr*(1.-y[i]*tmp); 
            kalpha[i] *= kpinverse_product[i];
        }
    }
    vector<double> kw;
    for(int i=0; i<d; i++){
        double tmp = 0.;
        for(int j=0; j<n; j++){
            tmp += (kalpha[j]*y[j])*x[j][i];
        }
        kw.push_back(tmp);
    }
    cout<<"kw: "<<kw<<endl;

    double kb = 0.;
    for(int i=0; i<n; i++){
        kb += y[i] - pinner_product(kw,x[i],d);
    }
    kb /= (double)n;
    cout<<"kb: "<<kb<<endl;

    system_clock::time_point end_time2 = system_clock::now();

    nanoseconds nano2 = end_time2 - start_time2;
    microseconds micro2 = duration_cast<microseconds>(end_time2 - start_time2);
    cout << "time (Plain_kernel) : " <<nano2.count() << " nanos" << endl;
    cout << "time (Plain_kernel) : " <<micro2.count() << " micros" << endl;

    uint32_t batchSize = 64;
    CCParams<CryptoContextCKKSRNS> parameters;
    SecretKeyDist secretKeyDist = UNIFORM_TERNARY;
    parameters.SetMultiplicativeDepth(110);
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
    cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << endl << endl;
    auto keys = cc->KeyGen();

    cc->EvalMultKeyGen(keys.secretKey);

    vector<int> ro;
    for(int i=1; i<nd; i++){
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
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > cy;
    cy.push_back(yy);
    for(int i=1; i<n; i++){
        auto tmp = cc->EvalRotate(yy,i);
        cy.push_back(tmp);
    }
    p[0] = cc->MakeCKKSPackedPlaintext(z);
    auto zz = cc->Encrypt(keys.publicKey, p[0]);

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
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    
    system_clock::time_point start_time3 = system_clock::now();
    
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > calpha;
    for(int i=0; i<n; i++){
        calpha.push_back(zz);
    }
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > cinverse_product;
    for(int i=0; i<n; i++){
        cinverse_product.push_back(cc->EvalMult(inverse(cc->EvalMult(inner_product(ctxs[i],ctxs[i],d,cc),1./divisors[i]),cc),1./divisors[i]));
    }
    for(int p=0; p<q; p++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                tmpv.push_back(cc->EvalMult(cc->EvalMult(calpha[j],cy[j]),inner_product(ctxs[j],ctxs[i],d,cc)));
            }
            auto tmp = add_many(tmpv,n,cc);
            tmp = cc->EvalSub(1.,cc->EvalMult(cy[i],tmp));
            calpha[i] = cc->EvalAdd(calpha[i],cc->EvalMult(tmp,0.01)); 
            calpha[i] = cc->EvalMult(calpha[i],cinverse_product[i]);
            tmpv.clear();
        }
    }
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > alphay;
    for(int i=0; i<n; i++){
        alphay.push_back(cc->EvalMult(calpha[i],cy[i]));
        alphay[i] = cc->EvalMult(alphay[i],diag[0]);
    }

    for(int i=0; i<n; i++){
        tmpv.push_back(alphay[i]);
        for(int j=1; j<d; j++){
            auto tmp = cc->EvalRotate(alphay[i],-j);
            tmpv.push_back(tmp);
        }
        alphay[i] = add_many(tmpv,d,cc);
        tmpv.clear();  
    }
    for(int i=0; i<n; i++){
        tmpv.push_back(cc->EvalMult(alphay[i],ctxs[i]));
    }
    auto cw = add_many(tmpv,n,cc);
    tmpv.clear();
    for(int i=0; i<n; i++){
        auto tmp = cc->EvalSub(cy[i],inner_product(cw,ctxs[i],d,cc));
        tmpv.push_back(tmp);
    }
    auto cb = cc->EvalMult(add_many(tmpv,n,cc),1./(double)n);
    tmpv.clear();
    auto sss = cc->EvalMult(ctxs[0],ctxs[1]);
    system_clock::time_point end_time3 = system_clock::now();

    cout << "level: " << cb->GetLevel() << endl;

    cc->Decrypt(keys.secretKey, cw, &result);
    result->SetLength(d);
    cout << "cw = " << result;
    cc->Decrypt(keys.secretKey, cb, &result);
    result->SetLength(d);
    cout << "cb = " << result;
    microseconds micro3 = duration_cast<microseconds>(end_time3 - start_time3);
    cout << "time : " <<micro3.count()/1000000. << " s" << endl;

    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> ckernel;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            p[j] = cc->MakeCKKSPackedPlaintext(naive_kernel[i][j]);
            auto c = cc->Encrypt(keys.publicKey, p[j]);
            tmpv.push_back(c);
        }
        ckernel.push_back(tmpv);
        tmpv.clear();
    }
    system_clock::time_point start_time4 = system_clock::now();

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > kcalpha;
    for(int i=0; i<n; i++){
        kcalpha.push_back(zz);
    }
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > kcinverse_product;
    for(int i=0; i<n; i++){
        kcinverse_product.push_back(cc->EvalMult(inverse(cc->EvalMult(ckernel[i][i],1./divisors[i]),cc),1./divisors[i]));
    }
    for(int p=0; p<q; p++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                tmpv.push_back(cc->EvalMult(cc->EvalMult(kcalpha[j],cy[j]),ckernel[j][i]));
            }
            auto tmp = add_many(tmpv,n,cc);
            tmp = cc->EvalSub(1.,cc->EvalMult(cy[i],tmp));
            kcalpha[i] = cc->EvalAdd(kcalpha[i],cc->EvalMult(tmp,0.01)); 
            kcalpha[i] = cc->EvalMult(kcalpha[i],kcinverse_product[i]);
            tmpv.clear();
        }
    }
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > kalphay;
    for(int i=0; i<n; i++){
        kalphay.push_back(cc->EvalMult(kcalpha[i],cy[i]));
        kalphay[i] = cc->EvalMult(kalphay[i],diag[0]);
    }
    for(int i=0; i<n; i++){
        tmpv.push_back(kalphay[i]);
        for(int j=1; j<d; j++){
            auto tmp = cc->EvalRotate(kalphay[i],-j);
            tmpv.push_back(tmp);
        }
        kalphay[i] = add_many(tmpv,d,cc);
        tmpv.clear();  
    }
    for(int i=0; i<n; i++){
        tmpv.push_back(cc->EvalMult(kalphay[i],ctxs[i]));
    }
    auto kcw = add_many(tmpv,n,cc);
    tmpv.clear();
    for(int i=0; i<n; i++){
        auto tmp = cc->EvalSub(cy[i],inner_product(kcw,ctxs[i],d,cc));
        tmpv.push_back(tmp);
    }
    auto kcb = cc->EvalMult(add_many(tmpv,n,cc),1./(double)n);

    cout << "level: " << kcb->GetLevel() << endl;
    cc->Decrypt(keys.secretKey, kcw, &result);
    result->SetLength(d);
    cout << "kcw = " << result;
    cc->Decrypt(keys.secretKey, kcb, &result);
    result->SetLength(d);
    cout << "kcb = " << result;

    system_clock::time_point end_time4 = system_clock::now();
    microseconds micro4 = duration_cast<microseconds>(end_time4 - start_time4);
    cout << "time : " <<micro4.count()/1000000. << " s" << endl;

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
