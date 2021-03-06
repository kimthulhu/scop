#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <libobj_internal.h>

//This function takes a char * and returns whether it is a float.
bool is_float(char const *const str)
{
	int i = 0;

	if (!str)
		return (false);

	//ensure str starts with a digit, or with '-' followed by a digit
	if (isdigit(str[i]))
		i++;
	else if (str[i] == '-' && isdigit(str[i + 1]))
		i += 2;
	else
		return (false);

	//skip until end of digits is found
	while (isdigit(str[i]))
		i++;

	//ensure digits are followed by '.' (the decimal point)
	if (str[i] != '.')
		return (false);
	i++;

	//ensure the decimal point is followed by the decimal digits
	if (!isdigit(str[i]))
		return (false);
	i++;

	//skip over decimal digits
	while (str[i] && isdigit(str[i]))
		i++;

	//ensure last digit is not followed by a letter
	if (isalpha(str[i]) || str[i] == '.')
		return (false);

	return (true);
}

//This function takes a char * and returns whether it is an int.
bool is_uint(char const *const str)
{
	int i = 0;

	if (!str)
		return (false);

	//ensure str starts with a digit
	if (!isdigit(str[i]))
		return (false);
	i++;

	//skip until end of digits is found
	while (isdigit(str[i]))
		i++;

	//ensure last digit is not followed by a '.' and a digit
	//(which would indicate that it's a float)
	if (str[i] == '.' && isdigit(str[i + 1]))
		return (false);

	//ensure last digit is not followed by a letter
	if (isalpha(str[i]))
		return (false);

	return (true);
}

//This function takes a char * starting with a float,
//and returns a char * to the element after the float.
//returns NULL if str is not a float
char *skip_float(char *const str)
{
	int i = 0;

	if (!str)
		return (NULL);

	//ensure str starts with a digit, or with '-' followed by a digit
	if (isdigit(str[i]))
		i++;
	else if (str[i] == '-' && isdigit(str[i + 1]))
		i += 2;
	else
		return (NULL);

	//skip until end of digits is found
	while (isdigit(str[i]))
		i++;

	//ensure digits are followed by '.' (the decimal point)
	if (str[i] != '.')
		return (NULL);
	i++;

	//ensure the decimal point is followed by the decimal digits
	if (!isdigit(str[i]))
		return (NULL);
	i++;

	//skip over decimal digits
	while (isdigit(str[i]))
		i++;

	if (isalpha(str[i]) || str[i] == '.')
		return NULL;

	return (str + i);
}

//This function takes a char * starting with an int,
//and returns a char * to the element after the int.
char *skip_uint(char *const str)
{
	int i = 0;

	if (!str)
		return (NULL);

	//ensure str starts with a digit
	if (!isdigit(str[i]))
		return (NULL);
	i++;

	//skip until end of digits is found
	while (str[i] && isdigit(str[i]))
		i++;

	//ensure last digit is not followed by a '.' and a digit
	//(which would indicate that it's a float)
	if (str[i] == '.' && isdigit(str[i + 1]))
		return (NULL);

	//ensure last digit is not followed by a letter
	if (isalpha(str[i]))
		return (NULL);

	return (&str[i]);
}

bool convert_element_array_to_extra_vertices(t_reader *reader)
{
	unsigned int const *const indices = reader->indices->vec;
	unsigned int const indices_len =
		reader->indices->used / sizeof(unsigned int);
	float const *const old = reader->vertices->vec;
	t_vector *new = vector_init(sizeof(float) * 3 * indices_len);

	if (!new)
		return (false);
	for (unsigned int i = 0; i < indices_len; i++)
	{
		if (!vector_append(
				&new,
				&old[indices[i] * 3],
				sizeof(float) * 3))
		{
			return (false);
		}
	}

	vector_delete(&reader->indices);
	vector_delete(&reader->vertices);
	reader->vertices = new;
	return (true);
}

void convert_hex_to_float_color(
	float dst[3],
	float const r,
	float const g,
	float const b)
{
	dst[0] = (1.0f / 255) * r;
	dst[1] = (1.0f / 255) * g;
	dst[2] = (1.0f / 255) * b;
}
