How to run and use keygen, encrypt, and decrypt.
================================================

IGNORE "test.c"!!!!!
--------------------
>
> This is a testing file and is NOT intended to be used!
>

What does it do?
----------------
> Generates a ssh key and encryptes/decryptes files using the ssh methods.
> You may use each of these functions separately or pipe them together!
>
> Note: Generating extremely long keys will on average, take under 20 seconds.
> However, due to the nature of random number generation, it may occasionally take longer!
>
>


Command Line Options
--------------------
>Keygen
>------------------------------------------------------------------------------------------------
>Usage:  ./keygen [options]\
>  ./keygen generates a public / private key pair, placing the keys into the public and private\
>  key files as specified below. The keys have a modulus (n) whose length is specified in\
>  the program options.\
>    -s <seed>   : Use <seed> as the random number seed. Default: time()\
>    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\
>    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\
>    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\
>    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\
>    -v          : Enable verbose output.\n\
>    -h          : Display program synopsis and usage.\
>
>
>
>Encrypt
>------------------------------------------------------------------------------------------------
>Usage: ./encrypt [options]\
>  ./encrypt encrypts an input file using the specified public key file,\
>  writing the result to the specified output file.\
>    -i <infile> : Read input from <infile>. Default: standard input.\
>    -o <outfile>: Write output to <outfile>. Default: standard output.\
>    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\
>    -v          : Enable verbose output.\
>    -h          : Display program synopsis and usage.\
>
>
>Decrypt
>------------------------------------------------------------------------------------------------
>Usage: ./decrypt [options]\
>  ./decrypt decrypts an input file using the specified private key file,\
>  writing the result to the specified output file.\
>    -i <infile> : Read input from <infile>. Default: standard input.\
>    -o <outfile>: Write output to <outfile>. Default: standard output.\
>    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\
>    -v          : Enable verbose output.\
>    -h          : Display program synopsis and usage.\
>
>
>
>


How to build and run the program.
=================================

Step One:
---------
> Ensure that the files:

> "keygen.c"\
> "encrypt.c"\
> "decrypt.c"\
> "Makefile"\
> "numtheory.c"\
> "numtheory.h"\
> "randstate.c"\
> "randstate.h"\
> "rsa.c"\
> "rsa.h"\
> "test.c"\
>

> Are all inside the same directory.
>
Step Two:
---------
> Open the terminal in the directory and type "make"
>
Step Three:
-----------
> Use the usage methods located above in the "Command Line Options" section
>
> You may also use the -h with the program to print the usage to the terminal instead
> of looking through the README
