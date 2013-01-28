#include <stdlib.h>
#include <stdio.h>

static const float ref[26] =
{
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094,
	0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929,
	0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
	0.01974, 0.00074,
};

static void usage(int argc, char** argv)
{
	(void) argc;

	fprintf(stderr,
		"Usage: %s file\n"
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

	FILE* f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "Could not open file '%s'\n", argv[1]);
		exit(1);
	}

	float freq[26];
	float total = 1;
	while (1)
	{
		char c = fgetc(f);
		if (feof(f))
			break;

		if ('a' <= c && c <= 'z')
			c -= 'a' - 'A';
		if ('A' <= c && c <= 'Z')
		{
			freq[c-'A']++;
			total++;
		}
	}
	fclose(f);

	for (int i = 0; i < 26; i++)
		freq[i] /= total;

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

	printf("Excepted shift: %i\n", best_shift);

	return 0;
}
