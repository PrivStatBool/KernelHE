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
    double time[test_d + 1]; 

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

	    PlainKmeans(result, plaintext1, plaintext2, N, d, k, iter);

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
            fprintf(KmeansPlainTime, "%f \n", time[d]);
        }
    fclose(KmeansPlainTime);

	return 0;
}