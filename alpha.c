#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LANG english
#include "freq.h" // letter frequencies

static char* readFile(FILE* f)
{
	size_t a_ret = 1;
	size_t n_ret = 0;
	char* ret = (char*) malloc(a_ret);
	assert(ret);
	while (1)
	{
		n_ret += fread(ret, 1, a_ret-n_ret, f);
		if (feof(f))
			break;
		a_ret *= 2;
		ret = (char*) realloc(ret, a_ret);
		assert(ret);
	}
	ret[n_ret] = 0;
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
		"Usage: %s MODE keylen [file]\n"
		"Find the most probable key for a Caesar or a Vigenère cipher.\n"
		"\n"
		"MODE:\n"
		"  --freqs,   -f  prints the letter frequencies of the text\n"
		"  --shift,   -s  find the most probable key for a Caesar or a\n"
		"                 Vigenère cipher, the assumed key length is to be\n"
		"                 provided.\n"
		"  --version, -v  prints information about this program\n"
		"  --help,    -h  prints this help page\n"
		,
		argv[0]
	);
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		usage(argc, argv);
		exit(0);
	}

	int curarg = 1;
	char* option = argv[curarg++];
	char freq = 0;
	if (strcmp(option, "--version") == 0 || strcmp(option, "-v") == 0)
	{
		fprintf(stderr, "alpha 0.1\n");
		fprintf(stderr, "Compiled on %s at %s\n", __DATE__, __TIME__);
		exit(0);
	}
	else if (strcmp(option, "--help") == 0 || strcmp(option, "-h") == 0)
	{
		usage(argc, argv);
		exit(0);
	}
	else if (strcmp(option, "--freqs") == 0 || strcmp(option, "-f") == 0)
	{
		freq = 1;
	}
	else if (strcmp(option, "--shift") == 0 || strcmp(option, "-s") == 0)
	{
		freq = 0;
	}
	else
	{
		fprintf(stderr, "Invalid mode '%s'\n\n", option);
		usage(argc, argv);
		exit(1);
	}

	if (curarg >= argc)
	{
		fprintf(stderr, "Missing key length\n\n");
		usage(argc, argv);
		exit(1);
	}

	size_t keylen = atoi(argv[curarg++]);

	FILE* f = curarg < argc ? fopen(argv[curarg++], "r") : stdin;
	if (!f)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[curarg-1]);
		exit(1);
	}
	char* content = readFile(f);
	fclose(f);

	if (freq)
	{
		for (size_t i = 0; i < keylen; i++)
		{
			float freq[26];
			computeFreqs(freq, content, i, keylen);
			printf("Offset %i\n", i);
			for (size_t j = 0; j < 26; j++)
				printf("%f  %c\n", freq[j], 'A'+j);
			printf("\n");
		}
	}
	else
	{
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
	}

	free(content);
	return 0;
}
