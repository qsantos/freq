#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s src [dst]\n"
		"Find the most probable substitution\n"
		,
		argv[0]
	);
}

float freq[26];
int compare(const void* a, const void* b)
{
	float fa = freq[(int)(*(char*)a)];
	float fb = freq[(int)(*(char*)b)];
	return fa > fb ? -1 :
	       fa < fb ?  1 :
	       0;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		usage(argc, argv);
		exit(0);
	}

	FILE* src = fopen(argv[1], "r");
	if (!src)
	{
		fprintf(stderr, "Could not open file '%s' for reading\n", argv[1]);
		exit(1);
	}
	char* content = readFile(src);
	fclose(src);

	static const char french[26] = "esaitnrulodmcpvqfgbhjxyzkw";
	static const char I[26] =
	{
			 0,  1,  2,  3,  4,  5,  6,  7,
			 8,  9, 10, 11, 12, 13, 14, 15,
			16, 17, 18, 19, 20, 21, 22, 23,
			24, 25,
	};

	computeFreqs(freq, content, 0, 1);

	char T[26];
	memcpy(T, I, 26);
	qsort(T, 26, 1, compare);

	char S[26];
	for (int i = 0; i < 26; i++)
		S[(int)T[i]] = french[i];

	FILE* dst = argc >= 3 ? fopen(argv[2], "w") : stdout;
	if (!dst)
	{
		fprintf(stderr, "Could not open file '%s' for writing\n", argv[2]);
	}
	for (char* c = content; *c; c++)
	{
		if ('A' <= *c && *c <= 'Z')
			fputc(S[*c - 'A'], dst);
		else
			fputc(*c, dst);
	}
	fclose(dst);

	free(content);
	return 0;
}
