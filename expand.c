/*
 * $Id: expand.c,v 1.8 2011/11/17 13:30:43 urs Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void usage(const char *name)
{
	fprintf(stderr, "Usage: %s [-t width] file...\n", name);
}

static int expand(const char *name, int tab_width);

int main(int argc, char **argv)
{
	int tab_width = 8;
	int errflg = 0;
	int opt, i;

	while ((opt = getopt(argc, argv, "t:")) != -1) {
		switch (opt) {
		case 't':
			tab_width = atoi(optarg);
			break;
		default:
			errflg = 1;
			break;
		}
	}
	if (errflg) {
		usage(argv[0]);
		exit(1);
	}

	if (optind == argc)
		expand("-", tab_width);
	else
		for (i = optind; i < argc; i++)
			expand(argv[i], tab_width);

	return 0;
}

static int expand(const char *name, int tab_width)
{
	FILE *file;
	int c;
	int count;

	if (strcmp(name, "-") == 0)
		file = stdin;
	else if (!(file = fopen(name, "r"))) {
		perror(name);
		return -1;
	}

	count = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\t') {
			int i;

			for (i = tab_width - count % tab_width; --i >= 0; ) {
				putchar(' ');
				count++;
			}
		} else if (c == '\n') {
			putchar(c);
			count = 0;
		} else {
			putchar(c);
			count++;
		}
	}

	if (file != stdin)
		fclose(file);

	return 0;
}
