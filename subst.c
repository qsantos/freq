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

#define LANG english

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

float freq[26];
int compare(const void* a, const void* b)
{
	float fa = freq[(int)(*(char*)a)];
	float fb = freq[(int)(*(char*)b)];
	return fa > fb ? -1 :
	       fa < fb ?  1 :
	       0;
}

static void usage(const char* name)
{
	fprintf(stderr,
		"Usage: %s src [dst]\n"
		"Find the most probable substitution\n"
		, name);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		usage(argv[0]);

	FILE* src = fopen(argv[1], "r");
	if (!src)
	{
		fprintf(stderr, "Could not open file '%s' for reading\n", argv[1]);
		exit(1);
	}
	char* content = readFile(src);
	fclose(src);

	static const char LANG[26] = "ESAITNRULODMCPVQFGBHJXYZKW";
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
		S[(int)T[i]] = LANG[i];

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
