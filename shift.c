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
		,
		argv[0]
	);
}

int main(int argc, char** argv)
{
	char encrypt = 1;
	int curarg = 1;
	char* option = argv[curarg];
	if (strcmp(option, "--help") == 0 || strcmp(option, "-h") == 0)
	{
		usage(argc, argv);
		exit(0);
		curarg++;
	}
	else if (strcmp(option, "--reverse") == 0 || strcmp(option, "-r") == 0)
	{
		encrypt = 0;
		curarg++;
	}

	if (curarg == argc)
	{
		fprintf(stderr, "I need a key\n\n");
		usage(argc, argv);
		exit(0);
	}

	char* key = argv[curarg++];

	FILE* src = curarg < argc ? fopen(argv[curarg++], "r") : stdin;
	if (!src)
	{
		fprintf(stderr, "Could not open file '%s' for reading\n", argv[curarg]);
		exit(1);
	}

	FILE* dst = curarg < argc ? fopen(argv[curarg++], "w") : stdout;
	if (!dst)
	{
		fprintf(stderr, "Could not open file '%s' for writing\n", argv[curarg]);
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
