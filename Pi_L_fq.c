#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <malloc.h>
#include <gmp.h>
#include <string.h>
#include <flint/fq.h>
#include <flint/fq_mat.h>
#include <flint/fq_vec.h>
#include <flint/fq_poly.h>
#include <flint/fmpz.h>
#include <flint/fmpz_mod_mpoly.h>

int n,t,d,m;
ulong N;

fq_ctx_t Fp;

flint_rand_t state;

bool DEBUG;

// ******* Initialize public parameters ********
void init(int nn, int dd){

	// DEBUG = true;
	DEBUG = false;
	n = nn;           // length of data
	t = 1;            // privacy threshold
	d = dd;            // degree of polynomial f
	m = (d+1)*t+1;    // number of servers
    flint_randinit(state);

	fmpz_t p;
	fmpz_init(p);
    fmpz_set_str(p,"340282366920938463463374607431768211297",10);  // 128 bit prime 
    // fmpz_set_str(p,"7",10);
    fq_ctx_init(Fp,p,1,"gen");
	fmpz_clear(p);

}



// ************ Share algorithm ********************
void L_Share(fq_mat_t x, fq_mat_t * s){
	// x[1..n];
	// s[1..m][1..n];

	fq_t temp;
	fq_init(temp,Fp);

	/*
	  Generating phi(u)

	  phi(u) is a degree-t polynomial with phi(0) = x
	  Here, we split phi(u)= [phi_0(u),...,phi_{n-1}(u)]
	  phi_i(0) = x_i
	*/
	fq_poly_t phi[n];
	for (int i = 0; i < n; i ++){
		fq_poly_init(phi[i],Fp);
		fq_poly_randtest(phi[i],state,t + 1,Fp);
		fq_poly_set_coeff(phi[i],0,fq_mat_entry(x,i,0),Fp);	
	}

	/*
	  Computing s = [s_1,...s_m]	 
	  s_j = phi(j) 
	*/
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            fq_set_ui(temp,j+1,Fp);
            fq_poly_evaluate_fq(fq_mat_entry(s[j],i,0),phi[i],temp,Fp);
        }
    }  


	if (DEBUG) {
		for (int i = 0; i < n; i ++){
			printf("phi_%d=",i);
			fq_poly_print_pretty(phi[i],"X",Fp);
			printf("\n");
		}
	}


	for (int i = 0; i < n; i ++){
		fq_poly_clear(phi[i], Fp);
	}
	fq_clear(temp,Fp);
}

// ************ Eval algorithm *********************

// eval an n-variate degree-d function f(x), output y=f(x)
void Eval(fq_mat_t f, fq_mat_t x, fq_t y){

	int index=0; //index of f

    // constant term
	if(d>=0){
		fq_set(y,fq_mat_entry(f,0,0),Fp);
	}

	fq_t y1;
   	fq_init(y1,Fp);

	// degree-1 terms
	if(d>=1){
		for(int i1=0;i1<n;i1++){
			index=index+1;
			fq_set(y1,fq_mat_entry(f,index,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i1,0),Fp);
			fq_add(y,y,y1,Fp);
		}
	}

	// degree-2 terms
	if(d>=2){
		for(int i1=0;i1<n;i1++)
		for(int i2=i1;i2<n;i2++)
		{
			index=index+1;
			fq_set(y1,fq_mat_entry(f,index,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i1,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i2,0),Fp);
			fq_add(y,y,y1,Fp);
		}
	}

	// degree-3 terms
	if(d>=3){
		for(int i1=0;i1<n;i1++)
		for(int i2=i1;i2<n;i2++)
		for(int i3=i2;i3<n;i3++)
		{
			index=index+1;
			fq_set(y1,fq_mat_entry(f,index,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i1,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i2,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i3,0),Fp);
			fq_add(y,y,y1,Fp);
		}
	}

	// degree-4 terms
	if(d>=4){
		for(int i1=0;i1<n;i1++)
		for(int i2=i1;i2<n;i2++)
		for(int i3=i2;i3<n;i3++)
		for(int i4=i3;i4<n;i4++)
		{
			index=index+1;
			fq_set(y1,fq_mat_entry(f,index,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i1,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i2,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i3,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i4,0),Fp);
			fq_add(y,y,y1,Fp);
		}
	}

	// degree-5 terms
	if(d>=5){
		for(int i1=0;i1<n;i1++)
		for(int i2=i1;i2<n;i2++)
		for(int i3=i2;i3<n;i3++)
		for(int i4=i3;i4<n;i4++)
		for(int i5=i4;i5<n;i5++)
		{
			index=index+1;
			fq_set(y1,fq_mat_entry(f,index,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i1,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i2,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i3,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i4,0),Fp);
			fq_mul(y1,y1,fq_mat_entry(x,i5,0),Fp);
			fq_add(y,y,y1,Fp);
		}
	}

}

