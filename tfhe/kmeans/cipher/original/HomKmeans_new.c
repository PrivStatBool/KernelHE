#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>

void HomKmeans_new(LweSample* res[10], LweSample *data[10][30], LweSample *label[10], const int d, const int length, const int N, const int k, const TFheGateBootstrappingCloudKeySet* bk){


	LweSample *A[N][d], *AU[k][N][d], *MAU[k][N][d], *AL[k][N][d], *SAL[k][d], *U[k][d], *D[N][k], *L[N], *NL[k];
	LweSample *temp;
	
    
	int label_l = ceil(log2(N)) + 2;

	for(int i = 0; i < N; i++){
		for(int j = 0; j < d; j++){
			A[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		}
	}

 	for(int i = 0; i < k; i++){
		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
 				AU[i][j][p] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
				MAU[i][j][p] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
				AL[i][j][p] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
			}
		}
	}

	for(int i = 0; i < k; i++){
		for(int j = 0; j < d; j++){
			U[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
			SAL[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		}
	}

	for(int i = 0; i < N; i++){
		L[i] = new_gate_bootstrapping_ciphertext_array(k, bk->params);
		for(int j = 0; j < k; j++){
			D[i][j] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
		}
	}


	for(int i = 0; i < k; i++){
		NL[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
	}
	
	temp = new_gate_bootstrapping_ciphertext_array(length, bk->params);


	///////////// data copy /////////////////
	////////// A[N][d][length] //////////////
	
 	for(int i = 0; i < N; i++){
 		for(int j = 0; j < d; j++){
			for(int p = 0; p < length; p++){
//error.. 				bootsCOPY(&A[i][j][p], &data[i][j][p], bk);
				bootsCOPY(&A[i][j][p], &A[i][j][p], bk);
			}
		}
	}

	/////////// U[k][d][length] /////////////
	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
 			bootsCOPY(&L[i][j], &label[i][j], bk);
		}
	}

	/////////// # of each label's counting ////////////

	for(int i = 0; i < k; i++){
 		for(int j = 0; j < length; j++){ 
			bootsCONSTANT(&NL[i][j], 0, bk);
		}
	}

	for(int i = 0; i < label_l; i++){
		bootsCONSTANT(&temp[i], 0, bk);
	}

	for(int i = 0; i < k; i++){
 		for(int j = 0; j < N; j++){
			bootsCOPY(&temp[0], &L[j][i], bk);
			HomAdd(NL[i], NL[i], temp, label_l, bk);
		}
	}

	/////////////////////////////////

	/////////// AUL = AU AND L /////////
	for(int i = 0; i < k; i++){
 		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
				for(int q = 0; q < length; q++){
 					bootsAND(&AL[i][j][p][q], &A[j][p][q], &L[j][i], bk);
				}
			}
		}
	}

	for(int i = 0; i < k; i++){
 		for(int j = 0; j < d; j++){
			for(int p = 0; p < length; p++){ 
				bootsCONSTANT(&SAL[i][j][p], 0, bk);
			}
		}
	}

	for(int i = 0; i < k; i++){
 		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
				HomAdd(SAL[i][p], SAL[i][p], AL[i][j][p], length, bk);
			}
		}
	}

	for(int i = 0; i < k; i++){
 		for(int j = 0; j < d; j++){
			HomDiv(U[i][j], temp, SAL[i][j], NL[i], length, bk);
		}
	}



	/////////////////////////////////////////////////////////////////////////////


	/////////// AU[k][N][d] = A[N][d] - U[k][d] /////////////
	for(int i = 0; i < k; i++){
 		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
				HomSubt(AU[i][j][p], A[j][p], U[i][p], length, bk);
			}
		}
	}

	/////////// (AU[k][N][d])^2 /////////////
	for(int i = 0; i < k; i++){
 		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
				HomMulti(MAU[i][j][p], AU[i][j][p], AU[i][j][p], length, bk);
			}
		}
	}

	/////////// D = sum_{d}(MAU) /////////////
	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
			for(int p = 0; p < length; p++){
				bootsCONSTANT(&D[i][j][p], 0, bk);
			}
		}
	}

	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
			for(int p = 0; p < d; p++){
				HomAdd(D[i][j], D[i][j], MAU[j][i][p], length, bk);
			}
		}
	}

	//////////// make label ///////////

	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
			bootsCONSTANT(&L[i][j], 1, bk);
		}
	}

	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k-1; j++){
			for(int p = j+1; p < k; p++){
				HomCompL(&temp[0], D[i][j], D[i][p], length, bk);
				bootsNOT(&temp[1], &temp[0], bk);
				bootsAND(&L[i][j], &L[i][j], &temp[0], bk);
				bootsAND(&L[i][p], &L[i][p], &temp[1], bk);
			}
		}
	}

	/////////////////////////////////

	
	for(int i = 0; i < N; i++){
 		for(int j = 0; j < k; j++){
			bootsCOPY(&res[i][j], &L[i][j], bk);
		}
	}

 	//clean up all pointers //
	for(int i = 0; i < N; i++){
		delete_gate_bootstrapping_ciphertext_array(k, L[i]);
		for(int j = 0; j < d; j++){
			delete_gate_bootstrapping_ciphertext_array(length, A[i][j]);
		}
	}

 	for(int i = 0; i < k; i++){
		for(int j = 0; j < N; j++){
			for(int p = 0; p < d; p++){
				delete_gate_bootstrapping_ciphertext_array(length, AU[i][j][p]);
				delete_gate_bootstrapping_ciphertext_array(length, MAU[i][j][p]);
				delete_gate_bootstrapping_ciphertext_array(length, AL[i][j][p]);
			}
		}
	}

	for(int i = 0; i < k; i++){
		for(int j = 0; j < d; j++){
			delete_gate_bootstrapping_ciphertext_array(length, SAL[i][j]);
			delete_gate_bootstrapping_ciphertext_array(length, U[i][j]);
		}
	}

	for(int i = 0; i < N; i++){
		for(int j = 0; j < k; j++){
			delete_gate_bootstrapping_ciphertext_array(length, D[i][j]);
		}
	}

	for(int i = 0; i < k; i++){
		delete_gate_bootstrapping_ciphertext_array(length, NL[i]);
	}

	delete_gate_bootstrapping_ciphertext_array(length, temp);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



 void HomKmeans_1d_abs(LweSample* res[100], LweSample* m[100], LweSample **a, LweSample **u, const int length, const int N, const int k, const TFheGateBootstrappingCloudKeySet* bk){


	int length2 = log2(N)+1;

 	LweSample *L[N], *E[N], *U[k], *G[N], *F[k], *H[k], *w[k-1], *ww[k-1], *v[k-1];
	LweSample *M[N];
	// L = data copy // 
	// M = label // 
	// E = label copy for add // 
	// G = L and M // 
 	for(int i = 0; i < N; i++)
 	{
 		L[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 		M[i] = new_gate_bootstrapping_ciphertext_array(k, bk->params);
 		E[i] = new_gate_bootstrapping_ciphertext_array(length2, bk->params);
 		G[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 	}

	// F = # of data in each cluster //
	// H = sum of data in esch cluster //
	// U = H / F //
 	for(int i = 0; i < k; i++)
 	{
 		F[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 		H[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 		U[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);

	}

	// variable for labelling //
 	for(int i = 0; i < k-1; i++)
 	{
 		w[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 		ww[i] = new_gate_bootstrapping_ciphertext_array(length, bk->params);
 		v[i] = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	}

	LweSample* t = new_gate_bootstrapping_ciphertext_array(k, bk->params);

	// data copy //
	// a[N][length] to L[N][length] //
 	for(int i = 0; i < N; i++)
	{
 		for(int j = 0; j < length; j++)
 		{
 			bootsCOPY(&L[i][j], &a[i][j], bk);
 		}
 	}

 	// initial value setting for counting and sum of each cluster's data //
 	// E[N][length2] = 0 //
 	// F[k][length] = {0, 0, ..., N} (# of data) //
 	// H[k][length] = 0 //
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < length2; j++)
			bootsCONSTANT(&E[i][j], 0, bk);
	}

	for(int i = 0; i < k; i++)
	{
		for(int j = 0; j < length; j++)
		{
			bootsCONSTANT(&F[i][j], 0, bk);
			bootsCONSTANT(&H[i][j], 0, bk);
		}
	}

	bootsCONSTANT(&F[k-1][length2-1], 1, bk);




 	int s = 0;
 	while(s < 1){
 		// calculate median of near cluster's means //
 		// w[i] = median(mean) of u[i] and u[i+1] //
		for(int i = 0; i < k-1; i++)
 		{
 			HomAdd(ww[i], u[i], u[i+1], length, bk);
 			HomLShift(w[i], ww[i], length, 1, bk);
 		}

 		printf("%d - calculating median of cluster's means clear!\n", s+1);

 		// label setting process //
 		for(int i = 0; i < N; i++)
 		{
 			// comparison with data and median //
 			// if L[i] > w[i], then v[j][1] = 1 //
 			for(int j = 0; j < k-1; j++)
 			{
 				HomCompL(&v[j][1], L[i], w[j], length, bk);
 				bootsNOT(&v[j][0], &v[j][1], bk);
 			}

 			// label setting //
 			// ex) M[i][j] = {100, 010, 001} //
 			for(int j = 0; j < k; j++)
 			{
 				bootsCONSTANT(&t[0], 1, bk);

 				for(int m = 1; m < k; m++)
 				{
 					int e;
 					if(m <= j)
 						e = 1;
 					else
 						e = 0;

 					bootsAND(&t[m], &v[m-1][e], &t[m-1], bk);
 				}

 				bootsCOPY(&M[i][j], &t[k-1], bk);
 			}
 			printf("%d - %d - label setting process clear!\n", s+1, i+1);
 		}
 		
 		for(int i = 0; i < N; i++){
 			for(int j = 0; j < k; j++){
 				bootsCOPY(&m[i][j], &M[i][j], bk);
 			}
 		}
/*
 	for(int i = 0; i < N; i++)
 	{
 		for(int j = 0; j < k; j++)
			bootsCOPY(&res[i][j], &M[i][j], bk);
 	}
*/


 		// counting # of each cluster's data : F //
 		// sum of each cluster's data : H //
 		// new_cluster mean u[i] = H[i]/F[i] //
 		// i = 0 ... k-2 //
		for(int i = 0; i < k-1; i++)
		{
			bootsCOPY(&F[i][0], &M[0][i], bk);
			for(int j = 1; j < N; j++)
			{
				bootsCOPY(&E[j][0], &M[j][i], bk);
				HomAdd(F[i], F[i], E[j], length2, bk);
			}

			for(int j = 0; j < N; j++)
			{
				for(int p = 0; p < length; p++)
				{
					bootsAND(&G[j][p], &L[j][p], &M[j][i], bk);
				}
 				
				HomAdd(H[i], H[i], G[j], length, bk);
					
			}


			HomDiv(u[i], U[i], H[i], F[i], length, bk);
		
 			printf("%d - %d cluster mean calculate complete\n", s+1,i+1);
			
			HomSubt(F[k-1], F[k-1], F[i], length2, bk);

		}
		// i = k-1 //
		for(int j = 0; j < N; j++)
		{
			for(int p = 0; p < length; p++)
			{
				bootsAND(&G[j][p], &L[j][p], &M[j][k-1], bk);
			}
			
			HomAdd(H[k-1], H[k-1], G[j], length, bk);
				
		}
 		
		
		HomDiv(u[k-1], U[k-1], H[k-1], F[k-1], length, bk);	

		printf("%d - %d cluster mean calculate complete\n", s+1,k);
		

		s++;

 		printf("%d - loop clear\n", s);


 	}

 	// result //
 	// res[i] = u[i] //
 	for(int i = 0; i < k; i++){
 		for(int j = 0; j < length; j++){
			bootsCOPY(&res[i][j], &u[i][j], bk);}}


 	//clean up all pointers //
 	for(int i = 0; i < N; i++)
 	{
 		delete_gate_bootstrapping_ciphertext_array(length, L[i]);
 		delete_gate_bootstrapping_ciphertext_array(length2, E[i]);
 		delete_gate_bootstrapping_ciphertext_array(k, M[i]);
 		delete_gate_bootstrapping_ciphertext_array(length, G[i]);
 	}

 	for(int i = 0; i < k; i++)
 	{
 		delete_gate_bootstrapping_ciphertext_array(length, F[i]);
 		delete_gate_bootstrapping_ciphertext_array(length, H[i]);
 		delete_gate_bootstrapping_ciphertext_array(length, U[i]);
 	}

 	for(int i = 0; i < k-1; i++)
 	{
 		delete_gate_bootstrapping_ciphertext_array(length, w[i]);
 		delete_gate_bootstrapping_ciphertext_array(length, ww[i]);
 		delete_gate_bootstrapping_ciphertext_array(2, v[i]);
 	}

 	delete_gate_bootstrapping_ciphertext_array(k, t);

 }

