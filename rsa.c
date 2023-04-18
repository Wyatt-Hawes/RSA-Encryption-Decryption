#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include <stdlib.h>
#include "randstate.h"


//Returns (p-1)(q-1)/gcd(p-1,q-1)
void lambda_n(mpz_t result, mpz_t p, mpz_t q){

	mpz_t p_minus_1;
	mpz_t q_minus_1;
	
	mpz_init(p_minus_1);
	mpz_init(q_minus_1);
	
	mpz_sub_ui(p_minus_1, p, 1);
	mpz_sub_ui(q_minus_1, q, 1);
	//gmp_printf("p-1:%Zd\n", p_minus_1);
	//gmp_printf("q-1:%Zd\n", q_minus_1);
	
	
	
	//printf("2\n");
	
	mpz_t gcd_p_q;
	mpz_init(gcd_p_q);
	gcd(gcd_p_q, p_minus_1, q_minus_1);

	mpz_t prod_p_q;
	mpz_init(prod_p_q);
	
	mpz_mul(prod_p_q, p_minus_1, q_minus_1);
	
	mpz_fdiv_q(result, prod_p_q, gcd_p_q);
	
	mpz_clear(p_minus_1);
	mpz_clear(q_minus_1);
	mpz_clear(gcd_p_q);
	mpz_clear(prod_p_q);
}


//
// Generates the components for a new public RSA key.
// p and q will be large primes with n their product.
// The product n will be of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
// The public exponent e will have around the same number of bits as n.
// All mpz_t arguments are expected to be initialized.
//
// p: will store the first large prime.
// q: will store the second large prime.
// n: will store the product of p and q.
// e: will store the public exponent.
//
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters){
	
	uint64_t p_bits_top = (3*nbits)/4;
	uint64_t p_bits_bottom = nbits/4;
	
	
	uint64_t p_rand = (random() % (p_bits_top - p_bits_bottom)) + p_bits_bottom;
	uint64_t q_rand = nbits - p_rand;
	
	
	//printf("1\n");
	
	
	make_prime(p, q_rand, iters);
	make_prime(q, p_rand, iters);
	//printf("1.5\n");
	
	//p-1 * q-1
	mpz_mul(n, p, q);
	//gmp_printf("p:%Zd\n", p);
	//gmp_printf("q:%Zd\n", q);
	//gmp_printf("n:%Zd\n", n);

	mpz_t lcm_p_q;
	mpz_init(lcm_p_q);
	lambda_n(lcm_p_q, p, q);
	//gmp_printf("lcm:%Zd p*q:%Zd  gcd(p,q):%Zd\n", lcm_p_q, prod_p_q, gcd_p_q);
	//p(n) = (p-1)(n-1)
	
	//Key is random int from 2 < e< n   s.t. gcd(e, p(n)) = 1
	
	//printf("3\n");
	mpz_t egcd;
	mpz_init(egcd);
	//printf("before loop\n");
	do{
	//////////////////////////////////////////
	mpz_urandomb(e, state, nbits);
	///Possibly check for amount of bits in e
	
	gcd(egcd, e, lcm_p_q);
	
	//gmp_printf("e:%Zd egcd:%Zd  lcm:%Zd\n", e, egcd,lcm_p_q);
	
	}while(mpz_cmp_ui(egcd, 1) != 0 || mpz_sizeinbase(e, 2) < nbits);
	
	//printf("5\n");
	//printf("Clearing in make_pub\n");
	mpz_clear(lcm_p_q);
	mpz_clear(egcd);
}

//
// Writes a public RSA key to a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// e: the public exponent.
// s: the signature of the username.
// username: the username that was signed as s.
//
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){

	gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n",n, e ,s , username);

}

//
// Reads a public RSA key from a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// e: will store the public exponent.
// s: will store the signature.
// username: an allocated array to hold the username.
// pbfile: the file containing the public key
//
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){

	gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n",n, e ,s , username);
}





//
// Generates the components for a new private RSA key.
// Requires an accompanying RSA public key to complete the pair.
// All mpz_t arguments are expected to be initialized.
//
// d: will store the RSA private key.
// e: the precomputed public exponent.
// p: the first large prime from the public key generation.
// p: the second large prime from the public key generation.
//
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q){

	mpz_t lambda;
	mpz_init(lambda);
	
	
	lambda_n(lambda, p, q);
	mod_inverse(d, e, lambda);
	
	mpz_clear(lambda);
}

//
// Writes a private RSA key to a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// d: the private key.
// pvfile: the file to write the private key to.
//
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile){
	gmp_fprintf(pvfile, "%Zx\n%Zx\n",n, d);
}

//
// Reads a private RSA key from a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// d: will store the private key.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile){

	gmp_fscanf(pvfile, "%Zx\n%Zx\n",n, d);
}

//
// Encrypts a message given an RSA public exponent and modulus.
// All mpz_t arguments are expected to be initialized.
//
// c: will store the encrypted message.
// m: the message to encrypt.
// e: the public exponent.
// n: the public modulus.
//
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n){
	pow_mod(c, m, e, n);
}

//
// Decrypts some ciphertext given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// m: will store the decrypted message.
// c: the ciphertext to decrypt.
// d: the private key.
// n: the public modulus.
//
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n){
	pow_mod(m, c, d, n);
}

//
// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e){

	uint64_t k = ((uint64_t)(mpz_sizeinbase(n, 2)) - 1)/((uint64_t)8);
	
	uint8_t *block = calloc(k, sizeof(uint8_t));
	block[0]=0xFF;
	
	size_t num_read;
	mpz_t m;
	mpz_init(m);
	
	mpz_t out;
	mpz_init(out);
	
	while(!feof(infile)){
		num_read = fread(&block[1], 1, k-1, infile);
		mpz_import(m, num_read + 1, 1, 1, 1, 0, block);
		rsa_encrypt(out, m, e, n);
		gmp_fprintf(outfile, "%Zx\n", out);
		
	}
	free(block);
	mpz_clear(m);
	mpz_clear(out);
}


//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d){

	uint64_t k = ((uint64_t)(mpz_sizeinbase(n, 2)) - 1)/((uint64_t)8);
	uint8_t *block = calloc(k, sizeof(uint8_t));
	
	size_t num_read;
	mpz_t c;
	mpz_init(c);
	
	mpz_t m;
	mpz_init(m);
	
	mpz_t out;
	mpz_init(out);
	
	while(!feof(infile)){
	
		num_read = gmp_fscanf(infile, "%Zx\n", c);
		rsa_decrypt(m, c, d, n);
		
		mpz_export(block, &num_read, 1, 1, 1, 0, m);
		
		for(int i = 1; i < (int)num_read; i++){
		fprintf(outfile, "%c", block[i]);
		}
		
	}
	
	
	free(block);
	mpz_clear(m);
	mpz_clear(c);
	mpz_clear(out);
	
}

//
// Signs some message given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// s: will store the signed message (the signature).
// m: the message to sign.
// d: the private key.
// n: the public modulus.
//
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n){
	pow_mod(s, m, d, n);
}

//
// Verifies some signature given an RSA public exponent and modulus.
// Requires the expected message for verification.
// All mpz_t arguments are expected to be initialized.
//
// m: the expected message.
// s: the signature to verify.
// e: the public exponent.
// n: the public modulus.
// returns: true if signature is verified, false otherwise.
//
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n){

	mpz_t t;
	mpz_init(t);
	
	pow_mod(t, s, e ,n);
		

	
	if(mpz_cmp(t, m) == 0){
		mpz_clear(t);
		return true;
	}

	mpz_clear(t);
	return false;
}
