#include <stdio.h>
#include <math.h>
#include <time.h>
// #include <PlainKNN.c>


int main(int argc, char const *argv[])
{
	for(int test_num = 0; test_num < 10; test_num++){
	const int N = 10;
	const int k = 3;
//    int d = 3;
	const int test_N = 1;
    const int iter = 1;
    const int test_d = 50;
    double time[test_d]; 

    for(int d = 1; d < test_d + 1; d++){
	
        int plaintext1[N][d];
        int plaintext2[N];
		int plaintext3[test_N][d];


        FILE* data = fopen("KNNdata.txt","r");
        for(int i = 0; i < N; i++){
            for(int j = 0; j < d; j++){
                fscanf(data, "%d", &plaintext1[i][j]);
            }
        }
        fclose(data);

        FILE* label = fopen("KNNlabel.txt","r");
        for(int i = 0; i < N; i++){
            fscanf(label, "%d", &plaintext2[i]);
        }
        fclose(label);

		FILE* new_data = fopen("KNNnewdata.txt","r");
        for(int i = 0; i < test_N; i++){
            for(int j = 0; j < d; j++){
                fscanf(new_data, "%d", &plaintext3[i][j]);
            }
        }
        fclose(new_data);


        int result[test_N];


    /////////////////////////////////////////////////////////////
    
        double time_temp = -clock();
        printf("%d\n", d);
	for(int num = 0; num < 10000; num++){		
		int A[N][d];
		int T[test_N][d];
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

		for(int i = 0; i < test_N; i++){
			for(int j = 0; j < d; j++){
				T[i][j] = plaintext3[i][j];
				//printf("%d, %d, %d\n", i, j, A[0][0]);
			}
		}
        

	
 			// calculate median of near cluster's means //
 			// w[i] = median(mean) of u[i] and u[i+1] //

		for(int s = 0; s < test_N; s++){
			int AT[N][d];
			int SAT[N];

			for(int i = 0; i < N; i++){
				AT[i][0] = A[i][0] - T[s][0];
				SAT[i] = AT[i][0]*AT[i][0];
				for(int j = 1; j < d; j++){
					AT[i][j] = A[i][j] - T[s][j];
					SAT[i] = SAT[i] + AT[i][j]*AT[i][j];
				}
			}



			int temp;

			for(int i = 0; i < k; i++){
				for(int j = i + 1; j < N; j++){
					if (SAT[j] < SAT[i]){
						temp = SAT[i];
						SAT[i] = SAT[j];
						SAT[j] = temp;
						temp = L[i];
						L[i] = L[j];
						L[j] = temp;
					}
				}
			}	

			int NL[k];
			for(int i = 0; i < k; i++){
				NL[i] = 0;
				for(int j = i + 1; j < k; j++){
					if (L[i] == L[j]){
						NL[i]++;
					}
				}
			}

			int max; 
			max = NL[0];
			for(int i = 1; i < k; i++){
				if (max < NL[i]){
					max = NL[i];
				}
			}

			result[s] = max;
		}

	
	}
        printf("%d\n", d);
        time_temp += clock();
        time_temp = time_temp/(CLOCKS_PER_SEC);

        printf("done in %lf seconds...\n", time_temp);


        time[d-1] = time_temp;
   
	/////////////////////////////////////////////////////////////
/*
	    for(int i = 0; i < k; i++){
            for(int j = 0; j < d; j++){
                printf("%d, ", result[i][j]);
            }
            printf("\n");
	    }
*/

        FILE* KNNPlainResult = fopen("KNNPlainResult.txt","w");
        for(int i = 0; i < N; i++){
            fprintf(KNNPlainResult, "%d \n", result[i]);
		}
        fclose(KNNPlainResult);
    }

	
	double time_0[test_d];

	FILE* time_pre = fopen("KNNPlainTime.txt","r");
        for(int i = 0; i < test_d; i++){
            fscanf(time_pre, "%lf", &time_0[i]);
			printf("%lf\n", time_0[i]);
        }
    fclose(time_pre);

    FILE* KNNPlainTime = fopen("KNNPlainTime.txt","w");
        for(int i = 0; i < test_d; i++){
            fprintf(KNNPlainTime, "%lf\n", ((test_num)*time_0[i] + time[i])/(test_num + 1));
        }
    fclose(KNNPlainTime);
	printf("test_num = %d clear...............\n", test_num);
	}

	return 0;

}