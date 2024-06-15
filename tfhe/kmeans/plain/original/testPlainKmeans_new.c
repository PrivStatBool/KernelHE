#include <stdio.h>
#include <math.h>
#include <PlainKmeans.c>


int main(int argc, char const *argv[])
{
	const int N = 64;
	const int k = 3;
//    int d = 3;
    const int iter = 1;
    const int test_d = 50;
    double time[test_d+1]; 

    for(int d = 1; d < test_d + 1; d++){

        int plaintext1[N][d];
        int plaintext2[k][d];


        FILE* data = fopen("Kmeansdata.txt","r");
        for(int i = 0; i < N; i++){
            for(int j = 0; j < d; j++){
                fscanf(data, "%d", &plaintext1[i][j]);
            }
        }
        fclose(data);

        FILE* cluster = fopen("Kmeanscluster.txt","r");
        for(int i = 0; i < k; i++){
           for(int j = 0; j < d; j++){
               fscanf(cluster, "%d", &plaintext2[i][j]);
            }
        }
        fclose(cluster);


        for(int i = 0; i < N; i++){
            for(int j = 0; j < d; j++){
                printf("%d, %d, %d, ", i, j, plaintext1[i][j]);
            }
            printf("\n");
        }


        int result[k][d];


    /////////////////////////////////////////////////////////////
    
        float time_temp = -clock();
        printf("%d\n", d);

	int A[N][d];
	int U[k][d];
	
	// data copy //
	for(int i = 0; i < N; i++){
		for(int j = 0; j < d; j++){
			
			A[i][j] = plaintext1[i][j];
			//printf("%d, %d, %d\n", i, j, A[0][0]);
		}
	}		

	for(int i = 0; i < k; i++){
		for(int j = 0; j < d; j++){
			U[i][j] = plaintext2[i][j];
		}
	}

        for(int i = 0; i < N; i++){
            for(int j = 0; j < d; j++){
                printf("%d, %d, %d, ", i, j, A[i][j]);
            }
            printf("\n");
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
				if(label[i] == j){
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
					result[i][j] = SAL[i][j];
				}
				else{
					result[i][j] = SAL[i][j] / NL[i];
				}
					
			}

		}
		s++;

		if (s >= iter){
			break;
		}


 		//printf("%d - loop clear\n", s);
	
 	}	

        printf("%d\n", d);
        time_temp += clock();
        time_temp = time_temp/(CLOCKS_PER_SEC);

        printf("done in %f seconds...\n", time_temp);


        time[d] = time_temp;
   
	/////////////////////////////////////////////////////////////
/*
	    for(int i = 0; i < k; i++){
            for(int j = 0; j < d; j++){
                printf("%d, ", result[i][j]);
            }
            printf("\n");
	    }
*/

        FILE* KmeansPlainResult = fopen("KmeansPlainResult.txt","w");
        for(int i = 0; i < k; i++){
            for(int j = 0; j < d; j++){
                fprintf(KmeansPlainResult, "%d ", result[i][j]);}
            fprintf(KmeansPlainResult, "\n");
        }
        fclose(KmeansPlainResult);

        printf("%d\n", d);
    }
    
    FILE* KmeansPlainTime = fopen("KmeansPlainTime.txt","w");
        for(int d = 1; d < test_d + 1; d++){
            fprintf(KmeansPlainTime, "%f\n", time[d]);
        }
    fclose(KmeansPlainTime);

	return 0;
}