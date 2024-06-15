#include <stdio.h>
#include <time.h>
#include <math.h>

 void PlainKmeans(int res[10][50], int (*a)[50], int (*u)[50], const int N, const int d, const int k, const int iter){

    printf("//////////////%d, %d, %d//////////////////\n/", (int)sizeof(a), (int)sizeof(a[1]), (int)sizeof(a[1][1]));


	int A[N][d];
	int U[k][d];
	
	// data copy //
	for(int i = 0; i < N; i++){
		for(int j = 0; j < d; j++){
			
			A[i][j] = a[i][j];
			//printf("%d, %d, %d\n", i, j, A[0][0]);
		}
	}		

	for(int i = 0; i < k; i++){
		for(int j = 0; j < d; j++){
			U[i][j] = u[i][j];
		}
	}



//    for(int i = 0; i < N; i++){
//        printf("%d, %d, %d \n", A[i][0], A[i][1], A[i][2]);
//        }
	int s = 0;
 	
 	while(1){
 		// calculate median of near cluster's means //
 		// w[i] = median(mean) of u[i] and u[i+1] //
		int AU[k][N][d];
	
		for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				for(int p = 0; p < d; p++){
					AU[i][j][p] = A[j][p]-U[i][p];
				}
			}
		}

		int MAU[k][N][d];
	
		for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				for(int p = 0; p < d; p++){
					MAU[i][j][p] = (int) pow((double)AU[i][j][p], 2);
				}
			}
		}


		int D[N][k];
		
		for(int i = 0; i < N; i++){
			for(int j = 0; j < k; j++){
				D[i][j] = 0;
				for(int p = 0; p < d; p++){
					D[i][j] = D[i][j] + MAU[j][i][p];}
			}
		}

		int label[N];

		for(int i = 0; i < N; i++){
			label[i] = 0;
			int min = D[i][0];
			
			for(int j = 1; j < k; j++){
				if (D[i][j] < min){
					min = D[i][j];
					label[i] = j;
				}
			}
		}

		int NL[k];
		int SAL[k][d];

		for(int i = 0; i < k; i++){
			NL[i] = 0;
			for(int j = 0; j < d; j++){
				SAL[i][j] = 0;
			}
		}

		for(int i = 0; i < N; i++){
			for(int j = 0; j < k; j++){
				if(label[i] == j+1){
					NL[j]++;
					for(int p = 0; p < d; p++){
						SAL[j][p] = SAL[j][p] + A[j][p];
					}
				}
			}
		}

		for(int i = 0; i < k; i++){
			for(int j = 0; j < d; j++){
				if (NL[i] == 0){
					res[i][j] = SAL[i][j];
				}
				else{
					res[i][j] = SAL[i][j] / NL[i];
				}
					
			}

		}
		s++;

		if (s >= iter){
			break;
		}


 		//printf("%d - loop clear\n", s);


 		

 	
 	return res[k][d];

 	}	

}