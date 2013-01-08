#include <stdio.h>

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
	(void) argc;
	(void) argv;

	return 0;
}
