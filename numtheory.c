#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include <assert.h>
#include "randstate.h"




void gcd(mpz_t d, mpz_t a, mpz_t b){

	mpz_t t;
	mpz_t a2;
	mpz_t b2;
	
	mpz_init(t);
	mpz_init(a2);
	mpz_init(b2);
	
	mpz_set(a2,a);
	mpz_set(b2,b);

	while (mpz_cmp_ui(b2, 0) != 0){
	
	mpz_set(t, b2);
	mpz_mod(b2, a2, b2);
	mpz_set(a2, t);
	
	}
	mpz_set(d, a2);
	
	mpz_clear(t);
	mpz_clear(a2);
	mpz_clear(b2);
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n){

	mpz_t q;
	mpz_init(q);
	mpz_t temp;
	mpz_t temp2;
	mpz_init(temp);
	mpz_init(temp2);
	
	mpz_cmp(o,a);	
	
	mpz_t r;
	mpz_t r_prime;
	mpz_t t;
	mpz_t t_prime;
	
	mpz_init(r);
	mpz_init(r_prime);
	mpz_init(t);
	mpz_init(t_prime);
	//////////////////////////////////////////
	
	mpz_set(r, n);
	mpz_set(r_prime, a);
	
	mpz_set_str(t, "0", 10);
	mpz_set_str(t_prime, "1", 10);
	
	while (mpz_cmp_ui(r_prime, 0) != 0){
		
		mpz_fdiv_q(q, r, r_prime);
		
		
		//temp = r_prime
		mpz_set(temp, r_prime);
		// r = r - q*r_prime
		mpz_submul(r, q, r_prime);
		// r_prime = r
		mpz_set(r_prime, r);
		//r = temp (old r_prime)
		mpz_set(r, temp);
		
		
		mpz_set(temp, t_prime);
		mpz_submul(t, q, t_prime);
		mpz_set(t_prime, t);
		mpz_set(t, temp);
	
	}
	if (mpz_cmp_ui(r,1) > 0){
		return;
	}
	if (mpz_cmp_ui(t,0) < 0){
	
		mpz_add(t, t, n);
	}
	
	mpz_set(o, t);

	mpz_clear(temp2);
	mpz_clear(temp);
	mpz_clear(q);
	mpz_clear(r);
	mpz_clear(r_prime);
	mpz_clear(t);
	mpz_clear(t_prime);
}



void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n){
	
	mpz_t mult;
	mpz_init(mult);
	
	mpz_t temp;
	mpz_init(temp);
	
	
	mpz_t v;
	mpz_init_set_str(v, "1", 10);
	
	mpz_t p;
	mpz_init(p);
	mpz_set(p, a);
	
	mpz_t d2;
	mpz_init(d2);
	mpz_set(d2, d);
		
	while(mpz_cmp_ui(d2, 0) > 0){
	
		mpz_mod_ui(temp, d2, 2);
		if(mpz_cmp_ui(temp, 1) == 0){
			
			mpz_mul(mult, v, p);
			mpz_mod(v, mult, n);
		}
		mpz_mul(mult, p, p);
		mpz_mod(p, mult, n);
		
		mpz_fdiv_q_ui(d2, d2, 2);
	}
	mpz_set(o, v);
	
	
	mpz_clear(mult);
	mpz_clear(temp);
	mpz_clear(v);
	mpz_clear(p);
	mpz_clear(d2);
}

