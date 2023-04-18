#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

//Maybe remove above

gmp_randstate_t state;


void randstate_init(uint64_t seed){
	gmp_randinit_default(state);
	gmp_randseed_ui(state, seed);
	
	srandom(seed);
}
void randstate_clear(void){
	gmp_randclear(state);
}
