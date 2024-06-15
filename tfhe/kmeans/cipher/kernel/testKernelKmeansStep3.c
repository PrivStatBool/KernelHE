#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>


int main() {

    printf("Step3 start\n");

    // reads the cloud key from file //
    FILE* secret_key = fopen("secret.key","rb");
    TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);
 
    // if necessary, the params are inside the key //
    const TFheGateBootstrappingParameterSet* params = key->params;

    // initial value //
    int N = 10;
    int k = 3;
    
    LweSample* answer[N];
    printf("Step3 start\n");

    for(int i = 0; i < N; i++){
        answer[i] = new_gate_bootstrapping_ciphertext_array(k, params);
    }

        printf("Step3 start\n");

    //import the res_number x res_length ciphertexts from the answer file
    FILE* answer_data = fopen("answer.data","rb");
    for(int i = 0; i < N; i++){
	    for (int j = 0; j < k; j++){
	        import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i][j], params);
        }
    }
    fclose(answer_data);
    

       printf("Step3 start\n");
 
    // int_answer initial value setting //
    int16_t int_answer[N][k];

    for (int i = 0; i < N; i++){
        for (int j = 0; j < k; j++){
            int_answer[i][j] = 0;
        }
    }
    printf("Step3 start\n");

    // decrypt and rebuild the plaintext answer //
    for (int i = 0; i < N; i++){
    	for (int j = 0; j < k; j++){
        	int ai = bootsSymDecrypt(&answer[i][j], key);
	        int_answer[i][j] |= (ai);
	    }
    }

    printf("new label of all data is: \n");
    for (int i = 0; i < N; i++){
        for (int j = 0; j < k; j++){
	        printf("%d ",int_answer[i][j]);
        }
	}
	printf("\n");


/*
    // save result to file //
    FILE* KmeansCipherResult = fopen("KmeansCipherResult.txt","w");
    fprintf(KmeansCipherResult, "%d %d %d \n", int_answer[0], int_answer[1], int_answer[2]);
    fclose(KmeansCipherResult);
    
*/
    // clean up all pointers //
    for (int i = 0; i < N; i++){
	    delete_gate_bootstrapping_ciphertext_array(k, answer[i]);
    }

    delete_gate_bootstrapping_secret_keyset(key);
}
