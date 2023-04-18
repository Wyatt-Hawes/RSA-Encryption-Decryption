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
fprintf(stderr, "Usage: ./decrypt [options]\n"
"  ./decrypt decrypts an input file using the specified private key file,\n"
"  writing the result to the specified output file.\n"
"    -i <infile> : Read input from <infile>. Default: standard input.\n"
"    -o <outfile>: Write output to <outfile>. Default: standard output.\n"
"    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n"
"    -v          : Enable verbose output.\n"
"    -h          : Display program synopsis and usage.\n");


}





//./encrypt -i test.c -o test.c.enc -v
int main(int argc, char **argv){
	char *input = "stdin";
	char *output = "stdout";
	int value;
	char *private_key = "rsa.priv";
	
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
			private_key = (optarg);
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
			//print synopsis and usage to stderr
			print_usage();
			return 1;
			break;
		
		}
	}
	
	mpz_t n;
	mpz_t d;
	
	mpz_init(n);
	mpz_init(d);
	
	FILE *priv_key;
	priv_key = fopen(private_key, "r");
	
	if (priv_key == NULL){
		fprintf(stderr,"Public Key file not found\n");
		return 1;
	}
	
	
	rsa_read_priv(n, d, priv_key);
	fclose(priv_key);
	if(v){
	
		gmp_fprintf(stderr, "n-modulus (%lu bits): %Zx\n"
		"d-private exponent (%lu bits): %Zx\n"
	
	"",mpz_sizeinbase(n, 2), n,
	mpz_sizeinbase(d, 2), d);
	}
	
	FILE *dec_input;
	FILE *dec_output;
	
	//printf("Input:%s Output:%s\n",input, output);
	
	if(strcmp(input, "stdin") != 0){
		dec_input = fopen(input, "r");
	}else{
		dec_input = stdin;
	}
	
	if(strcmp(output, "stdout") != 0){
		dec_output = fopen(output, "w");
	}else{
		dec_output = stdout;
	}
	
	
	if (dec_input == NULL || dec_output == NULL){
		fprintf(stderr, "invalid input/output\n");
		return 1;
	}
	
	rsa_decrypt_file(dec_input, dec_output, n, d);
	
	
	fclose(dec_input);
	fclose(dec_output);
	
	mpz_clear(n);
	mpz_clear(d);
	return 0;	
}
