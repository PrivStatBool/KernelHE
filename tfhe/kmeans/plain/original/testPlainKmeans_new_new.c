#include <stdio.h>
#include <math.h>
#include <PlainKmeans.c>


int main(int argc, char const *argv[])
{
	for(int test_num = 0; test_num < 1; test_num++){
//	const int N = 10;
	const int k = 3;
    const int d = 10;
    const int iter = 1;
//    const int test_d = 67;
	const int test_N = 100;
//    double time[test_d]; 
    double time[test_N]; 

    for(int N = 1; N < test_N + 1; N++){
	
        int plaintext1[N][d];
        int plaintext2[N];


        FILE* data = fopen("Kmeansdata.txt","r");
        for(int i = 0; i < N; i++){
            for(int j = 0; j < d; j++){
                fscanf(data, "%d", &plaintext1[i][j]);
            }
        }
        fclose(data);

        FILE* cluster = fopen("Kmeanscluster.txt","r");
        for(int i = 0; i < N; i++){
            fscanf(cluster, "%d", &plaintext2[i]);
        }
        fclose(cluster);


        int result[N];


    /////////////////////////////////////////////////////////////
    
        double time_temp = -clock();
        printf("%d\n", d);
	for(int num = 0; num < 10000; num++){		
		int A[N][d];
		int L[N];
	
		// data copy //
		for(int i = 0; i < N; i++){
			for(int j = 0; j < d; j++){
				A[i][j] = plaintext1[i][j];
				//printf("%d, %d, %d\n", i, j, A[0][0]);
			}
		}		

		for(int i = 0; i < N; i++){
			L[i] = plaintext2[i];
		}

        

//    for(int i = 0; i < N; i++){
//        printf("%d, %d, %d \n", A[i][0], A[i][1], A[i][2]);
//        }
		int s = 0;
 	
 		while(1){
 			// calculate median of near cluster's means //
 			// w[i] = median(mean) of u[i] and u[i+1] //

		int NL[k];
		int SAL[k][N];

		for(int i = 0; i < k; i++){
			NL[i] = 0;
			for(int j = 0; j < N; j++){
				SAL[i][j] = 0;
			}
		}

		for(int i = 0; i < N; i++){
			for(int j = 0; j < k; j++){
				if(L[i]==j+1){
					NL[j]++;
					for(int p = 0; p < d; p++){
						SAL[j][p] = SAL[j][p] + A[i][p];
					}
				}
			}
		}

		int U[k][d];

		for(int i = 0; i < k; i++){
			if (NL[i] == 0){
				for(int j = 0; j < d; j++){
					U[i][j] = SAL[i][j];
				}
			}
			else{
				for(int j = 0; j < d; j++){
					U[i][j] = SAL[i][j] / NL[i];
				}
			}
		}

		int MAU[k][N][d];

		for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				for(int p = 0; p < d; p++){
					MAU[i][j][p] = pow(A[j][p] - U[i][p], 2);
				}
			}
		}

		int SMAU[k][N];


		for(int i = 0; i < k; i++){
			for(int j = 0; j < N; j++){
				SMAU[i][j] = MAU[i][j][0];
				for(int p = 1; p < N; p++){
					SMAU[i][j] = SMAU[i][j] + MAU[i][j][p];
				}
			}
		}

		int min; 
		for(int i = 0; i < N; i++){
			min = SMAU[0][i];
			L[i] = 1;
			for(int j = 1; j < k; j++){
				if (SMAU[j][i] < min){
					min = SMAU[j][i];
					L[i] = j + 1;
				}
			}
		}

					
		s++;

		if (s >= iter){
			break;
		}

	
		}

		for(int i = 0; i < N; i++){
			result[i] = L[i];
 		}	
		
	}
        printf("%d\n", N);
        time_temp += clock();
        time_temp = time_temp/(CLOCKS_PER_SEC);

        printf("done in %lf seconds...\n", time_temp);


        time[N-1] = time_temp;
   
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
        for(int i = 0; i < N; i++){
            fprintf(KmeansPlainResult, "%d \n", result[i]);
		}
        fclose(KmeansPlainResult);

    }

	double time_0[test_N];

	FILE* time_pre = fopen("KmeansPlainTime.txt","r");
        for(int i = 0; i < test_N; i++){
            fscanf(time_pre, "%lf", &time_0[i]);
			printf("%lf\n", time_0[i]);
        }
    fclose(time_pre);
    
    FILE* KmeansPlainTime = fopen("KmeansPlainTime.txt","w");
        for(int i = 0; i < test_N; i++){
            fprintf(KmeansPlainTime, "%lf\n", ((test_num)*time_0[i] + time[i])/(test_num + 1));
        }
    fclose(KmeansPlainTime);
	printf("test_num = %d clear...............\n", test_num);
	}
	
	return 0;

}