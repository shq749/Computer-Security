#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>

//function declarations
int SB_Encrypt(FILE *fpPlainFile, FILE *fpCipherFile, char *szPassword);
static unsigned long sdbm(unsigned char *str);
int KeystreamGenerator();
void ShuffleXor(unsigned char * temp_block);

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
	SB_Encrypt(fpPlainFile, fpCipherFile, szPassword);//Simple Block Cipher
	fclose(fpPlainFile);
	fclose(fpCipherFile);
	return 0;
}

int SB_Encrypt(FILE *fpPlainFile, FILE *fpCipherFile, char *szPassword)//Simple Block Cipher
{
	unsigned char keystream_vector[16];
	unsigned char temp_block[16];
	int count,i;
	if(szPassword == NULL || szPassword[0] == '\0')
	{
		fprintf(stderr, "warning: empty password\n");
		szPassword = "";
	}
	unsigned long seed = sdbm(szPassword);
	fprintf(stderr, "using seed=%lu from password=\"%s\"\n", seed, szPassword);
	
	g_X = seed % 256;
	
	//Create an initialization vector (IV)
	for(count = 0;count <= 15;count++)
	{
		keystream_vector[count] = KeystreamGenerator();
	}

	while((count = fread(temp_block, 1, 16, fpPlainFile)) == 16)
	{
		for(count = 0;count <= 15;count++)
		{
			temp_block[count] ^= keystream_vector[count];
		}
		ShuffleXor(temp_block);
		//keystream_vector[16] = temp_block[16];
		for(i=0;i<16;i++)
		{
			keystream_vector[i] = temp_block[i];
		}
		if(fwrite(temp_block, 1, 16, fpCipherFile) != 16)
		{
			fprintf(stderr, "write error\n");
			return 1;
		}
	}
	
	//it’s the last block, add padding
	unsigned char padding = (unsigned char)(16 - count);
	if(g_DebugInfo != 0)
	{
		printf("padding from byte %d: value=%d\n", count, padding);
	}
	while(count <= 15)
	{
		temp_block[count] = padding;
		count++;
	}
	for(count = 0;count <= 15;count++)
	{
		temp_block[count] ^= keystream_vector[count];
	}
	ShuffleXor(temp_block);
	//keystream_vector[16] = temp_block[16];
	for(i=0;i<16;i++)
	{
		keystream_vector[i] = temp_block[i];
	}
	if(fwrite(keystream_vector, 1, 16, fpCipherFile) != 16)
	{
		fprintf(stderr, "write error\n");
	}
	return 0;
}

void ShuffleXor(unsigned char * temp_block)
{
	unsigned char keystream_vector[16];
	int i,j,k,m,n;
	for(i=0;i<=15;i++)
	{
		keystream_vector[i] = KeystreamGenerator();
	}
	if(g_DebugInfo != 0)
	{
		printf("\nbefore shuffle: [ ");
		for(j=0;j<=15;j++)
		{
			printf("%02x ", temp_block[j]);
		}
		puts("]");
		printf("keystream: [ ");
		for(k=0;k<=15;k++)
		{
			printf("%02x ", keystream_vector[k]);
		}
		puts("]");
	}
	for(i=0;i<=15;i++)
	{
		char first = keystream_vector[i] & 0xf; //lower 4 bits of the keystream
		char second = (keystream_vector[i] >> 4) & 0xf; //top 4 bits of the keystream
		if(g_DebugInfo != 0)
		{
			printf("%d: swapping (%d, %d) = [ %02x <> %02x ]\n",
				i, first, second, temp_block[first], temp_block[second]);
		}
		//swap(block[first], block[second]); //exchange the bytes
		char temp = temp_block[first];
		temp_block[first] = temp_block[second];
		temp_block[second] = temp;
	}
	if(g_DebugInfo != 0)
	{
		printf("after shuffle: [ ");
		for(m=0;m<=15;m++)
		{
			printf("%02x ", temp_block[m]);
		}
		puts("]");
	}
	for(i=0;i<=15;i++)
	{
		temp_block[i] ^= keystream_vector[i];
	}
	if(g_DebugInfo != 0)
	{
		printf("after xor with keystream: [ ");
		for(n=0;n<=15;n++)
		{
			printf("%02x ", temp_block[n]);
		}
		puts("]");
	}
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
