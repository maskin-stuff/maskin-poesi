#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gen.h"

char s_error_buf[512] = { 0 };
struct ast *s_root;
int s_concat;

static int gen_internal(FILE *f, struct ast *ast)
{
	struct ast *sub;
	size_t depth, choice;

	switch (ast->tag)
	{
	case AT_SPEC:
	case AT_CHOICE:
		depth = ast_count_choices(ast);
		choice = rand() % depth;
		sub = ast_get_rhs_depth(ast, choice);
		if (sub->tag == AT_CHOICE)
		{
			sub = sub->lhs;
		}
		return gen_internal(f, sub);
	case AT_WORD:
		if (strcmp(ast->str, "\\") == 0)
		{
			fprintf(f, "\n");
			return 0;
		}
		if (strcmp(ast->str, "+") == 0)
		{
			s_concat = 1;
			return 0;
		}
		if (s_concat)
		{
			fprintf(f, "%s", ast->str);
			s_concat = 0;
			return 0;
		}
		fprintf(f, " %s", ast->str);
		return 0;
	case AT_LIST:
		if (gen_internal(f, ast->lhs))
			return -1;
		return gen_internal(f, ast->rhs);
	case AT_CONSTR:
		sub = ast_get_constr(s_root, ast->str);
		if (!sub)
		{
			snprintf(s_error_buf, sizeof(s_error_buf),
			         "Unknown construct \"%s\"", ast->str);
			return -1;
		}
		return gen_internal(f, sub);
	default:
		assert(0 && "Called gen on wrong ast type");
	}
}

int gen(FILE *f, struct ast *ast)
{
	struct ast *sub;
	s_root = ast;
	s_concat = 1;
	sub = ast_get_constr(s_root, BASE_NAME);
	if (!sub)
	{
		snprintf(s_error_buf, sizeof(s_error_buf),
		         "Missing \"" BASE_NAME "\" construct");
		return -1;
	}
	return gen_internal(f, sub);
}

const char *gen_get_error_string(void)
{
	return s_error_buf;
}
