#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <HomOper.c>
#include <HomMultiDiv.c>
#include <HomKernelKNN.c>


int main() {
    
    
    for(int NN = 6; NN < 20; NN++){
    printf("%d - Step2 start\n", 5*NN);
    // reads the cloud key from file //
    FILE* cloud_key = fopen("cloud.key","rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
    fclose(cloud_key);
 
    // if necessary, the params are inside the key //
    const TFheGateBootstrappingParameterSet* params = bk->params;

    // initial value // 
    int N = 5*NN;
    int d = 10; 
    int k = 3;
    int length = 16;

    int test_N = 1;

    LweSample* ciphertext1[(N+test_N)*(N+test_N)], *ciphertext2[N], *result[test_N];

    for(int i = 0; i < (N+test_N)*(N+test_N); i++){
	    ciphertext1[i] = new_gate_bootstrapping_ciphertext_array(length, params);}
    for(int i = 0; i < N; i++){
    	ciphertext2[i] = new_gate_bootstrapping_ciphertext_array(k, params);}

    // reads the N*d x length ciphertexts  = data //
    // reads the N x label_length ciphertexts  = label //
    // reads the d x length ciphertexts  = new data //
    FILE* cloud_data = fopen("cloud.data","rb");
    for (int i = 0; i < (N+test_N)*(N+test_N); i++){
        for (int j = 0; j < length; j++){
            import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext1[i][j], params);
        }
    }

    for (int i = 0; i < N; i++){
    	for (int j = 0; j < k; j++){
	        import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext2[i][j], params);
        }
    }    
    fclose(cloud_data);

printf(".......\n");
    // result pointer //
    for (int i = 0; i < test_N; i++){
        result[i] = new_gate_bootstrapping_ciphertext_array(k, params);}

    

    //////////////////////////////
    ////// start operation ///////

    

    
    /////////////////////////////////////////////////////////////
    
        
        printf("///////////////////////////////////////////////////////////\n");
        printf("///////////////// Kernel KNN algorithm start /////////////////\n");
        printf("///////////////////////////////////////////////////////////\n \n");
    // start time check //
        float time = -clock();

        HomKernelKNN(result, ciphertext1, ciphertext2, length, N, test_N, k, bk);

    // end time check //    
        time += clock();
        time = time/(CLOCKS_PER_SEC);

        printf("\ndone in %f seconds...\n \n", time);
        printf("///////////////////////////////////////////////////////////\n");
        printf("/////////////// Kernel KNN algorithm completed ///////////////\n");
        printf("///////////////////////////////////////////////////////////\n \n");

    
    

    ////// end operation ///////
    //////////////////////////////
    


    // export the label_length ciphertexts to a file //
    FILE* answer_data = fopen("answer.data","wb");
    for (int j = 0; j < test_N; j++){
        for (int i = 0; i < k; i++){
	        export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[j][i], params);}}
    fclose(answer_data);

    // clean up all pointers //
    for(int i = 0; i < (N+test_N)*(N+test_N); i++){
	    delete_gate_bootstrapping_ciphertext_array(length, ciphertext1[i]);}
    for(int i = 0; i < N; i++){
	    delete_gate_bootstrapping_ciphertext_array(k, ciphertext2[i]);}
    for(int i = 0; i < test_N; i++){
        delete_gate_bootstrapping_ciphertext_array(k, result[i]);}
    }
    
}