void L_Eval(int j,fq_mat_t f,fq_mat_t s_j,fq_t out_j){
	// f: input length-n
	//    output length-1
	//    degree-d

    // Eval f(s_j)
	Eval(f,s_j,out_j);
}

// ************ Ver algorithm **********************

void La_intpoly_coeff(fq_mat_t L0){
	//interpolate f such that f(j)=y(j), return res=f(0)
	// locs[1..m]
	// vals[1..m]

	// compute L_j(0) (:=fq_mat_entry(L0,j,0))
	fq_t ksubj;
	fq_init(ksubj,Fp);

	for (int j = 0; j < m; j ++){
		fq_one(fq_mat_entry(L0,j,0),Fp);

		for (int k = 1; k <= m; k++){
			if(k != (j + 1)){
				fq_mul_ui(fq_mat_entry(L0,j,0),fq_mat_entry(L0,j,0),k,Fp);
				fq_set_si(ksubj,k-(j+1),Fp);
				fq_div(fq_mat_entry(L0,j,0),fq_mat_entry(L0,j,0),ksubj,Fp);
			}
		}
	}
}

int L_Ver(fq_mat_t L0, fq_t * out, fq_t y){
	// out[1..m]
	// return 1 if y is correct; otherwise return 0
	
    // Generating psi(u)
	// psi: input: length-1
	//     output: length-1
	//     degree-t
	//     psi(0) = 0
	fq_poly_t psi;
	fq_poly_init(psi,Fp);
	fq_poly_randtest(psi, state, t + 1, Fp);

	fq_t zero;
	fq_init(zero,Fp);
	fq_zero(zero,Fp);
	fq_poly_set_coeff(psi,0,zero,Fp);
	fq_clear(zero,Fp);


	fq_t temp;
	fq_init(temp,Fp);

	//interpolate y=F(0)
	fq_zero(y,Fp);
	for(int j = 0; j < m; j++){
		fq_mul(temp,out[j],fq_mat_entry(L0,j,0),Fp);
		fq_add(y,y,temp,Fp);
	}

	//interpolate z=G(0)
	//compute psiout: psi(j)f(s_j)
	fq_t psiout[m];
	for(int j = 0; j < m; j ++){
		fq_init(psiout[j],Fp);
		fq_set_ui(temp,j+1,Fp);
		fq_poly_evaluate_fq(psiout[j],psi,temp,Fp);
		fq_mul(psiout[j],psiout[j],out[j],Fp);
	}

	fq_t z;
	fq_init(z,Fp);
	fq_zero(z,Fp);	
	for(int j = 0; j < m; j++){
		fq_mul(temp,psiout[j],fq_mat_entry(L0,j,0),Fp);
		fq_add(z,z,temp,Fp);
	}
	

	int res = fq_is_zero(z,Fp);


	fq_poly_clear(psi, Fp);
	fq_clear(temp, Fp);
	fq_clear(z, Fp);
	for (int j = 0; j < m; j ++){
		fq_clear(psiout[j], Fp);
	}

	return res;
}


