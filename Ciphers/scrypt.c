#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>

//function declarations
int Scrypt(FILE *fpPlainFile, FILE *fpCipherFile, char *szPassword);
static unsigned long sdbm(unsigned char *str);
int KeystreamGenerator();

//global variables
int g_DebugInfo = 0;
int g_X;

//function implementations

int main(int argc, char **argv)
{
	size_t nKeyLength;
	int bOptInvalid = 0;
	int nOption;
	while( (nOption = getopt(argc, argv, "d")) != -1 )
	{
		switch(nOption)
		{
			case 'd':
				g_DebugInfo = 1;
				break;
			case '?':
				bOptInvalid = 1;
		}
	}
	if(bOptInvalid != 0)
	{
		fprintf(stderr, "usage: %s [-d] password plaintextfile ciphertextfile\n", argv[0]);
		exit(1);
	}
	if(argc - 3 != optind)
	{
		fprintf(stderr, "usage: %s [-d] password plaintextfile ciphertextfile\n", argv[0]);
		exit(1);
	}
	char * szPassword = argv[optind];
	char * szPlainFile = argv[optind + 1];
	char * szCipherFile = argv[optind + 2];
	if(g_DebugInfo != 0)
	{
		printf("plaintextfile=\"%s\" ciphertextfile=\"%s\" password=\"%s\"\n", szPlainFile, szCipherFile, szPassword);
	}
	FILE *fpPlainFile = fopen(szPlainFile, "r");
	if(fpPlainFile == NULL)
	{
		fprintf(stderr, "%s: cannot open plaintext file for reading \"%s\"\n", argv[0], szPlainFile);
		exit(1);
	}
	FILE *fpCipherFile = fopen(szCipherFile, "w");
	if(fpCipherFile == NULL)
	{
		fprintf(stderr, "%s: cannot open ciphertext file for writing \"%s\"\n", argv[0], szCipherFile);
		exit(1);
	}
	Scrypt(fpPlainFile, fpCipherFile, szPassword);//Stream cipher
	fclose(fpPlainFile);
	fclose(fpCipherFile);
	return 0;
}

int Scrypt(FILE *fpPlainFile, FILE *fpCipherFile, char *szPassword)//Stream cipher
{
	if(szPassword == NULL || szPassword[0] == '\0')
	{
		fprintf(stderr, "warning: empty password\n");
		szPassword = "";
	}
	unsigned long seed = sdbm(szPassword);
	fprintf(stderr, "using seed=%lu from password=\"%s\"\n", seed, szPassword);
	g_X = seed % 256;
	int plaintext,keytext;
	while((plaintext = fgetc(fpPlainFile)) != EOF)
	{
		keytext = KeystreamGenerator();
		int ciphertext = plaintext ^ keytext; //Each byte of plaintext is XORed keytext to produce a byte of ciphertext
		if(g_DebugInfo != 0)
		{
			printf("0x%02x(%c) xor 0x%02x = 0x%02x(%c)\n",
				(unsigned char)plaintext, isprint((unsigned char)plaintext) ? (unsigned char)plaintext : '?',
				(unsigned char)keytext,
				(unsigned char)ciphertext, isprint((unsigned char)ciphertext) ? (unsigned char)ciphertext : '?');
		}
		putc(ciphertext,fpCipherFile);
	}
	return 0;
}

static unsigned long sdbm(unsigned char *str)//the sbdm hash function
{
	unsigned long hash = 0;
	int c;
	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;
	return hash;
}

int KeystreamGenerator()//linear congruential generator
{
	g_X = (1103515245 * g_X + 12345) % 256;
	return g_X;
}
