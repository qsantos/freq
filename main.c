#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef unsigned char u8;

static char bstrncmp(const char* a, const char* b, size_t n)
{
	for (; n; n--, a++, b++)
		if (*a != *b) 
			return *(u8*)a < *(u8*)b ? -1 : 1;
	return 0;
}

static void swap(char* a, char* b, size_t blocksize, char* buffer)
{
	memcpy(buffer, a,      blocksize);
	memcpy(a,      b,      blocksize);
	memcpy(b,      buffer, blocksize);
}

static void quicksort(char* left, char* right, size_t blocksize, char* buffer)
{
	if (left >= right)
		return;
	
//	swap((left+right)/2, right); // TODO
	char* pivotValue = right;
	char* storeIndex = left;
	for (char* i = left; i < right; i += blocksize)
		if (bstrncmp(i, pivotValue, blocksize) < 0)
		{
			swap(i, storeIndex, blocksize, buffer);
			storeIndex += blocksize;
		}
	swap(storeIndex, right, blocksize, buffer);

	if (storeIndex > left)
		quicksort(left, storeIndex - blocksize, blocksize, buffer);
	quicksort(storeIndex + blocksize, right, blocksize, buffer);
}

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s blocksize file\n"
		"\n"
		"PARAMS:\n"
		" blocksize  the byte-length of blocks\n"
		" file       the name of the file to be analyzed\n"
		,
		argv[0]
	);
}

int main(int argc, char** argv)
{
	if (argc <= 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
	{
		usage(argc, argv);
		exit(0);
	}
	else if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)
	{
		printf("frequency\n");
		printf("Compiled on %s at %s\n", __DATE__, __TIME__);
		exit(0);
	}

	if (argc < 3)
	{
		usage(argc, argv);
		exit(1);
	}


	// initialize parameters
	size_t blocksize = atoll(argv[1]);
	char*  buffer    = malloc(blocksize);
	assert(buffer);


	// load the file
	FILE* f = fopen(argv[2], "r");

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* content = malloc(size);
	assert(content);
	fread(content, 1, size, f);
	fclose(f);


	// sort all that random data
	quicksort(content, content + size, blocksize, buffer);


	for (size_t i = 0; i < size; )
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 8; k++, i++)
				printf("%.2x", (u8) content[i]);
			printf(" ");
		}
		printf("\n");
	}

	return 0;
}
