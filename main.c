#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parse.h"
#include "gen.h"

char *read_file(const char *path, size_t *len)
{
	FILE *f;
	char *buf;

	f = fopen(path, "r");
	if (!f)
	{
		perror("fopen");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(*len);

	(void) fread(buf, 1, *len, f);
	fclose(f);

	return buf;
}

int main(int argc, char **argv)
{
	size_t len;
	char *src;
	struct ast *ast;

	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <input> [seed]\n", argv[0]);
		exit(1);
	}

	src = read_file(argv[1], &len);
	if (!src)
	{
		exit(1);
	}

	ast = parse(src, len);

	if (!ast)
	{
		fprintf(stderr, "error: %s\n", parse_get_error_string());

		size_t loc, lineno, begin, end, offset;
		loc = parse_get_error_location();
		get_line_info(src, len, loc, &lineno,
		              &begin, &end, &offset);

		fprintf(stderr, "%.*s\n", (int) (end - begin), &src[loc]);
		while (offset-- > 0) fputc(' ', stderr);
		fprintf(stderr, "^ \n");
		exit(1);
	}

	// ast_debug_dump(ast, stdout);
	// printf("\n");

	int seed = 0;
	if (argc > 2)
	{
		seed = atoi(argv[2]);
	}

	srand(seed);

	if (gen(stdout, ast))
	{
		fprintf(stderr, "error: %s\n",
		        gen_get_error_string());
		exit(1);
	}
	printf("\n");
}
