#define PROFILE

#include "openfhe.h"

using namespace std;
using namespace lbcrypto;


vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >>
naive_ctxs(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > ctxs, int n, CryptoContext<DCRTPoly> cc) {
        
    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> nctxs;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmp;
    for(int i=0; i<n; i++){
        tmp.push_back(ctxs[i]);
        for(int j=1; j<n; j++){
            tmp.push_back(cc->EvalRotate(ctxs[i],j));
        }
        nctxs.push_back(tmp);
        tmp.clear();
    }

    return nctxs;
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > 
add_many(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ctxs, int n, CryptoContext<DCRTPoly> cc) {

    auto res = cc->EvalAdd(ctxs[0],ctxs[1]);
    if (n > 2){
        for(int i=2; i<n; i++){
            res = cc->EvalAdd(res,ctxs[i]);
        }
    }
    return res;
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > 
inner_product(Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > c1, Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > c2, int d, CryptoContext<DCRTPoly> cc) {

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    tmpv.push_back(cc->EvalMult(c1,c2));
    for(int i=1; i<d; i++){
        auto tmp = cc->EvalRotate(tmpv[0],i);
        tmpv.push_back(tmp);
    }
    auto res = add_many(tmpv,d,cc);
    
    return res;
}

vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> 
naive_matrix_mult(vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> c1, vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> c2, int n, CryptoContext<DCRTPoly> cc) {

    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > >> v;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmp1;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmp2;

    for(int p=0; p<n; p++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                tmp1.push_back(cc->EvalMult(c1[p][j],c2[j][i]));
            }
            tmp2.push_back(add_many(tmp1,n,cc));
            tmp1.clear();
        }
        v.push_back(tmp2);
        tmp2.clear();
    }

    return v;
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > 
inverse(Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > ct, CryptoContext<DCRTPoly> cc) {
    
    auto ct1 = cc->EvalSub(1.0,ct);
    auto ct2 = cc->EvalAdd(ct1,1.0);

    for(int i=0; i<5; i++){
        ct1 = cc->EvalMult(ct1,ct1);
        ct2 = cc->EvalMult(ct2,cc->EvalAdd(ct1,1.0));
    }

    return ct2;
}

vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> 
naive_matrix_inverse(vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> a, int n, CryptoContext<DCRTPoly> cc){
    
    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> transpose;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> tmp;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            tmp.push_back(a[j][i]);
        }
        transpose.push_back(tmp);
        tmp.clear();
    }

    auto c = naive_matrix_mult(a,transpose,n,cc);

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ct;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            tmp.push_back(c[j][i]);
        }
        ct.push_back(add_many(tmp,n,cc));
        tmp.clear();
    }

    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> mv5;
    mv5.push_back(ct);
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> mv;
    for(int p=0; p<4; p++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                tmp.push_back(cc->EvalMult(c[i][j],mv5[p][j]));
            }
            mv.push_back(add_many(tmp,n,cc));
            tmp.clear();
        }
        mv5.push_back(mv);
        mv.clear();
    }

    auto alpha = inverse(mv5[4][0], cc);
    alpha = cc->EvalMult(alpha,mv5[3][0]); 

    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> b;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            tmp.push_back(cc->EvalMult(alpha,transpose[i][j]));
        }
        b.push_back(tmp);
        tmp.clear();
    }

    vector<vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>> tmp1;
    for(int p=0; p<10; p++){
        auto mult = naive_matrix_mult(a,b,n,cc);
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                tmp.push_back(-mult[i][j]);
            }
            tmp1.push_back(tmp);
            tmp.clear();
        }
        for(int i=0; i<n; i++){
            tmp1[i][i] = cc->EvalAdd(tmp1[i][i],2.0);
        }
        b = naive_matrix_mult(b,tmp1,n,cc);
        tmp1.clear();
    }

    return b;
}

vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > 
transpose(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ctxs, vector<Plaintext> diag, int n, CryptoContext<DCRTPoly> cc){
    
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > ctxst;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            auto tmp = cc->EvalMult(ctxs[j],diag[i]);
            if(j < i){
                tmp = cc->EvalRotate(tmp,i-j);
            }
            else if(j > i){
                tmp = cc->EvalRotate(tmp,-j+i);
            }
            tmpv.push_back(tmp);
        }
        auto tmp = add_many(tmpv,n,cc);
        tmpv.clear();
        ctxst.push_back(tmp);
    }

    return ctxst;
}

vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > 
diagonalize(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ctxst, vector<Plaintext> diag, int n, CryptoContext<DCRTPoly> cc){

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > dev;
    for(int i=0; i<n; i++){
        auto tmp = cc->EvalMult(ctxst[i],diag[i]);
        tmpv.push_back(tmp);
    }
    auto tmp = add_many(tmpv,n,cc);
    dev.push_back(tmp);
    tmpv.clear();

    for(int i=1; i<n; i++){
        for(int j=0; j<n; j++){
            auto tmp = cc->EvalMult(ctxst[j],diag[(n+j-i)%n]); 
            tmpv.push_back(tmp);
        }
        auto tmp = add_many(tmpv,n,cc);
        dev.push_back(tmp);
        tmpv.clear();
    }

    return dev;
}

vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > 
matrix_mult(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> dev, vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ctxst, int n, CryptoContext<DCRTPoly> cc){

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > matrix_mult;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(j==0){
                auto tmp = cc->EvalMult(dev[j],ctxst[i]);
                tmpv.push_back(tmp);
            }
            else{
                auto tmp = cc->EvalRotate(ctxst[i],j);
                auto tmp1 = cc->EvalRotate(ctxst[i],-n+j);
                tmp1 = cc->EvalAdd(tmp,tmp1);
                tmp = cc->EvalMult(dev[j],tmp1);
                tmpv.push_back(tmp); 
            }
        }
        auto tmp = add_many(tmpv,n,cc);
        tmpv.clear();
        matrix_mult.push_back(tmp);
    }

    return matrix_mult;
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >
matrix_vector_mult(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> dev, Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > ctxt, int n, CryptoContext<DCRTPoly> cc){

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    for(int j=0; j<n; j++){
        if(j==0){
            auto tmp = cc->EvalMult(dev[j],ctxt);
            tmpv.push_back(tmp);
        }
        else{
            auto tmp = cc->EvalRotate(ctxt,j);
            auto tmp1 = cc->EvalRotate(ctxt,-n+j);
            tmp1 = cc->EvalAdd(tmp,tmp1);
            tmp = cc->EvalMult(dev[j],tmp1);
            tmpv.push_back(tmp); 
        }
    }
    auto matrix_vector_mult = add_many(tmpv,n,cc);
    tmpv.clear();

    return matrix_vector_mult;
    
}

vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >>
matrix_inverse(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> ctxs, int n, double dv, CryptoContext<DCRTPoly> cc){
    
    vector<Plaintext> p;
    for(int i=0; i<n; i++){
        Plaintext ptxt;
        p.push_back(ptxt);
    }
    vector<Plaintext> diag;
    for(int i=0; i<n; i++){
        vector<double> d;
        for(int j=0; j<n; j++){
            if(i == j){
                d.push_back(1.);
            }
            else{
                d.push_back(0.);
            }
        }
        p[i] = cc->MakeCKKSPackedPlaintext(d);
        diag.push_back(p[i]);
        d.clear();
    }

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;
    
    auto ctxst = transpose(ctxs,diag,n,cc);

    auto c = matrix_mult(diagonalize(ctxst,diag,n,cc),ctxs,n,cc);
    auto c_diag = diagonalize(c,diag,n,cc);

    auto tmp = add_many(c,n,cc);
    tmpv.push_back(tmp);
    for(int i=0; i<4; i++){
        auto tmp = matrix_vector_mult(c_diag,tmpv[i],n,cc);
        tmpv.push_back(tmp);
    }
    
    auto binverse = cc->EvalMult(tmpv[4],1./dv);
    binverse = cc->EvalMult(binverse,diag[0]);
    auto alpha = cc->EvalMult(inverse(binverse,cc),tmpv[3]);
    alpha = cc->EvalMult(alpha,1./dv);
    alpha = cc->EvalMult(alpha,diag[0]);
    tmpv.clear();
    tmpv.push_back(alpha);
    for(int i=1; i<n; i++){
        auto tmp = cc->EvalRotate(alpha,-i);
        tmpv.push_back(tmp);
    }
    alpha = add_many(tmpv,n,cc);
    
    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > b;
    for(int i=0; i<n; i++){
        auto tmp = cc->EvalMult(alpha,ctxs[i]);
        b.push_back(tmp);
    }

    vector<Plaintext> diag2;
    for(int i=0; i<n; i++){
        vector<double> d;
        for(int j=0; j<n; j++){
            if(i == j){
                d.push_back(2.);
            }
            else{
                d.push_back(0.);
            }
        }
        p[i] = cc->MakeCKKSPackedPlaintext(d);
        diag2.push_back(p[i]);
        d.clear();
    }
    for(int i=0; i<15; i++){
        auto ab = matrix_mult(diagonalize(ctxst,diag,n,cc),b,n,cc);
        for(int j=0; j<n; j++){
            ab[j] = cc->EvalSub(diag2[j],ab[j]);
        }
        b = matrix_mult(diagonalize(b,diag,n,cc),ab,n,cc);
    }

    return b;
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >
power_method(vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > >> c, int n, CryptoContext<DCRTPoly> cc){
    
    vector<Plaintext> p;
    for(int i=0; i<n; i++){
        Plaintext ptxt;
        p.push_back(ptxt);
    }
    vector<Plaintext> diag;
    for(int i=0; i<n; i++){
        vector<double> d;
        for(int j=0; j<n; j++){
            if(i == j){
                d.push_back(1.);
            }
            else{
                d.push_back(0.);
            }
        }
        p[i] = cc->MakeCKKSPackedPlaintext(d);
        diag.push_back(p[i]);
        d.clear();
    }

    auto c_diag = diagonalize(c,diag,n,cc);

    vector<Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > > tmpv;

    auto tmp = add_many(c,n,cc);
    tmpv.push_back(tmp);
    for(int i=0; i<4; i++){
        auto tmp = matrix_vector_mult(c_diag,tmpv[i],n,cc);
        tmpv.push_back(tmp);
    }

    return tmpv[4];
}

Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > 
square_root(Ciphertext<lbcrypto::DCRTPolyImpl<bigintdyn::mubintvec<bigintdyn::ubint<unsigned int> > > > ct, CryptoContext<DCRTPoly> cc) {

    auto tmp = cc->EvalSub(ct,1.0);
    for(int i=0; i<5; i++){
        ct = cc->EvalMult(ct,cc->EvalSub(1.0,cc->EvalMult(tmp,0.5)));
        tmp = cc->EvalMult(cc->EvalMult(tmp,tmp),cc->EvalMult(cc->EvalSub(tmp,3.0),0.25));
    }
    ct = cc->EvalMult(ct,cc->EvalSub(1.0,cc->EvalMult(tmp,0.5)));

    return ct;
}
