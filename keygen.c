#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "randstate.h"
#include "rsa.h"
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <sys/stat.h>

void print_usage(){

fprintf(stderr, "Usage: ./keygen [options]\n"
"  ./keygen generates a public / private key pair, placing the keys into the public and private\n"
"  key files as specified below. The keys have a modulus (n) whose length is specified in\n"
"  the program options.\n"
"    -s <seed>   : Use <seed> as the random number seed. Default: time()\n"
"    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n"
"    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n"
"    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n"
"    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n"
"    -v          : Enable verbose output.\n"
"    -h          : Display program synopsis and usage.\n");


}



int main(int argc, char **argv){
	
	int b = 1024;
	int i = 50;
	char *public_file = "rsa.pub";
	char *private_file = "rsa.priv";
	
	int value;
	
	uint64_t seed = time(NULL);
	int v = 0;
	
	
	while ((value = getopt(argc, argv, "b:i:n:d:s:vh")) != -1){
	
		switch(value){
		
		case 'b':
			b = atoi(optarg);
			break;
		
		case 'i':
			i = atoi(optarg);
			break;
			
		case 'n':
			public_file = (optarg);
			break;
			
		case 'd':
			private_file = (optarg);
			break;
			
		case 's':
			seed = atol(optarg);
			break;
		
		case 'v':
			v = 1;
			break;
		
		case 'h':
			//Print usage and program synopsis
			print_usage();
			return 0;
			break;
		default:
			//printf("invalid input");
			print_usage();
			return 1;
			break;
		
		}
	}
	
	
	//Test for invalid inputs
	
	if ((b < 50) || (b > 4096)){
		fprintf(stderr, "./keygen: Number of bits must be 50-4096, not %d.\n", b);
		print_usage();
		return 1;
	
	}
	
	if ((i < 1) || (i > 500)){
		fprintf(stderr, "./keygen: Number of iterations must be 1-500, not %d.", i);
		print_usage();
		return 1;
	
	}
	
	
	
	
	
	
	
	
	//printf("%d %d %s %s %lu %d\n",b,i,public_file,private_file,seed,v);
	
	FILE *private;
	FILE *public;
	
	//printf("open\n");
	
	private = fopen(private_file, "w");
	public =  fopen(public_file, "w");
	
	if (private == NULL || public == NULL){
		fprintf(stderr,"Invalid file input\n");
		return 1;
		
	}
	
	chmod(private_file, S_IRUSR + S_IWUSR);
	//printf("init\n");
	randstate_init(seed);
	
	//For public key storage
	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t e;
	
	//For private key storage
	mpz_t d;
	
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(e);
	
	mpz_init(d);
	
	
	
	
	rsa_make_pub(p, q, n, e, b, i);
	rsa_make_priv(d, e, p, q);
	
	char* name = getenv("USER");
	//printf("User: %s\n", name);
	
	mpz_t m;
	mpz_init(m);
	
	mpz_set_str(m, name, 62);
	
	mpz_t s;
	mpz_init(s);
	
	rsa_sign(s, m, d, n);
	
	//Writing to files
	rsa_write_pub(n, e, s, name, public);
	rsa_write_priv(n, d, private);
	
	if(v){
	
	gmp_fprintf(stderr, "username: %s\n"
	
	"user signature (%lu): %Zd\n" 
	"p (%lu bits): %Zd\n"
	"q (%lu bits): %Zd\n"
	"n - modulus (%lu bits): %Zd\n"
	"e - public exponent (%lu bits): %Zd\n"
	"d - private exponent (%lu bits): %Zd\n"
	
	
	
	
	
	
	"", name, 
		mpz_sizeinbase(s, 2), s, 
		mpz_sizeinbase(p, 2), p,
		mpz_sizeinbase(q, 2), q,
		mpz_sizeinbase(n, 2), n,
		mpz_sizeinbase(e, 2), e,
		mpz_sizeinbase(d, 2), d);
	}
	
	
	
	mpz_clear(m);
	mpz_clear(s);
	
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(e);
	
	mpz_clear(d);
	
	//printf("close\n");
	fclose(private);
	fclose(public);
	randstate_clear();
	return 0;
}
