#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <time.h>



///////////////////////////////     Addition     ////////////////////////////
//res = a + b

void HomAdd(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* c = new_gate_bootstrapping_ciphertext_array(length, bk->params);	    
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    

	bootsCONSTANT(&c[0], 0, bk);
    
	for(int i = 0; i < length -1; i++){
		bootsXOR(&temp[0], &a[i], &b[i], bk);
		bootsAND(&temp[1], &a[i], &b[i], bk);
		bootsXOR(&res[i], &temp[0], &c[i], bk);
		bootsAND(&temp[0], &temp[0], &c[i], bk);
		bootsOR(&c[i+1], &temp[0], &temp[1], bk);
	}

	bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
	bootsXOR(&res[length-1], &temp[0], &c[length-1], bk);

	delete_gate_bootstrapping_ciphertext_array(length, c);    
	delete_gate_bootstrapping_ciphertext_array(2, temp);    
}





///////////////////////////////     2's liment     ////////////////////////////
//res = -a

void HomTwosCompliment(LweSample* res, const LweSample* a, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    
	LweSample* b = new_gate_bootstrapping_ciphertext_array(length, bk->params);	    

	bootsCONSTANT(&b[0], 1, bk);
    

	for(int i = 0; i < length - 2; i++){
		bootsNOT(&temp[0], &a[i], bk);
		bootsXOR(&res[i], &temp[0], &b[i], bk);
		bootsAND(&b[i+1], &temp[0], &b[i], bk);}

	bootsNOT(&temp[0], &a[length-2], bk);
	bootsXOR(&res[length-2], &temp[0], &b[length-2], bk);

	bootsCOPY(&temp[0], &a[0], bk);
	for(int i = 1; i < length; i++){
		bootsOR(&temp[0], &temp[0], &a[i], bk);
	}

	bootsNOT(&temp[1], &a[length-1], bk);

	bootsMUX(&res[length-1], &temp[0], &temp[1], &a[length-1],bk);

	delete_gate_bootstrapping_ciphertext_array(length, b);    
	delete_gate_bootstrapping_ciphertext_array(2, temp);    
}




///////////////////////////////     subtract     ////////////////////////////
//res = a - b

void HomSubt(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* c = new_gate_bootstrapping_ciphertext_array(length, bk->params);	    
	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    

	bootsCONSTANT(&c[0], 0, bk);
    
	for(int i = 0; i < length -1; i++){
		bootsXOR(&temp[0], &a[i], &b[i], bk);
		bootsANDNY(&temp[1], &a[i], &b[i], bk);
		bootsXOR(&res[i], &temp[0], &c[i], bk);
		bootsANDNY(&temp[0], &temp[0], &c[i], bk);
		bootsOR(&c[i+1], &temp[1], &temp[0], bk);} 

	bootsXOR(&temp[0], &a[length-1], &b[length-1], bk);
	bootsXOR(&res[length-1], &temp[0], &c[length-1], bk);

	delete_gate_bootstrapping_ciphertext_array(length, c);    
	delete_gate_bootstrapping_ciphertext_array(2, temp);    
}



///////////////////////////////     left shift     ////////////////////////////

void HomLShift(LweSample* res, const LweSample* a, const int length, const int k, const TFheGateBootstrappingCloudKeySet* bk) {

	for(int i = 0; i < length - k; i++){
		bootsCOPY(&res[i], &a[i+k], bk);}
	for(int i = length-k; i < length; i++){
		bootsCOPY(&res[i], &a[length-1], bk);}
}



///////////////////////////////     right shift     ////////////////////////////

void HomRShift(LweSample* res, const LweSample* a, const int length, const int k, const TFheGateBootstrappingCloudKeySet* bk) {

	bootsCOPY(&res[length-1], &a[length-1], bk);
	for(int i = length-2; i > k-1; i--){
		bootsCOPY(&res[i], &a[i-k], bk);}
	for(int i = 0; i < k; i++){
		bootsCONSTANT(&res[i], 0, bk);}


}




///////////////////////////////     equivalent or not     ////////////////////////////
// if a = b then res = E(1)
// else res = E(0)

void HomEqui(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);	    

	bootsCONSTANT(&temp[0], 1, bk);
	for(int i = 0; i < length; i++){		
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsAND(&temp[0], &temp[0], &temp[1], bk);
		
	}
	bootsCOPY(&res[0], &temp[0], bk);

	delete_gate_bootstrapping_ciphertext_array(2, temp);
}



///////////////////////////////     big comparison     ////////////////////////////
// if a > b then res = E(1)
// else res = E(0)

void HomCompL(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length-1; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &a[i], bk);}

	bootsCOPY(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}




///////////////////////////////     small comparison     ////////////////////////////
// if a < b then res = E(1)
// else res = E(0)

void HomCompS(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length-1; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &b[i], bk);}

	bootsCOPY(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}

///////////////////////////////     smaller than or equal to comparison     ////////////////////////////

void HomCompSE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length-1; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &a[i], bk);}

	bootsNOT(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}

///////////////////////////////     larger than or equal to comparison     ////////////////////////////

