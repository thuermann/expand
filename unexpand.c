/*
 * $Id: unexpand.c,v 1.7 2011/11/17 13:30:33 urs Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void usage(const char *name)
{
	fprintf(stderr, "Usage: %s [-t width] file...\n", name);
}

static int unexpand(const char *name, int tab_width);

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
		unexpand("-", tab_width);
	else
		for (i = optind; i < argc; i++)
			unexpand(argv[i], tab_width);

	return 0;
}

static int unexpand(const char *name, int tab_width)
{
	FILE *file;
	int c;
	int count;
	int spc_cnt, spc_flag;
	char line[256], *buf;

	if (strcmp(name, "-") == 0)
		file = stdin;
	else if (!(file = fopen(name, "r"))) {
		perror(name);
		return -1;
	}

	buf = line;
	count = spc_cnt = 0;
	spc_flag = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == ' ' && !spc_flag) {
			count++;
			spc_cnt++;
			if (count % tab_width == 0) {
				if (spc_cnt > 1)
					*buf++ = '\t';
				else
					*buf++ = ' ';
				spc_cnt = 0;
			}
		} else if (c == '\t') {
			*buf++ = c;
			count += tab_width - count % tab_width;
			spc_cnt = 0;
		} else if (c == '\n') {
			while (spc_cnt > 0) {
				*buf++ = ' ';
				spc_cnt--;
			}
			*buf = '\0';
			puts(line);
			buf = line;
			count = 0;
			spc_flag = 0;
		} else {
			while (spc_cnt > 0) {
				*buf++ = ' ';
				spc_cnt--;
			}
			*buf++ = c;
			count++;
			spc_flag = 1;
		}
	}

	if (file != stdin)
		fclose(file);

	return 0;
}
