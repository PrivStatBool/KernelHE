#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>
#include <HomOper.c>
#include <HomMultiDiv.c>
#include <HomKmeans_new.c>


int main() {
    for(int NN = 13; NN < 20; NN++){
    int N = 5*NN;
    printf("%d - Step2 start\n", N);

    // reads the cloud key from file //
    FILE* cloud_key = fopen("cloud.key","rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
    fclose(cloud_key);

    // if necessary, the params are inside the key //
    const TFheGateBootstrappingParameterSet* params = bk->params;


    // initial value // 
//    int N = 10; 
    int k = 3;
    int length = 16;
    int d = 10;
    
    LweSample* ciphertext1[N][d], *ciphertext2[N], *result[N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < d; j++){
	        ciphertext1[i][j] = new_gate_bootstrapping_ciphertext_array(length, params);
        }
    }
    for(int i = 0; i < N; i++){
        ciphertext2[i] = new_gate_bootstrapping_ciphertext_array(k, params);
    }

    // reads the N x length ciphertexts from the cloud file  = data//
    // reads the k x length ciphertexts from the cloud file  = cluster mean//
    FILE* cloud_data = fopen("cloud.data","rb");
        
    for (int i = 0; i < N; i++){
        for(int j = 0; j < d; j++){
	        for (int p = 0; p < length; p++){
		        import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext1[i][j][p], params);
            }
        }
    }
                    

    for (int i = 0; i < N; i++){
        for(int j = 0; j < k; j++){
	        import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext2[i][j], params);
        }
    }
    fclose(cloud_data);

    // result pointer //
    for(int i = 0; i < N; i++){
        result[i] = new_gate_bootstrapping_ciphertext_array(k, params);
    }

    // start time check //
    float time = -clock();

    //////////////////////////////
    ////// start operation ///////

    HomKmeans_new(result, ciphertext1, ciphertext2, d, length, N, k, bk);

    ////// end operation /////////
    //////////////////////////////

    // end time check //    
    time += clock();
    time = time/(CLOCKS_PER_SEC);

    printf("done in %f seconds...\n", time);

    // export the res_number x res_length ciphertexts to a file //
    FILE* answer_data = fopen("answer.data","wb");
    for (int i = 0; i < N; i++){
	    for (int j = 0; j < k; j++){
            export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i][j], params);
        }
    }
    fclose(answer_data);
    
    // clean up all pointers //
    for(int i = 0; i < N; i++){
        for(int j = 0; j < d; j++){
	        delete_gate_bootstrapping_ciphertext_array(length, ciphertext1[i][j]);
        }
    }

    for(int i = 0; i < N; i++){
        delete_gate_bootstrapping_ciphertext_array(k, ciphertext2[i]);
    }

    for(int i = 0; i < N; i++){
        delete_gate_bootstrapping_ciphertext_array(k, result[i]);
    }

    
    delete_gate_bootstrapping_cloud_keyset(bk);
    }
}



//	int row = sizeof(ciphertext1)/sizeof(ciphertext1[0]);
//	int col = sizeof(ciphertext1[0])/sizeof(ciphertext1[0][0]);
//	printf("%d, %d\n", row, col);
//    int row2 = sizeof(ciphertext2)/sizeof(ciphertext2[0]);
//	int col2 = sizeof(ciphertext2[0])/sizeof(ciphertext2[0][0]);
//	printf("%d, %d\n", row2, col2);