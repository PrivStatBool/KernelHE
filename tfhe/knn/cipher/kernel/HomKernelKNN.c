#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>

 void HomKernelKNN(LweSample* res[100], LweSample* a[200], LweSample* l[100], const int length, const int N, const int test_N, const int k, const TFheGateBootstrappingCloudKeySet* bk){



 	LweSample *K[(N+test_N)*(N+test_N)], *L[N], *D[test_N][N], *NL[k];

	// A = data copy // 
	// B = dimensional distance with data and new_data // 
	// C = distance with data and new_data // 
	for(int i = 0; i < (N+test_N)*(N+test_N); i++){
 		K[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	}

 	for(int i = 0; i < N; i++){
 		L[i] = new_gate_bootstrapping_ciphertext_array(k, bk->params);
	}

	for(int i = 0; i < test_N; i++){
		for(int j = 0; j < N; j++){
 			D[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		}
	}

	for(int i = 0; i < k; i++){
 		NL[i] = new_gate_bootstrapping_ciphertext_array(k, bk->params);
	}

	// variable for labelling //
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(length, bk->params);





	///// copy data /////
	// a[N*d][length] to A[N*d][length] //
	printf("// data copy\n");
 	for(int i = 0; i < (N+test_N)*(N+test_N); i++){
 		for(int j = 0; j < length; j++){
 			bootsCOPY(&K[i][j], &a[i][j], bk);
 		}
 	}

 	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
 			bootsCOPY(&L[i][j], &l[i][j], bk);
 		}
 	}




	printf("// calculate distance_with kernel\n");
	////// data - New data /////
 	for(int i = 0; i < test_N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			HomAdd(D[i][j], K[j*(N+test_N)+j], K[(N+i)*(N+test_N)+(N+i)], length, bk);
			HomRShift(temp, K[(N+i)*(N+test_N)+j], length, 1, bk);
			HomAdd(D[i][j], D[i][j], temp, length, bk);

		}
 	}


	for(int s = 0; s < test_N; s++){
		printf("// small k - sorting algorithm\n");
		// find k small distance //
		for(int i = 0; i < k; i++){
			for(int j = i + 1; j < N; j++){
				HomCompL(&temp[0], D[s][i], D[s][j], length, bk);
				HomSwap(temp[0], D[s][i], D[s][j], length, bk);
				HomSwap(temp[0], L[i], L[j], k, bk);
			}
		}



		printf("// find mode of sorted label\n");
		// find mode of sorted label //
		for(int i = 0; i < k; i++){
			bootsCONSTANT(&temp[i], 0, bk);
		}

		for(int i = 0; i < k; i++){
			for(int j = 0; j < k; j++){
				bootsCONSTANT(&NL[i][j], 0, bk);
			}
		}

		for(int i = 0; i < k; i++){
			for(int j = i+1; j < k; j++){
				HomEqui(&temp[0], L[i], L[j], k, bk);
				HomAdd(NL[i], NL[i], temp, k, bk);
			}
		}

		for(int i = 0; i < k; i++){
			for(int j = i + 1; j < k; j++){
				HomCompL(&temp[0], NL[i], NL[j], k, bk);
				HomSwap(temp[0], NL[i], NL[j], k, bk);
				HomSwap(temp[0], L[i], L[j], k, bk);
			}
		}

		for(int i = 0; i < k; i++){
			bootsCOPY(&res[s][i], &L[0][i], bk);
		}
	}


 	//////////////////////////////////////////////////////////


 	//////////////////////////////////////////////////////////


/*
	// result //
	bootsCOPY(&res[0], &E[1], bk);

*/
 	//clean up all pointers //
 	for(int i = 0; i < (N+test_N)*(N+test_N); i++){
 		delete_gate_bootstrapping_ciphertext_array(length, K[i]);
	}
	for(int i = 0; i < N; i++){
 		delete_gate_bootstrapping_ciphertext_array(k, L[i]);
	}
	for(int i = 0; i < test_N; i++){
		for(int j = 0; j < N; j++){
 			delete_gate_bootstrapping_ciphertext_array(length, D[i][j]);
		}
	}
	for(int i = 0; i < k; i++){
 		delete_gate_bootstrapping_ciphertext_array(k, NL[i]);
	}
 	
 	delete_gate_bootstrapping_ciphertext_array(length, temp);
 }

