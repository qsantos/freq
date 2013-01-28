#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const float ref[26] =
{
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094,
	0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929,
	0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
	0.01974, 0.00074,
};

static char* readFile(FILE* f)
{
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* ret = malloc(size);
	fread(ret, 1, size, f);
	return ret;
}

static void computeFreqs(float freq[26], char* target, int length, int offset, int step)
{
	float total = 1;
	memset(freq, 0, 26*sizeof(float));
	for (char* c = target; offset < length; offset += step, c += step)
	{
		if ('a' <= *c && *c <= 'z')
			*c -= 'a' - 'A';

		if ('A' <= *c && *c <= 'Z')
		{
			freq[*c-'A']++;
			total++;
		}
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
			float diff = freq[j] - ref[(j+i)%26];
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
		"Usage: %s keylen [file]\n"
		"Performs a statical analysis on a file\n"
		"\n"
		"  keylen  assumed length of the key\n"
		"  file    input file (default: stdin)\n"
		,
		argv[0]
	);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		usage(argc, argv);
		exit(0);
	}

	FILE* f = argc < 3 ? stdin : fopen(argv[2], "r");
	if (!f)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[2]);
		exit(1);
	}
	char* content = readFile(f);
	size_t length = strlen(content);
	fclose(f);

	size_t keylen = atoi(argv[1]);
	char* key = malloc(keylen+1);
	key[keylen] = 0;

	for (size_t i = 0; i < keylen; i++)
	{
		float freq[26];
		computeFreqs(freq, content, length, i, keylen);
		char shift = bestShift(freq);
		key[i] = 'A' + shift;
	}

	if (keylen == 1)
		printf("Excepted shift: %i\n", key[0] - 'A');
	else
		printf("Excepted key: %s\n", key);

	free(key);
	free(content);
	return 0;
}
