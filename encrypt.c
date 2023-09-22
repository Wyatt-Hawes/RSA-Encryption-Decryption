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

fprintf(stderr, "Usage: ./encrypt [options]\n"
"  ./encrypt encrypts an input file using the specified public key file,\n"
"  writing the result to the specified output file.\n"
"    -i <infile> : Read input from <infile>. Default: standard input.\n"
"    -o <outfile>: Write output to <outfile>. Default: standard output.\n"
"    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n"
"    -v          : Enable verbose output.\n"
"    -h          : Display program synopsis and usage.\n");


}


//./encrypt -i test.c -o test.c.enc -v
int main(int argc, char **argv){
	char *input = "stdin";
	char *output = "stdout";
	int value;
	char *public_key = "rsa.pub";
	
	int v = 0;
	
	while ((value = getopt(argc, argv, "i:o:n:vh")) != -1){
	
		switch(value){
		
		case 'i':
			input = (optarg);
			break;
			
		case 'o':
			output = (optarg);
			break;
			
		case 'n':
			public_key = (optarg);
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
			//invalid input
			print_usage();
			return 1;
			break;
		
		}
	}
	
	char name[50] = "";
	mpz_t n;
	mpz_t e;
	mpz_t s;
	
	mpz_init(n);
	mpz_init(e);
	mpz_init(s);
	
	FILE *pub_key;
	pub_key = fopen(public_key, "r");
	
	if (pub_key == NULL){
		fprintf(stderr,"Public Key file not found\n");
		return 1;
	}
	

	rsa_read_pub(n, e, s, name, pub_key);
	fclose(pub_key);
	if(v){
	
		gmp_fprintf(stderr, "username: %s\n"
	
	"user signature (%lu): %Zx\n" 
	"n-modulus (%lu bits): %Zx\n"
	"e-public exponent (%lu bits): %Zx\n",
	
		name, 
		mpz_sizeinbase(s, 2), s, 
		mpz_sizeinbase(n, 2), n,
		mpz_sizeinbase(e, 2), e);
	}
	mpz_t m;
	mpz_init(m);
	mpz_set_str(m, name, 62);
	
	if(!rsa_verify(m, s, e, n)){
		fprintf(stderr,"Signature couldnt be verified\n");
		return 1;
		
	}
	
	FILE *enc_input;
	FILE *enc_output;
	
	//printf("Input:%s Output:%s\n",input, output);
	if(strcmp(input, "stdin") != 0){
		enc_input = fopen(input, "r");
	}else{
		enc_input = stdin;
	}
	
	if(strcmp(output, "stdout") != 0){
		enc_output = fopen(output, "w");
	}else{
		//printf("Setting to stdout\n");
		enc_output = stdout;
	}
	
	if (enc_input == NULL || enc_output == NULL){
		fprintf(stderr,"invalid input/output\n");
		return 1;
	}
	
	rsa_encrypt_file(enc_input, enc_output, n, e);
		
	fclose(enc_input);
	fclose(enc_output);
	
	mpz_clear(m);
	mpz_clear(n);
	mpz_clear(e);
	mpz_clear(s);
	return 0;	
}