int main(){
	int n_list[9];
	n_list[0] = 1413;
	n_list[1] = 180;
	n_list[2] = 68;
	n_list[3] = 39;
	n_list[4] = 27;
	n_list[5] = 21;
	n_list[6] = 17;
	n_list[7] = 15;
	n_list[8] = 13;

    for (int index = 0; index <= 3; index ++){
		int nn = n_list[index];
		int dd = index + 2;
		printf("n = %d, d = %d\n", nn,dd);

	    for (int times = 1; times <= 5; times ++){

			init(nn,dd);  // Initialize public parameters
			
			// Data x
		    fq_mat_t x;
			fq_mat_init(x,n,1,Fp);
		    fq_mat_randtest(x,state,Fp);
		      
		    if (DEBUG) {
			    printf("x: ");
			    fq_mat_print_pretty(x,Fp);
			    printf("\n");
		    }
		    
		    // Shares s_1,...,s_j (in_j)
			fq_mat_t * in;
		    in=malloc(sizeof(fmpz_mat_t)*m);
		    for(int j = 0;j < m; j ++)
				fq_mat_init(in[j],n,1,Fp);

		    // n-variate function f of degree d
			// N=C(n+d,d) is the number of monomials in f
			fmpz_t Nm;
			fmpz_init(Nm);
			fmpz_bin_uiui(Nm,n+d,d);
			N=fmpz_get_ui(Nm);

		    fq_mat_t f;
			fq_mat_init(f,N,1,Fp);
			fq_mat_randtest(f,state,Fp);

		    // Output of servers (out_j)
		    fq_t * out=malloc(sizeof(fq_t)*m);
		    for(int j=0;j<m;++j){
		        fq_init(out[j],Fp);
				fq_zero(out[j],Fp);
		    }



		//**********************************
			
		    // Share

		    // printf("***Sharing***\n");
			clock_t share_start,share_end;
			double share_time = 0;

			share_start = clock();
			L_Share(x,in);
			share_end = clock();
			share_time = (double) (share_end- share_start)/CLOCKS_PER_SEC;
		    printf("Time of Share: %f ms\n",share_time*1000);


			// Eval
			// printf("***Evaling****\n");	

			clock_t eval_start,eval_end;
			double eval_time;
		    eval_time = 0;

		    double total_eval_time = 0;
			for (int j = 0;j < m; j ++){
			    eval_start=clock();
		        L_Eval(j,f,in[j],out[j]);
			    eval_end=clock();
			    total_eval_time += (double) (eval_end-eval_start)/CLOCKS_PER_SEC;
			    if ( (double) (eval_end-eval_start)/CLOCKS_PER_SEC > eval_time ) {
			    	eval_time = (double) (eval_end-eval_start)/CLOCKS_PER_SEC;
			    }
		    }
		    printf("Time of Max_Eval: %f ms\n",eval_time*1000);
		    printf("Time of total Eval: %f ms\n",total_eval_time*1000);

		    // Ver
		    // printf("***Verifying***\n");

			//preprocess Lagrange interpolation coefficients
			fq_mat_t L0;
			fq_mat_init(L0,m,1,Fp);
			// fq_mat_entry(L0,j,0): L_j(0)= \prod_{k=1,k!=j}^m k/(k-j)
			La_intpoly_coeff(L0); 

			if (DEBUG) {
				printf("L_1(0)..L_m(0):");
				fq_mat_print_pretty(L0,Fp);
			}  
			
			

		    int IsCorrect;
		    fq_t y;
		    fq_init(y,Fp);
		    clock_t Ver_start,Ver_end;
		    Ver_start=clock();
		    IsCorrect = L_Ver(L0,out,y);
		    Ver_end=clock();
			
			if (DEBUG) {
				printf("Ac? %d, y=",IsCorrect);
				fq_print_pretty(y,Fp);
				printf("\n");
			}  
		    
		    double Ver_time= (double) (Ver_end-Ver_start)/CLOCKS_PER_SEC;
		    printf("Time of Ver: %f ms\n",Ver_time*1000);

			



			fq_t fx;
		    fq_init(fx,Fp);
		    // Eval f(x)
		    clock_t fx_start,fx_end;
		    fx_start=clock();
		    Eval(f,x,fx);
		    fx_end=clock();

			if (DEBUG) {
				printf("f(x)=");
				fq_print_pretty(fx,Fp);
				printf("\n");
			}

		    double fx_time= (double) (fx_end-fx_start)/CLOCKS_PER_SEC;
		    printf("Time of directly eval f(x): %f ms\n",fx_time*1000);


			// clear memory
		    fq_mat_clear(f,Fp);
			fq_mat_clear(x,Fp);
			for(int j=0;j<m;++j){
		        fq_clear(out[j],Fp);
		    }
			fq_mat_clear(L0,Fp);
			fq_clear(y,Fp);
		    fq_clear(fx,Fp);
		    
		    for(int j = 0;j < m; j ++)
				fq_mat_clear(in[j],Fp);
		}
	}
    return 0;
}