#include <stdio.h>
#include <stdlib.h>

int unexpand(char *name);

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
			unexpand(*argv);
	}

	return (0);
}

int unexpand(char *name)
{
	FILE *infile, *outfile;
	char *tname;
	int c;
	int count;
	int spc_cnt, spc_flag;
	char line[256], *buf;

	if (!(infile = fopen(name, "r")))
		return (-1);
	if (!(outfile = fopen(tname = tmpnam(NULL), "w")))
		return (-1);

	buf = line;
	count = spc_cnt = 0;
	spc_flag = 0;
	while ((c = fgetc(infile)) != EOF)
	{
		if (c == ' ' && !spc_flag)
		{
			count++;
			spc_cnt++;
			if (count % tab_width == 0)
			{
				if (spc_cnt > 1)
					*buf++ = '\t';
				else
					*buf++ = ' ';
				spc_cnt = 0;
			}
		}
		else if (c == '\t')
		{
			*buf++ = c;
			count += tab_width - count % tab_width;
			spc_cnt = 0;
		}
		else if (c == '\n')
		{
			while (spc_cnt > 0)
			{
				*buf++ = ' ';
				spc_cnt--;
			}
			*buf++ = c;
			*buf = '\0';
			fputs(line, outfile);
			buf = line;
			count = 0;
			spc_flag = 0;
		}
		else
		{
			while (spc_cnt > 0)
			{
				*buf++ = ' ';
				spc_cnt--;
			}
			*buf++ = c;
			count++;
			spc_flag = 1;
		}
	}

	fclose(infile);
	fclose(outfile);

	remove(name);
	rename(tname, name);

	return (0);
}
