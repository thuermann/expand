/*
 * $Id: expand.c,v 1.4 2011/11/17 13:30:03 urs Exp $
 */

#include <stdio.h>
#include <stdlib.h>
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

	for (i = optind; i < argc; i++)
		expand(argv[i], tab_width);

	return 0;
}

static int expand(const char *name, int tab_width)
{
	FILE *infile, *outfile;
	char *tname;
	int c;
	int count;
	char line[256], *buf;

	if (!(infile = fopen(name, "r")))
		return -1;
	if (!(outfile = fopen(tname = tmpnam(NULL), "w")))
		return -1;

	buf = line;
	count = 0;
	while ((c = fgetc(infile)) != EOF) {
		if (c == '\t') {
			int i;

			for (i = tab_width - count % tab_width; --i >= 0; ) {
				*buf++ = ' ';
				count++;
			}
		} else if (c == '\n') {
			*buf++ = c;
			*buf = '\0';
			fputs(line, outfile);
			buf = line;
			count = 0;
		} else {
			*buf++ = c;
			count++;
		}
	}

	fclose(infile);
	fclose(outfile);

	remove(name);
	rename(tname, name);

	return 0;
}
