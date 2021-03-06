#ifndef LIBOBJ_INTERNAL_H
#define LIBOBJ_INTERNAL_H

#include "vector.h"
#include <stdio.h>

typedef struct s_reader
{
	FILE *fp;
	t_vector *vertices;
	t_vector *indices;
	char *line;
	ssize_t line_len;
	size_t buf_len;
} t_reader;

bool convert_element_array_to_extra_vertices(t_reader *reader);
bool extract_vertex(t_reader *reader);
bool extract_face(t_reader *reader);
bool is_float(char const *const str);
bool is_uint(char const *const str);
char *skip_float(char *const str);
char *skip_uint(char *const str);

#endif
