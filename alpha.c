#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LANG french
#include "freq.h" // letter frequencies

static char* readFile(FILE* f)
{
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* ret = malloc(size+1);
	fread(ret, 1, size, f);
	ret[size] = 0;
	return ret;
}

static void computeFreqs(float freq[26], char* target, int offset, int step)
{
	float total = 1;
	memset(freq, 0, 26*sizeof(float));
	int i = 0; // current position in the key

	char* c = target;
	for (; *c && offset; c++)
		if (('A' <= *c && *c <= 'Z') || ('a' <= *c && *c <= 'z'))
			offset--;
	for (; *c; c++)
	{
		if ('a' <= *c && *c <= 'z')
			*c -= 'a' - 'A';

		if ('A' <= *c && *c <= 'Z' && i++ == 0)
		{
			freq[*c-'A']++;
			total++;
		}

		if (i >= step)
			i = 0;
	}
	for (int i = 0; i < 26; i++)
		freq[i] /= total;
}

static char bestShift(float freq[26])
{
	// Caesar Cipher: find the best shift
	float best_score = 26; // one want to minimize this value
	int best_shift = 0;
	for (int i = 0; i < 26; i++)
	{
		float score = 0;
		for (int j = 0; j < 26; j++)
		{
			float diff = freq[(j+i)%26] - LANG[j];
			score += diff*diff;
		}
		if (score < best_score)
		{
			best_score = score;
			best_shift = i;
		}
	}
	return best_shift;
}

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s keylen file\n"
		"Find the most probable key for a Caesar or a Vigenère cipher.\n"
		"\n"
		"  keylen  assumed length of the key\n"
		"  file    input file\n"
		,
		argv[0]
	);
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		usage(argc, argv);
		exit(0);
	}

	FILE* f = fopen(argv[2], "r");
	if (!f)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[2]);
		exit(1);
	}
	char* content = readFile(f);
	fclose(f);

	size_t keylen = atoi(argv[1]);
	char* key = malloc(keylen+1);
	key[keylen] = 0;

	for (size_t i = 0; i < keylen; i++)
	{
		float freq[26];
		computeFreqs(freq, content, i, keylen);
		key[i] = 'A' + bestShift(freq);
	}

	if (keylen == 1)
		printf("%i\n", key[0] - 'A');
	else
		printf("%s\n", key);

	free(key);
	free(content);
	return 0;
}
