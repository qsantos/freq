/*\
 *  Simple frequency analasys tool for cryptography
 *  Copyright (C) 2013  Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

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
	printf("quicksort(%p, %p, %i, %p)\n", left, right, blocksize, buffer);
	if (left >= right)
		return;
	
	// computes some blocksize-aligned index in the left-right range
	// (somewhat around the middle)
	// left+right / 2 may overflow size_t
	char* pivotIdx = (char*) ((size_t)left + blocksize  *  (((size_t)right-(size_t)left)/2 / blocksize));
	printf("%p\n", pivotIdx);
	printf("v = 0x");for(size_t i=0;i<blocksize;i++)printf("%.2x",(u8)pivotIdx[i]);printf("\n");
	printf("v = 0x");for(size_t i=0;i<blocksize;i++)printf("%.2x",(u8)right[i]);printf("\n");
	swap(pivotIdx, right, blocksize, buffer);

	char* pivotValue = right;
	char* storeIndex = left;
	for (char* i = left; i < right; i += blocksize)
		if (bstrncmp(i, pivotValue, blocksize) < 0)
		{
			swap(i, storeIndex, blocksize, buffer);
			storeIndex += blocksize;
		}
	printf("%p\n", storeIndex);
	swap(storeIndex, right, blocksize, buffer);

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
	quicksort(content, content + size - blocksize, blocksize, buffer);
	printf("Sorted\n");


	// count similar blocks
	size_t* counts    = NULL;
	size_t  n_counts  = 0;
	size_t  a_counts  = 0;

	size_t  cur_count = 0;
	char*   cur_block = content;
	for (char* block = content; block <= content + size - blocksize; block += blocksize)
	{
		if (bstrncmp(block, cur_block, blocksize) != 0)
		{
			if (n_counts >= a_counts)
			{
				a_counts = a_counts ? 2*a_counts : 1;
				counts = realloc(counts, a_counts*sizeof(size_t));
			}
			counts[n_counts] = cur_count;
			memcpy(content + n_counts*blocksize, cur_block, blocksize);
			n_counts++;

			cur_count = 0;
			cur_block = block;
		}

		cur_count++;
	}

	for (size_t i = 0; i < n_counts; i++)
	{
		printf("%u ", counts[i]);
		for (size_t j = 0; j < blocksize; j++)
			printf("%.2x", (u8) content[blocksize*i+j]);
		printf("\n");
	}

	free(counts);
	free(content);
	free(buffer);
	return 0;
}
