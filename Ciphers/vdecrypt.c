#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <getopt.h>

//function declarations
size_t LoadKeyFile(char *szFileName, char **pszKey);
int BVC_Decrypt(FILE *fpCipherFile, FILE *fpPlainFile, char *szKey, size_t nKeyLength);


//global variables
int g_DebugInfo = 0;


//function implementations

int main(int argc, char **argv)
{
	char * szKey = NULL;
	size_t nKeyLength;
	int bOptInvalid = 0;
	int nOption;
	while( (nOption = getopt(argc, argv, "dk:")) != -1 )
	{
		switch(nOption)
		{
			case 'd':
				g_DebugInfo = 1;
				break;
			case 'k':
				szKey = optarg;
				break;
			case '?':
				bOptInvalid = 1;
		}
	}
	if(bOptInvalid != 0)
	{
		fprintf(stderr, "usage: %s [-d] [keyfile | -k key] ciphertextfile plaintextfile\n", argv[0]);
		exit(1);
	}
	if(szKey == NULL)
	{
		if(argc - 3 == optind)
		{
			nKeyLength = LoadKeyFile(argv[optind++], &szKey);
			if(nKeyLength != 0)
			{
				fprintf(stderr, "keyfile=%s, length=%lu\n", argv[optind-1], nKeyLength);
			}
			else
			{
				fprintf(stderr, "bad key file: %s\n", argv[optind-1]);
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, "usage: %s [-d] [keyfile | -k key] ciphertextfile plaintextfile\n", argv[0]);
			exit(1);
		}
	}
	else
	{
		if(argc - 2 == optind)
		{
			nKeyLength = strlen(szKey);
			if(g_DebugInfo != 0)
			{
				printf("using key = \"%s\"\n", szKey);
			}
		}
		else
		{
			fprintf(stderr, "got key but wrong args: optind=%d argc=%d\n", optind, argc);
			fprintf(stderr, "usage: %s [-d] [keyfile | -k key] ciphertextfile plaintextfile\n", argv[0]);
			exit(1);
		}
	}
	char * szCipherFile = argv[optind];
	char * szPlainFile = argv[optind + 1];
	if(g_DebugInfo != 0)
	{
		printf("plaintextfile = \"%s\", ciphertextfile = \"%s\"\n", szPlainFile, szCipherFile);
	}
	FILE *fpCipherFile = fopen(szCipherFile, "r");
	FILE *fpPlainFile;
	if(fpCipherFile != NULL)
	{
		fpPlainFile = fopen(szPlainFile, "w");
		if(fpPlainFile != NULL)
		{
			BVC_Decrypt(fpCipherFile, fpPlainFile, szKey, nKeyLength);
		}
		else
		{
			fprintf(stderr, "%s: cannot open plaintext file for writing \"%s\"\n", argv[0], szPlainFile);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "%s: cannot open ciphertext file for reading \"%s\"\n", argv[0], szCipherFile);
		exit(1);
	}
	fclose(fpCipherFile);
	fclose(fpPlainFile);

	return 0;
}

size_t LoadKeyFile(char *szFileName, char **pszKey)
{
	int fd = open(szFileName, O_RDONLY);
	if(pszKey != NULL)
	{
		if(fd >= 0)
		{
			struct stat stat_buf;
			if( fstat(fd, &stat_buf) >= 0)
			{
				size_t nKeyFileSize = stat_buf.st_size;
				(*pszKey) = malloc(nKeyFileSize);
				if((*pszKey) != NULL)
				{
					if(nKeyFileSize != 0)
					{
						if(read(fd, *pszKey, nKeyFileSize) == nKeyFileSize)
						{
							if(g_DebugInfo != 0)
							{
								printf("read key file: \"%s\" %lu bytes\n", szFileName, nKeyFileSize);
							}
							return nKeyFileSize;
						}
						else
						{
							fprintf(stderr, "cannot read key file data: %s\n", szFileName);
						}
					}
					else
					{
						fprintf(stderr, "empty key file\n");
						exit(1);
					}
				}
				else
				{
					fprintf(stderr, "cannot allocate %lu for key\n", (unsigned long)fd);
				}
			}
			else
			{
				fprintf(stderr, "cannot stat key file %s\n", szFileName);
			}
		}
		else
		{
			fprintf(stderr, "cannot open key file %s\n", szFileName);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "readkey - NULL buffer address\n");
	}
	return 0;
}

int BVC_Decrypt(FILE *fpCipherFile, FILE *fpPlainFile, char *szKey, size_t nKeyLength)
{
	int i = 0;
	if(szKey != NULL)
	{
		int ciphertext;
		int row,column; //Simulate rows and columns in table 256x256
		while((ciphertext = fgetc(fpCipherFile)) != EOF)
		{
			column = szKey[i];
			row = (ciphertext + 256 - column) % 256; //Binary Vigen??re Cipher
			if(g_DebugInfo != 0)
			{
				printf("found %u(%c) in column %d(%c) at row %d(%c)\n",
					(unsigned char)ciphertext, isprint((unsigned char)ciphertext) ? (unsigned char)ciphertext : '?',
					(unsigned char)column, isprint((unsigned char)column) ? (unsigned char)column : '?',
					(unsigned char)row, isprint((unsigned char)row) ? (unsigned char)row : '?');
			}
			i++;
			if(i == nKeyLength)
			{
				i = 0;
			}
			putc(row,fpPlainFile); // row = plaintext
		}
		return 0;
	}
	else
	{
		fprintf(stderr, "unexpected error: null key\n");
		return 1;
	}
}
