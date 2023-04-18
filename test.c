#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include <assert.h>
#include "randstate.h"
#include "rsa.h"
#include <sys/stat.h>




int main(){

	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t e;
	mpz_t d;
	
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(e);
	mpz_init(d);
	
	mpz_set_str(p, "20", 10);
	mpz_set_str(q, "7", 10);
	mpz_set_str(n, "14", 10);
	mpz_set_str(e, "25", 10);
	//gcd(a, a, b);
	//mod_inverse(a, a, b);
	//pow_mod(a, a, b, c);
	
/*
	bool prime = is_prime(a, 50);
	//gmp_printf("pow_mod = %Zd\n", a);
	//assert(prime);
	if(prime){
		gmp_printf("%Zd is prime\n", a);
	}
	else{
		gmp_printf("%Zd is not prime\n", a);
	}
*/
	
	FILE *my_file;
	FILE *file_2;
	FILE *priv_1;
	FILE *priv_2;
	
	my_file = fopen("test.pub", "w");
	priv_1 = fopen("test.priv", "w");
	chmod("test.pub", S_IRUSR + S_IWUSR);
	chmod("test.priv", S_IRUSR + S_IWUSR);

	randstate_init(123);
	
	//make_prime(b, 20, 50);	
	//gmp_printf("Prime is %Zd\n", b);
	//printf("Make_pub\n");
	rsa_make_pub(p,q,n,e, 512, 50);
	rsa_make_priv(d, e, p, q);
	//printf("Write_pub\n");
	rsa_write_pub(n,e,p, "wyatt", my_file);
	rsa_write_priv(n, d, priv_1);
	
	fclose(my_file);
	fclose(priv_1);
	
	priv_1 = fopen("test.priv", "r");
	my_file = fopen("test.pub", "r");
	
	mpz_t one;
	mpz_t two;
	mpz_t three;
	char name[50];
	
	mpz_init(one);
	mpz_init(two);
	mpz_init(three);
	
	rsa_read_pub(one,two,three,name,my_file);
	file_2 = fopen("test2.pub", "w");
	rsa_write_pub(one,two,three,name,file_2);
	
	rsa_read_priv(one,two, priv_1);
	priv_2 = fopen("test2.priv", "w");
	rsa_write_priv(one,two,priv_2);
	
	fclose(file_2);
	fclose(priv_1);
	fclose(priv_2);
	
	
	
	FILE *input;
	FILE *output;
	
	FILE *decrypted;
	
	input = fopen("test.c", "r");
	output = fopen("test.c.enc", "w");
	
	rsa_encrypt_file(input, output, n, e);
	
	fclose(input);
	fclose(output);
	
	
	input = fopen("test.c.enc", "r");
	decrypted = fopen("decrypted.txt", "w");
	
	rsa_decrypt_file(input, decrypted, n, d);
	
	fclose(input);
	fclose(decrypted);
	
	mpz_t signature;
	mpz_t message;
	mpz_init(signature);
	mpz_init(message);
	
	mpz_set_str(message, "1337", 10);
	
	gmp_printf("message: %Zd\n", message);
	
	rsa_sign(signature, message, d, n);
	gmp_printf("enc sig: %Zx\n", signature);
	
	bool same = rsa_verify(message, signature, e, n);
	
	if (same){
		printf("true\n");
	}else{
		printf("false\n");
	}
	mpz_clear(signature);
	mpz_clear(message);
	
	
	
	mpz_t test;
	mpz_init(test);
	
	make_prime(test, 100, 50);
	gmp_printf("Num: %Zd\nSize:%lu\n",test, mpz_sizeinbase(test, 2));
	
	
	if(is_prime(test, 50)){
		printf("prime\n");
	}
	else{
		printf("not prime\n");
	}
	
	mpz_clear(test);
	
	
	
	
	
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(e);
	mpz_clear(d);
	
	mpz_clear(one);
	mpz_clear(two);
	mpz_clear(three);
	randstate_clear();
	fclose(my_file);
	
	return 0;
}

