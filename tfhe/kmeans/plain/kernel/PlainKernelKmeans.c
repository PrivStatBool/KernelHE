#include <stdio.h>
#include <time.h>
#include <math.h>

 void PlainKernelKmeans(int res[64], int (*kernel_matrix)[64], int label[64], const int N, const int d, const int k, const int iter){

/*
	// make Kernel matrix //
    int MK[N];
	for(int i = 0; i < d; i++){
		MK[i] = (int)pow((double)MK[i],2);
	}
	for(int i = 1; i < d; i++){
		MK[0] = MK[0] + MK[i];
	}
*/		
	int K[N][N];
	int L[N];
	
	// data copy //
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			K[i][j] = kernel_matrix[i][j];
			//printf("%d, %d, %d\n", i, j, A[0][0]);
		}
	}		
	
	for(int i = 0; i < N; i++){
		L[i] = label[i];
	}



//    for(int i = 0; i < N; i++){
//        printf("%d, %d, %d \n", A[i][0], A[i][1], A[i][2]);
//        }
	int s = 0;
 	
 	while(1){
 		// calculate median of near cluster's means //
 		// w[i] = median(mean) of u[i] and u[i+1] //
		int NL[k];
		int SKL[k][N];

		for(int i = 0; i < k; i++){
			NL[i] = 0;
			for(int j = 0; j < N; j++){
				SKL[i][j] = 0;
			}
		}

		for(int i = 0; i < N; i++){
			for(int j = 1; j < k + 1; j++){
				if(L[i]==j){
					NL[j]++;
					for(int p = 0; p < N; p++){
						SKL[j-1][p] = SKL[j-1][p] + K[i][p];
					}
				}
			}
		}

		int SLSKL[k];

		for(int i = 0; i < k; i++){
			SLSKL[i] = 0;
		}

		for(int i = 0; i < N; i++){
			for(int j = 1; j < k + 1; j++){
				if(L[i] == j){
					SLSKL[j-1] = SLSKL[j-1] + SKL[j-1][i];
				}
			}
		}

		int Y[k];

		for(int i = 0; i < k; i++){
			if(NL[i] == 0){
				Y[i] = SLSKL[i];	
			}
			else{
				Y[i] = SLSKL[i]/NL[i];
			}
			
		}		

		int U[k][N];

		for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				if(NL[i] == 0){
					U[i][j] = (Y[i] - 2*SKL[i][j]);
				}
				else{
					U[i][j] = (Y[i] - 2*SKL[i][j])/NL[i];
				}
			}
		}

		int min;

		for(int i = 0; i < N; i++){
			min = U[0][i];
			L[i] = 1;
			for(int j = 1; j < k; j++){
				if (U[j][i] < min){
					min = U[j][i];
					L[i] = j + 1;
				}
			}
		}

		s++;

		if (s >= iter){
			break;
		}

 		//printf("%d - loop clear\n", s);

	
 	}	

	for(int i = 0; i < N; i++){
		res[i] = L[i];
	}




	return res[N];

}