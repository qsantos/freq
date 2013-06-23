/*\
 *  This is an awesome programm simulating awesome battles of awesome robot tanks
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

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s [OPTIONS] key [src [dst]]\n"
		"Encrypt a file with the Caesar or the Vigenère cipher\n"
		"\n"
		"  key     integer for the Caesar shift value\n"
		"          string for the Vigenère key\n"
		"  src     the source file (default: stdin)\n"
		"  dst     the destination file (default: stdout)\n"
		"\n"
		"OPTIONS:\n"
		"  --reverse, -r  uses the key to decrypt\n"
		"  --version, -v  prints information about this program\n"
		"  --help,    -h  prints this page of help\n"
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

	char encrypt = 1;
	int curarg = 1;
	char* option = argv[curarg];
	if (strcmp(option, "--version") == 0 || strcmp(option, "-v") == 0)
	{
		fprintf(stderr, "shift 0.1\n");
		fprintf(stderr, "Compiled on %s the %s\n", __DATE__, __TIME__);
		exit(0);
	}
	else if (strcmp(option, "--help") == 0 || strcmp(option, "-h") == 0)
	{
		usage(argc, argv);
		exit(0);
	}
	else if (strcmp(option, "--reverse") == 0 || strcmp(option, "-r") == 0)
	{
		encrypt = 0;
		curarg++;
	}

	if (curarg >= argc)
	{
		fprintf(stderr, "I need a key\n\n");
		usage(argc, argv);
		exit(0);
	}

	char* key = argv[curarg++];

	FILE* src = curarg < argc ? fopen(argv[curarg++], "r") : stdin;
	if (!src)
	{
		fprintf(stderr, "Could not open file '%s' for reading\n", argv[curarg-1]);
		exit(1);
	}

	FILE* dst = curarg < argc ? fopen(argv[curarg++], "w") : stdout;
	if (!dst)
	{
		fprintf(stderr, "Could not open file '%s' for writing\n", argv[curarg-1]);
		exit(1);
	}

	int decimal = atoi(key);
	int len;
	if (decimal)
	{
		len = 1;
		if (encrypt == 0)
			decimal = 26 - decimal;

		decimal %= 26;
		if (decimal < 0)
			decimal += 26;
		key[0] = decimal;
	}
	else
	{
		len = 0;
		for (char* c = key; *c; c++, len++)
		{
			if ('A' <= *c && *c <= 'Z')
				*c = encrypt ? *c - 'A' : (26 - *c + 'A');
			else if ('a' <= *c && *c <= 'z')
				*c = encrypt ? *c - 'a' : (26 - *c + 'A');
			else
			{
				fprintf(stderr, "Sorry, I could not understand the key\n");
				exit(1);
			}
		}
	}
	int s = 0;
	while (1)
	{
		char c = fgetc(src);
		if (feof(src))
			break;
		if ('A' <= c && c <= 'Z')
			fputc('A' + ((c+key[s++] - 'A')%26), dst);
		else if ('a' <= c && c <= 'z')
			fputc('a' + ((c+key[s++] - 'a')%26), dst);
		else
			fputc(c, dst);
		if (s >= len)
			s = 0;
	}

	fclose(dst);
	fclose(src);
	return 0;
}