void HomCompLE(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(2, bk->params);
	
	bootsCONSTANT(&temp[0], 0, bk);
	
	for(int i = 0; i < length-1; i++){
		bootsXNOR(&temp[1], &a[i], &b[i], bk);
		bootsMUX(&temp[0], &temp[1], &temp[0], &b[i], bk);}

	bootsNOT(&res[0], &temp[0], bk);
	delete_gate_bootstrapping_ciphertext_array(2, temp);
}



///////////////////////////////     maximum     ////////////////////////////
// if a > b then res = a

void HomMax(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	
	HomCompL(&temp[0], a, b, length, bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &temp[0], &a[i], &b[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(1, temp);
}



///////////////////////////////     minimum     ////////////////////////////
// if a < b then res = a

void HomMin(LweSample* res, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	
	HomCompS(&temp[0], a, b, length, bk);

	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &temp[0], &a[i], &b[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(1, temp);
}




///////////////////////////////     absolute value     ////////////////////////////
// if a < b then res = a

void HomAbs(LweSample* res, const LweSample* a, const int length, const TFheGateBootstrappingCloudKeySet* bk) {

	LweSample* na = new_gate_bootstrapping_ciphertext_array(length, bk->params);

	HomTwosCompliment(na, a, length, bk);
	
	for(int i = 0; i < length; i++){
		bootsMUX(&res[i], &a[length-1], &na[i], &a[i], bk);}

	delete_gate_bootstrapping_ciphertext_array(length, na);
}

///////////////////////////////     swap      ////////////////////////////
//


void HomSwap(const LweSample c, const LweSample* a, const LweSample* b, const int length, const TFheGateBootstrappingCloudKeySet* bk) {
	
	LweSample* temp =  new_gate_bootstrapping_ciphertext_array(1, bk->params);
	LweSample* D1 =  new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* D2 =  new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* E1 =  new_gate_bootstrapping_ciphertext_array(length, bk->params);
	LweSample* E2 =  new_gate_bootstrapping_ciphertext_array(length, bk->params);

	bootsNOT(&temp[0], &c, bk);


	for(int i = 0; i < length; i++){
		bootsAND(&D1[i], &a[i], &c, bk);
		bootsAND(&D2[i], &a[i], &temp[0], bk);
		bootsAND(&E1[i], &b[i], &c, bk);
		bootsAND(&E2[i], &b[i], &temp[0], bk);}

	HomAdd(a, D2, E1, length, bk);
	HomAdd(b, D1, E2, length, bk);


	delete_gate_bootstrapping_ciphertext_array(1, temp);
	delete_gate_bootstrapping_ciphertext_array(length, D1);
	delete_gate_bootstrapping_ciphertext_array(length, D2);
	delete_gate_bootstrapping_ciphertext_array(length, E1);
	delete_gate_bootstrapping_ciphertext_array(length, E2);
}


///////////////////////////////     arg min      ////////////////////////////
//


void HomArgmin(const LweSample* res[10], const LweSample* a[100], const LweSample* l[100], const int length, const int N, const int k, const TFheGateBootstrappingCloudKeySet* bk) {
	
	LweSample *A[N], *L[N];
	LweSample *temp = new_gate_bootstrapping_ciphertext_array(1, bk->params);
	
	for (int i = 0; i < N; i++){
		A[i] =  new_gate_bootstrapping_ciphertext_array(length, bk->params);
		L[i] =  new_gate_bootstrapping_ciphertext_array(k, bk->params);
	}
	
	for (int i = 0; i < N; i++){
		for (int j = 0; j < length; j++){
			bootsCOPY(&A[i][j], &a[i][j], bk);
		}
		for (int j = 0; j < k; j++){
			bootsCOPY(&L[i][j], &l[i][j], bk);
		}
	}
	
	for (int i = 0; i < N-1; i++){
		HomCompS(&temp[0], A[0], A[i+1], length, bk);

		HomSwap(temp[0], A[0], A[i+1], length, bk);
		HomSwap(temp[0], L[0], L[i+1], k, bk);
	}

	for (int i = 0; i < k; i++){
		bootsCOPY(&res[i][0], &L[0][i], bk);
	}

	for (int i = 0; i < N; i++){
		delete_gate_bootstrapping_ciphertext_array(length, A[i]);
		delete_gate_bootstrapping_ciphertext_array(k, L[i]);
	}

	delete_gate_bootstrapping_ciphertext_array(1, temp);
}

/*
 void HomMode(LweSample* res[100], LweSample* a[200], LweSample* l[100], LweSample* b[5], const int length, const int label_length, const int N, const int d, const int k, const TFheGateBootstrappingCloudKeySet* bk){



 	LweSample *A[N*d], *B[N*d], *C[N], *L[N];



	//clean up all pointers //
 	for(int i = 0; i < N*d; i++)
 	{
 		delete_gate_bootstrapping_ciphertext_array(length, A[i]);
 		delete_gate_bootstrapping_ciphertext_array(length, B[i]);
	}
	
 	for(int i = 0; i < N; i++)
	{
 		delete_gate_bootstrapping_ciphertext_array(length, C[i]);
 		delete_gate_bootstrapping_ciphertext_array(label_length, L[i]);
	}

 	delete_gate_bootstrapping_ciphertext_array(label_length + k, D);
 	delete_gate_bootstrapping_ciphertext_array(label_length + k, E);
 	delete_gate_bootstrapping_ciphertext_array(2, temp);

}
*/