bool is_prime(mpz_t n, uint64_t iters){
	
	////////////////////////////
	if (mpz_cmp_ui(n, 1) <= 0){
		
		return false;
	}
	
	if (mpz_cmp_ui(n, 2) == 0){
		return true;
	}
	
	//printf("iters:%l\n", iters);
	//  n-1 = 2^s * r
	//Default s = 0,  r = n-1;
	
	mpz_t s; //Cleared
	mpz_t r; //Cleared
	mpz_t n_copy; //Cleared
	mpz_init(s);
	mpz_init(r);
	mpz_init(n_copy);
	
	mpz_set(n_copy, n);
	
	
	
	mpz_t temp; //Cleared
	mpz_init(temp);
	mpz_t r_mod_two; //Cleared
	mpz_init(r_mod_two);
	
	mpz_t n_minus_one; //Cleared
	mpz_init(n_minus_one);
	mpz_sub_ui(n_minus_one, n, 1);
	
	mpz_set_str(s, "0", 10);
	mpz_sub_ui(temp, n, 1);
	mpz_set(r, temp);
	
	mpz_mod_ui(r_mod_two, r, 2);
	
	//gmp_printf("R%2: %Zd\n", r_mod_two);
	while(mpz_cmp_ui(r_mod_two, 1) != 0){
		
		
		mpz_fdiv_q_ui(n_copy, temp , 2);
		//S + 1
		mpz_add_ui(s, s, 1);
		
		//mpz_sub(temp, n_copy, one);
		mpz_set(temp, n_copy);
		mpz_set(r, n_copy);
		mpz_mod_ui(r_mod_two, r, 2);
		//gmp_printf("Loop:Zd * %Zd", s, r);
	}
	
	//gmp_printf("2^%Zd * %Zd:   n is %Zd\n", s, r, n);
	
	//mpz_t i; //Cleared
	//mpz_init_set_str(i, "1", 10);
	
	mpz_t y; //Cleared
	mpz_init(y);
	
	
	//gmp_randstate_t state;
	//gmp_randinit_default(state);
	
	mpz_t a; //Cleared
	mpz_init(a);
	
	mpz_t s_minus_one; //Cleared
	mpz_init(s_minus_one);
	mpz_sub_ui(s_minus_one, s, 1);
	
	mpz_t n_minus_four; //Cleared
	mpz_init(n_minus_four);
	mpz_sub_ui(n_minus_four, n, 2);
	mpz_sub_ui(n_minus_four, n_minus_four, 2);
	
	mpz_t j; //Cleared
	mpz_init(j);
	
	mpz_t two; //Cleared
	mpz_init_set_str(two, "2", 10);
	
	//printf("--------------------------\nBefore For Loop\n--------------------------\n");
	for(uint64_t i2 = 1; i2 < iters; i2++){
	
		//choose random a {2...n-2}
		
		if (mpz_cmp_ui(n, 4) == 0){
			mpz_set_ui(a, 2);
		}else{
		mpz_urandomm(a, state, n_minus_four);
		mpz_add_ui(a, a, 2);
		}
		
		pow_mod(y, a, r, n);
		
		
		if((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus_one) != 0)){
		
			//j = 1,
			mpz_set_str(j, "1", 10);
			//printf("--------------------------\nBefore While Loop\n--------------------------\n");
			while((mpz_cmp(j, s_minus_one) <= 0) && (mpz_cmp(y, n_minus_one) != 0)){
				
				pow_mod(y, y, two, n);
				//gmp_printf("%Zd\n", y);
				if(mpz_cmp_ui(y, 1) == 0){
					//gmp_printf("returning false since %Zd == %Zd\n", y, one);
					mpz_clear(s);
					mpz_clear(r);
					mpz_clear(n_copy);
					mpz_clear(two);
					mpz_clear(temp);
					mpz_clear(r_mod_two);
					mpz_clear(n_minus_one);
					//mpz_clear(i);
					mpz_clear(y);
					mpz_clear(a);
					mpz_clear(n_minus_four);
					mpz_clear(j);
					mpz_clear(s_minus_one);
					return false;
				}
				mpz_add_ui(j, j, 1);
			}
			if((mpz_cmp(y, n_minus_one) != 0)){
				//gmp_printf("returning false since %Zd != %Zd\n", y, one);
				mpz_clear(s);
				mpz_clear(r);
				mpz_clear(n_copy);
				mpz_clear(two);
				mpz_clear(temp);
				mpz_clear(r_mod_two);
				mpz_clear(n_minus_one);
				//mpz_clear(i);
				mpz_clear(y);
				mpz_clear(a);
				mpz_clear(n_minus_four);
				mpz_clear(j);
				mpz_clear(s_minus_one);
				return false;
				
			}
		}
	}
	
	
	
	mpz_clear(s);
	mpz_clear(r);
	mpz_clear(n_copy);
	mpz_clear(two);
	mpz_clear(temp);
	mpz_clear(r_mod_two);
	mpz_clear(n_minus_one);
	//mpz_clear(i);
	mpz_clear(y);
	mpz_clear(a);
	mpz_clear(n_minus_four);
	mpz_clear(j);
	mpz_clear(s_minus_one);
	return true;
}


void make_prime(mpz_t p, uint64_t bits, uint64_t iters){

	//gmp_randstate_t state;
	//gmp_randinit_default(state);
	mpz_t val;
	mpz_init(val);
	
	mpz_urandomb(val, state, bits);
	
	while((mpz_sizeinbase(val, 2) < bits) || !is_prime(val, iters)){
	
		mpz_urandomb(val, state, bits);
		mpz_setbit(val, 0);
		//gmp_printf("Size:%lu\n", mpz_sizeinbase(val, 2));
	}
	
	//gmp_printf("created:%Zd\n", val);
	
	mpz_set(p, val);
	
	mpz_clear(val);
}


