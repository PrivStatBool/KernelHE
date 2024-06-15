#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <math.h>
//#include "stdlib.h"


int main() {

    printf("Step1 start\n");

    // key setting process //
    // generate a keyset //
    const int minimum_lambda = 110;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);

    // generate a random key //
    uint32_t seed[] = { 314, 1592, 657 };
    tfhe_random_generator_setSeed(seed,3);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

    // export the secret key to file for later use //
    FILE* secret_key = fopen("secret.key","wb");
    export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
    fclose(secret_key);

    // export the cloud key to a file (for the cloud) //
    FILE* cloud_key = fopen("cloud.key","wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);

    /////////////////////////////////////////////////////////////////////////////
    // initial value //
    int length = 16;
    int N = 100;
    int k = 3;

    // generate encrypt the data //
    int plaintext1[N][N];

    FILE* data = fopen("KernelKmeansdata.txt","rb");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
	        fscanf(data, "%d ", &plaintext1[i][j]);
        }
    }
    fclose(data);

    LweSample* ciphertext1[N][N];
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
	        ciphertext1[i][j] = new_gate_bootstrapping_ciphertext_array(length, params);
        }
    }


    // encryption plaintext1 //
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
	    	for (int p = 0; p < length; p++){
                bootsSymEncrypt(&ciphertext1[i][j][p], (plaintext1[i][j]>>p)&1, key);
            }
        }
    }


    // generate encrypt the initial cluster mean //
    int plaintext2[N][k];

    FILE* cluster = fopen("KernelKmeanscluster.txt","rb");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < k; j++){
	        fscanf(cluster, "%d ", &plaintext2[i][j]);
        }
    }
    fclose(cluster);


    LweSample* ciphertext2[N];

    for(int i = 0; i < N; i++){
        ciphertext2[i] = new_gate_bootstrapping_ciphertext_array(k, params);
        
    }

    // encryption plaintext2 //
    for (int i = 0; i < N; i++){
        for (int j = 0; j < k; j++){
            bootsSymEncrypt(&ciphertext2[i][j], (plaintext2[i][j])&1, key);
        }
    }


    
    // export the N x length ciphertexts  = data //
    FILE* cloud_data = fopen("cloud.data","wb");
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int p = 0; p < length; p++){
	            export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext1[i][j][p], params);
            }
        }
    }
    
    // export the k x length ciphertexts = initial cluster mean //
    for (int i = 0; i < N; i++){
        for (int j = 0; j < k; j++){
//      	   	printf("%d %d %d\n", i,j,0);
	        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext2[i][j], params);
        }
    }
    fclose(cloud_data);

    //clean up all pointers //
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            delete_gate_bootstrapping_ciphertext_array(length, ciphertext1[i][j]);
       }
    }

    for(int i = 0; i < N; i++){
        delete_gate_bootstrapping_ciphertext_array(k, ciphertext2[i]);
        
    }

    //clean up all pointers
    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);

}
