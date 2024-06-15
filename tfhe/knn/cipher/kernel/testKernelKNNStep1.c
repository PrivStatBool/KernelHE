#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <math.h>

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
    
    //////////////////////////////////////////////////////////////
    // initial value //
    int length = 16;
    int N = 100;
    int k = 3;
    int test_N = 1;
    int d = 10;
    int loop = 10;
    // generate encrypt the data //
    // plaintext1 = data //
    // plaintext2 = label //
    // plaintext3 = new data //
    int plaintext1[(N+test_N)*(N+test_N)];
    
    int plaintext2[N];

    FILE* data = fopen("KernelKNNdata.txt","rb");
    for(int i = 0; i < (N+test_N)*(N+test_N); i++)
    {
        fscanf(data, "%d ", &plaintext1[i]);
    }
    fclose(data);

    FILE* label = fopen("KernelKNNlabel.txt","rb");
    for(int i = 0; i < N; i++)
    {
        fscanf(label, "%d ", &plaintext2[i]);
    }
    fclose(label);

       


    LweSample* ciphertext1[(N+test_N)*(N+test_N)];
    LweSample* ciphertext2[N];
    

    // encryption plaintext1 //
    for(int i = 0; i < (N+test_N)*(N+test_N); i++){
	    ciphertext1[i] = new_gate_bootstrapping_ciphertext_array(length, params);
    }
    for (int i = 0; i < (N+test_N)*(N+test_N); i++){
	   for (int j = 0; j < length; j++) {
            bootsSymEncrypt(&ciphertext1[i][j], (plaintext1[i]>>j)&1, key);
        }
    }

    // encryption plaintext2 //
    for(int i = 0; i < N; i++){
	    ciphertext2[i] = new_gate_bootstrapping_ciphertext_array(k, params);
    }
    for (int i = 0; i < N; i++){
	   for (int j = 0; j < k; j++) {
            bootsSymEncrypt(&ciphertext2[i][j], (plaintext2[i]>>j)&1, key);
        }
    }


    printf("Let's start homomorphic KernelKNN algorithms\n\n");
    
    // export the N*d x length ciphertexts  = data //
    FILE* cloud_data = fopen("cloud.data","wb");
    for (int i = 0; i < (N+test_N)*(N+test_N); i++){
	    for (int j = 0; j < length; j++){
	        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext1[i][j], params);}}

    // export the N x label_length ciphertexts  = label //
    for (int i = 0; i < N; i++){
	    for (int j = 0; j < k; j++){
	        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext2[i][j], params);}}

    fclose(cloud_data);

    //clean up all pointers
    for(int i = 0; i < (N+test_N)*(N+test_N); i++)
	    delete_gate_bootstrapping_ciphertext_array(length, ciphertext1[i]);
    for(int i = 0; i < N; i++)
	    delete_gate_bootstrapping_ciphertext_array(k, ciphertext2[i]);

    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);

}