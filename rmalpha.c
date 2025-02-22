#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void
usage(void)
{
	(void)fprintf(stderr,
			"usage: rmalpha <input> <output>\n"
			"NOTE: DO NOT specify the same file for input and output!\n"
			"      The data in the input will be destroyed!\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	FILE *input, *output;
	int c, i;

	if (argc < 3)
		usage();

	if ((input = fopen(argv[1], "rb")) == NULL) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((output = fopen(argv[2], "wb")) == NULL) {
		perror(argv[2]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; (c = getc(input)) != EOF; i++)
		if ((i & 1) == 0)
			(void)putc(c, output);

	(void)fclose(output);
	(void)fclose(input);

	return 0;
}
