#include <stdio.h>
#include <math.h>
#include <PlainKernelKmeans.c>


int main(int argc, char const *argv[])
{
	for(int test_num = 0; test_num < 1; test_num++){

//	const int N = 64;
	const int k = 3;
    const int d = 10;
    const int iter = 1;
//    const int test_d = 50;
    const int test_N = 100;
//    double time[test_d+1];
    double time[test_N+1];

    for(int N = 1; N < test_N + 1; N++){

        int plaintext1[N][N];
        int plaintext2[N];


        FILE* data = fopen("KernelKmeansdata.txt","r");
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                fscanf(data, "%d", &plaintext1[i][j]);
            }
        }
        fclose(data);

        FILE* cluster = fopen("KernelKmeanscluster.txt","r");
        for(int i = 0; i < N; i++){
            fscanf(cluster, "%d", &plaintext2[i]);
        }
        fclose(cluster);


        int result[N];


    /////////////////////////////////////////////////////////////
    
        float time_temp = -clock();
        printf("%d\n", d);
    for(int num = 0; num < 10000; num++){    
	    PlainKernelKmeans(result, plaintext1, plaintext2, N, d, k, iter);
    }
        time_temp += clock();
        time_temp = time_temp/(CLOCKS_PER_SEC);

        printf("done in %f seconds...\n", time_temp);


        time[N] = time_temp;
   
	/////////////////////////////////////////////////////////////
/*
	    for(int i = 0; i < k; i++){
            for(int j = 0; j < d; j++){
                printf("%d, ", result[i][j]);
            }
            printf("\n");
	    }
*/

        FILE* KernelKmeansPlainResult = fopen("KernelKmeansPlainResult.txt","w");
        for(int i = 0; i < N; i++){
            fprintf(KernelKmeansPlainResult, "%d \n", result[i]);
        }
        fclose(KernelKmeansPlainResult);

        printf("%d\n", N);
    }
/*
    FILE* KernelKmeansPlainTime = fopen("KernelKmeansPlainTime.txt","w");
        for(int N = 1; N < test_N+1; N++){
            fprintf(KernelKmeansPlainTime, "%f \n", time[N]);
        }
    fclose(KernelKmeansPlainTime);
*/

    	double time_0[test_N];

	FILE* time_pre = fopen("KernelKmeansPlainTime.txt","r");
        for(int i = 0; i < test_N; i++){
            fscanf(time_pre, "%lf", &time_0[i]);
			printf("%lf\n", time_0[i]);
        }
    fclose(time_pre);
    
    FILE* KernelKmeansPlainTime_new = fopen("KernelKmeansPlainTime.txt","w");
        for(int i = 0; i < test_N; i++){
            fprintf(KernelKmeansPlainTime_new, "%lf\n", ((test_num)*time_0[i] + time[i])/(test_num + 1));
        }
    fclose(KernelKmeansPlainTime_new);
	printf("test_num = %d clear...............\n", test_num);
	}

	return 0;
}