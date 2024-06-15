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
    int length = 16;
    int N = 64;
    int k = 3;
    int d = 3;

    LweSample* answer[k][d];

    for(int i = 0; i < k; i++){
        for(int j = 0; j < d; j++){
	        answer[i][j] = new_gate_bootstrapping_ciphertext_array(length, params);}}
        
    //import the res_number x res_length ciphertexts from the answer file
    FILE* answer_data = fopen("answer.data","rb");
    for(int i = 0; i < k; i++){
        for(int j = 0; j < d; j++){
	        for (int p = 0; p < length; p++){
	        import_gate_bootstrapping_ciphertext_fromFile(answer_data, &answer[i][j][p], params);}}}
    fclose(answer_data);
    
        
    // int_answer initial value setting //
    int16_t int_answer[k][d];

    for (int i = 0; i < k; i++){
        for (int j = 0; j < d; j++){
            int_answer[i][j] = 0;}}

    // decrypt and rebuild the plaintext answer //
    for (int i = 0; i < k; i++){
        for (int j = 0; j < d; j++){
    	    for (int p = 0; p < length; p++){
        	    int ai = bootsSymDecrypt(&answer[i][j][p], key);
	            int_answer[i][j] |= (ai<<p);}}}
    

    for (int i = 0; i < k; i++){
        printf("new cluster mean u[%d] is: (%d, %d, %d) \n", i, int_answer[i][0], int_answer[i][1], int_answer[i][2]);}


    // save result to file //
//    FILE* KmeansCipherResult = fopen("KmeansCipherResult.txt","w");
//    fprintf(KmeansCipherResult, "%d, %d, %d \n", int_answer[0], int_answer[1], int_answer[2]);
//    fclose(KmeansCipherResult);
    

    // clean up all pointers //
    for (int i = 0; i < k; i++){
        for (int j = 0; j < d; j++){
            delete_gate_bootstrapping_ciphertext_array(length, answer[i][j]);}}

    delete_gate_bootstrapping_secret_keyset(key);
}
