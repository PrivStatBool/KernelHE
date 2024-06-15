 void HomKernelKmeans(LweSample* res[64], LweSample *K[64][64], LweSample *L_0[64], const int length, const int N, const int k, const int iter, const TFheGateBootstrappingCloudKeySet* bk){


	LweSample *A[k][N][N], *SA[k][N], *LSA[k][N], *SLSA[k], *L[N];

	LweSample *x[k][N], *y[k], *u[k][N];
	LweSample *n[k];
	LweSample *temp[10];
	
	for(int i = 0; i < k; i++){
	 	for(int j = 0; j < N; j++){
 		 	for(int p = 0; p < N; p++){
		 		A[i][j][p] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
			}
		}
	}

	for(int i = 0; i < k; i++){
	 	for(int j = 0; j < N; j++){
	 		SA[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	 		LSA[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	 		x[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	 		u[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		}
	}
	
	for(int i = 0; i < N; i++){
		L[i] = new_gate_bootstrapping_ciphertext_array(k, bk->params);
	}
	for(int i = 0; i < k; i++){
		y[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		n[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		SLSA[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	}

	for(int i = 0; i < 10; i++){
		temp[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	}
	 		

	// initial label copy //
 	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
 			bootsCOPY(&L[i][j], &L_0[i][j], bk);
		}
	}

 	
	////////// start iteration ///////////
	int s = 0; 
	
	while(1){
	
		// count n[i] //
		for(int i = 0; i < k; i++){	
			for(int j = 0; j < length; j++){	
				bootsCONSTANT(&n[i][j], 0, bk);	
			}
		}
		for(int i = 0; i < 10; i++){	
			for(int j = 0; j < length; j++){	
				bootsCONSTANT(&temp[i][j], 0, bk);
			}
		}
		
		
	 	for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				bootsCOPY(&temp[i][length-1], &L[j][i], bk);
				HomAdd(n[i], n[i], temp[i], length, bk);
			}
		}
		
		// A[k][N][N][length] = K[N][N][length] AND L[N][k][0] //
	 	for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				for(int p = 0; p < N; p++){
					for(int q = 0; q < length; q++){
// error.... 			bootsAND(&A[i][j][p][q], &K[j][p][q], &L[j][i], bk);
						bootsAND(&A[i][j][p][q], &A[0][j][p][q], &L[j][i], bk);
//						printf("Step2-5 - %d - %d- %d - %d\n",i,j,p,q);	

					}
				}
			}
		}
		
		// initialize SA to 0 //		 			
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
				for(int p = 0; p < length; p++){
					bootsCONSTANT(&SA[i][j][p], 0, bk);
				}
			}
		}
		
		// SA[k][N] = sum(A[k][N][N]) //// x = 2 * SA //					
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
				for(int p = 0; p < N; p++){
	 				HomAdd(SA[i][j], A[i][j][p], SA[i][j], length, bk);}
	 			HomRShift(x[i][j], SA[i][j], length, 1, bk);
	 		}	
		}
		
		// LSA[k][N][length] = SA[k][N][length] AND L[N][k] //	
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
				for(int p = 0; p < length; p++){
					bootsAND(&LSA[i][j][k], &SA[i][j][k], &L[j][i], bk);
				}
			}
		}
		
		// initialize SLSA to 0 //
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < length; j++){
				bootsCONSTANT(&SLSA[i][j], 0, bk);
			}
		}
		
		// SLSA[k] = sum(LSA[k][N]) // 
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
 				HomAdd(SLSA[i], LSA[i][j], SLSA[i], length, bk);
			}
		}
		
		// y[k] = SLSA[k]/n[k]	//		
 		for(int i = 0; i < k; i++){
 			HomDiv(y[i], temp[0], SLSA[i], n[i], length, bk);
		}
		
		// x[i][j] = y[i]-x[i][j] //		
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
 				HomSubt(x[i][j], y[i], x[i][j], length, bk);
			}
		}
		
		// u[i][j] = x[i][j]/n[i] //
		for(int i = 0; i < k; i++){
 			for(int j = 0; j < N; j++){
 				HomDiv(u[i][j], temp[0], x[i][j], n[i], length, bk);
			}
		}
		
		
		// L[] = arg min_i u[i][j] // 
		for(int p = 0; p < N; p++){
			for(int i = 0; i < k; i++){
				for(int j = 0; j < k; j++){
					if (i == j){
						bootsCONSTANT(&temp[i][j], 1, bk);
					}
					else
						bootsCONSTANT(&temp[i][j], 0, bk);
				}
			}
			
			for(int i = 0; i < k-1; i++){
				HomCompS(temp[k], u[0][p], u[i+1][p], length, bk);

				HomSwap(temp[k][0], u[0][p], u[i+1][p], length, bk);
				HomSwap(temp[k][0], temp[0], temp[i+1], length, bk);
			}
			
			for(int i = 0; i < k; i++){		
				bootsCOPY(&res[p][i], &temp[0][i], bk);
			}
		}
		
		s++;	

		if (s == iter)
			break;
			
	}
	// printf("Step2-clear...........\n");

 	for(int i = 0; i < k; i++){
	 	for(int j = 0; j < N; j++){
 		 	for(int p = 0; p < N; p++){
				delete_gate_bootstrapping_ciphertext_array(length, A[i][j][p]);
			}
		}
	}
		 		
	for(int i = 0; i < k; i++){
	 	for(int j = 0; j < N; j++){
			delete_gate_bootstrapping_ciphertext_array(length, SA[i][j]);
			delete_gate_bootstrapping_ciphertext_array(length, LSA[i][j]);
			delete_gate_bootstrapping_ciphertext_array(length, x[i][j]);
			delete_gate_bootstrapping_ciphertext_array(length, u[i][j]);
			
		}
	}
	for(int j = 0; j < N; j++){
		delete_gate_bootstrapping_ciphertext_array(k, L[j]);
	}
	 		
	for(int i = 0; i < k; i++){
		delete_gate_bootstrapping_ciphertext_array(length, y[i]);
		delete_gate_bootstrapping_ciphertext_array(length, n[i]);
	}

	for(int i = 0; i < 10; i++){
		delete_gate_bootstrapping_ciphertext_array(length, temp[i]);
	}

}
	///////////////////////////////////////////////////////////////////////////////////////////	 			
		 			

