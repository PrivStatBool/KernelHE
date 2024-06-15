#include <stdio.h>
#include <math.h>
#include <time.h>
// #include <PlainKNN.c>


int main(int argc, char const *argv[])
{
	for(int test_num = 0; test_num < 1000; test_num++){
	const int N = 10;
	const int k = 3;
//    int d = 3;
	const int test_N = 1;
    const int iter = 1;
    double time; 

    
	
        int plaintext1[N+test_N][N+test_N];
        int plaintext2[N];
		

        FILE* data = fopen("KernelKNNdata.txt","r");
        for(int i = 0; i < N+test_N; i++){
            for(int j = 0; j < N+test_N; j++){
                fscanf(data, "%d", &plaintext1[i][j]);
            }
        }
        fclose(data);

        FILE* label = fopen("KernelKNNlabel.txt","r");
        for(int i = 0; i < N; i++){
            fscanf(label, "%d", &plaintext2[i]);
        }
        fclose(label);

		

        int result[test_N];


    /////////////////////////////////////////////////////////////
    	printf("%d\n", test_num);

        double time_temp = -clock();
        
	for(int num = 0; num < 10000; num++){		
		int K[N+test_N][N+test_N];
		int L[N];
	
		// data copy //
		for(int i = 0; i < N+test_N; i++){
			for(int j = 0; j < N+test_N; j++){
				K[i][j] = plaintext1[i][j];
			}
		}



		for(int i = 0; i < N; i++){
			L[i] = plaintext2[i];
		}

		int X,Y;
		for(int i = 0; i < 1; i++){
			for(int j = 0; j < 1; j++){
				X = X*Y;
			}
		}	


 			// calculate median of near cluster's means //
 			// w[i] = median(mean) of u[i] and u[i+1] //

		
		int D[test_N][N];

		for(int i = 0; i < test_N; i++){
			for(int j = 0; j < N; j++){
				D[i][j] = K[j][j] + 2*K[j][N+i]+ K[N+i][N+i];
			}
		}



		int temp;
		for(int s = 0; s < test_N; s++){
			for(int i = 0; i < k; i++){
				for(int j = i + 1; j < N; j++){
					if (D[s][j] < D[s][i]){
						temp = D[s][i];
						D[s][i] = D[s][j];
						D[s][j] = temp;
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

        time_temp += clock();
        time_temp = time_temp/(CLOCKS_PER_SEC);

        printf("done in %lf seconds...\n", time_temp);


           
	/////////////////////////////////////////////////////////////
/*
	    for(int i = 0; i < k; i++){
            for(int j = 0; j < d; j++){
                printf("%d, ", result[i][j]);
            }
            printf("\n");
	    }
*/
	
        FILE* KernelKNNPlainResult = fopen("KernelKNNPlainResult.txt","w");
        for(int i = 0; i < test_N; i++){
            fprintf(KernelKNNPlainResult, "%d \n", result[i]);
		}
        fclose(KernelKNNPlainResult);
    

	
	double time_0;

	FILE* time_pre = fopen("KernelKNNPlainTime.txt","r");
        fscanf(time_pre, "%lf", &time_0);
    fclose(time_pre);

    FILE* KernelKNNPlainTime = fopen("KernelKNNPlainTime.txt","w");
        fprintf(KernelKNNPlainTime, "%lf\n", ((test_num)*time_0 + time_temp)/(test_num + 1));
    fclose(KernelKNNPlainTime);
	printf("test_num = %d clear...............\n", test_num);
	}

	return 0;

}
