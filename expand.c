/*
 * $Id: expand.c,v 1.3 2011/11/17 13:29:53 urs Exp $
 */

#include <stdio.h>
#include <stdlib.h>

static int expand(const char *name, int tab_width);

int main(int argc, char **argv)
{
	int tab_width = 8;

	while (++argv, --argc) {
		if (**argv == '-')
			switch (*++*argv) {
			case 't':
				tab_width = atoi(*argv + 1);
				break;
			default:
				break;
			}
		else
			expand(*argv, tab_width);
	}

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
