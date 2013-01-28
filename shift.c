#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s key [src [dst]]\n"
		"Encrypt a file with the Caesar or the Vigenère cipher\n"
		"\n"
		"  key     integer for the Caesar shift value\n"
		"          string for the Vigenère key\n"
		"  src     the source file (default: stdin)\n"
		"  dst     the destination file (default: stdout)\n"
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

	FILE* src = argc < 3 ? stdin : fopen(argv[2], "r");
	if (!src)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[2]);
		exit(1);
	}

	FILE* dst = argc < 4 ? stdout : fopen(argv[3], "w");
	if (!dst)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[3]);
		exit(1);
	}

	char* shift = argv[1];
	int decimal = atoi(argv[1]);
	int len;
	if (decimal)
	{
		len = 1;
		shift[0] = decimal;
	}
	else
	{
		len = 0;
		for (char* c = shift; *c; c++, len++)
		{
			if ('A' <= *c && *c <= 'Z')
				*c -= 'A';
			else if ('a' <= *c && *c <= 'z')
				*c -= 'a';
			else
			{
				fprintf(stderr, "Sorry, I could not understand the shift\n");
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
			fputc('A' + ((c+shift[s] - 'A')%26), dst);
		else if ('a' <= c && c <= 'z')
			fputc('a' + ((c+shift[s] - 'a')%26), dst);
		else
			fputc(c, dst);
		if (++s >= len)
			s = 0;
	}

	fclose(dst);
	fclose(src);
	return 0;
}
