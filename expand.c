#include <stdio.h>
#include <stdlib.h>

int expand(char *name);

int tab_width = 8;

int main(int argc, char **argv)
{
	while (++argv, --argc)
	{
		if (**argv == '-')
			switch (*++*argv)
			{
			case 't':
				tab_width = atoi(*argv + 1);
				break;
			default:
				break;
			}
		else
			expand(*argv);
	}

	return (0);
}

int expand(char *name)
{
	FILE *infile, *outfile;
	char *tname;
	int c;
	int count;
	char line[256], *buf;

	if (!(infile = fopen(name, "r")))
		return (-1);
	if (!(outfile = fopen(tname = tmpnam(NULL), "w")))
		return (-1);

	buf = line;
	count = 0;
	while ((c = fgetc(infile)) != EOF)
	{
		if (c == '\t')
		{
			int i;

			for (i = tab_width - count % tab_width; --i >= 0; )
			{
				*buf++ = ' ';
				count++;
			}
		}
		else if (c == '\n')
		{
			*buf++ = c;
			*buf = '\0';
			fputs(line, outfile);
			buf = line;
			count = 0;
		}
		else
		{
			*buf++ = c;
			count++;
		}
	}

	fclose(infile);
	fclose(outfile);

	remove(name);
	rename(tname, name);

	return (0);
}
