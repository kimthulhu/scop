#include "main.h"

typedef struct s_mat4
{
	float mat4[16];
} t_mat4;

//Only for vectors of 3,
//make sure that dst is not the same memory as src1 or src2!
static void cross_product_vectors(
	float *dst,
	float *src1,
	float *src2)
{
	dst[0] = src1[1] * src2[2] - src1[2] * src2[1];
	dst[1] = src1[2] * src2[0] - src1[0] * src2[2];
	dst[2] = src1[0] * src2[1] - src1[1] * src2[0];
}

static void normalize_vector(
	float *const dst,
	float const *const src,
	size_t const len)
{
	float mod = 0.0f;
	double magnitude;

	for (size_t i = 0; i < len; ++i)
		mod += src[i] * src[i];
	magnitude = sqrt(mod);
	if (magnitude == 0)
	{
		fprintf(stderr, "The input vector is a zero vector");
		return;
	}
	for (size_t i = 0; i < len; ++i)
		dst[i] = src[i] / magnitude;
}

static t_mat4 get_identity_matrix(void)
{
	t_mat4 result;

	memset(result.mat4, 0, 16 * sizeof(float));
	result.mat4[0] += 1.0f;
	result.mat4[5] += 1.0f;
	result.mat4[10] += 1.0f;
	result.mat4[15] += 1.0f;
	return result;
}

static t_mat4 multiply_matrices(
	t_mat4 const src1,
	t_mat4 const src2)
{
	t_mat4 result;

	memset(&result, 0, sizeof(t_mat4));
	for (unsigned int y = 0; y < 4; y++)
		for (unsigned int x = 0; x < 4; x++)
			for (unsigned int i = 0; i < 4; i++)
			{
				result.mat4[y * 4 + x] +=
					src1.mat4[y * 4 + i] * src2.mat4[i * 4 + x];
			}
	return result;
}

static t_mat4 get_translation_matrix(
	float const x_translation,
	float const y_translation,
	float const z_translation)
{
	t_mat4 result = get_identity_matrix();

	result.mat4[12] += x_translation;
	result.mat4[13] += y_translation;
	result.mat4[14] += z_translation;
	return result;
}

static t_mat4 get_rotation_matrix(
	float const x_degrees,
	float const y_degrees,
	float const z_degrees)
{
	t_mat4 x_matrix = get_identity_matrix();
	t_mat4 y_matrix = get_identity_matrix();
	t_mat4 z_matrix = get_identity_matrix();

	//Set up x-axis rotation matrix
	x_matrix.mat4[5] = cos((double)x_degrees);
	x_matrix.mat4[6] = sin((double)x_degrees);
	x_matrix.mat4[9] = -sin((double)x_degrees);
	x_matrix.mat4[10] = cos((double)x_degrees);
	//Set up y-axis rotation matrix
	y_matrix.mat4[0] = cos((double)y_degrees);
	y_matrix.mat4[2] = -sin((double)y_degrees);
	y_matrix.mat4[8] = sin((double)y_degrees);
	y_matrix.mat4[10] = cos((double)y_degrees);
	//Set up z-axis rotation matrix
	z_matrix.mat4[0] = cos((double)z_degrees);
	z_matrix.mat4[1] = sin((double)z_degrees);
	z_matrix.mat4[4] = -sin((double)z_degrees);
	z_matrix.mat4[5] = cos((double)z_degrees);

	//Combine the matrices (crude method, may lead to gimbal lock)
	return multiply_matrices(multiply_matrices(x_matrix, y_matrix), z_matrix);
}

static t_mat4 get_scaling_matrix(
	float const x_scaling,
	float const y_scaling,
	float const z_scaling)
{
	t_mat4 result = get_identity_matrix();

	result.mat4[0] += x_scaling;
	result.mat4[5] += y_scaling;
	result.mat4[10] += z_scaling;
	return result;
}

static t_mat4 get_projection_matrix(
	const float fov,
	const float aspect,
	float near,
	float far)
{
	t_mat4 result = get_identity_matrix();
	float f = (float)tan(fov / 2.0f);

	result.mat4[0] = 1.0f / (f * aspect);
	result.mat4[5] = 1.0f / f;
	result.mat4[10] = -(far + near) / (far - near);
	result.mat4[11] = -1.0f;
	result.mat4[14] = -(2.0f * far * near) / (far - near);
	return (result);
}

void send_model_matrix(t_app *app)
{
	t_mat4 transformation_matrix = get_identity_matrix();

	//Add scaling to the transformation matrix
	transformation_matrix = multiply_matrices(
		transformation_matrix,
		get_scaling_matrix(
			app->orientation.scaling_x,
			app->orientation.scaling_y,
			app->orientation.scaling_z));

	//Add rotation to the transformation matrix
	transformation_matrix = multiply_matrices(
		transformation_matrix,
		get_rotation_matrix(
			app->orientation.rotation_x,
			app->orientation.rotation_y,
			app->orientation.rotation_z));

	//Add translation to the transformation matrix
	transformation_matrix = multiply_matrices(
		transformation_matrix,
		get_translation_matrix(
			app->orientation.translation_x,
			app->orientation.translation_y,
			app->orientation.translation_z));

	//Send the transformation matrix to the shader program
	glUseProgram(app->shader_program);
	glUniformMatrix4fv(
		glGetUniformLocation(app->shader_program, "modelmatrix"),
		1,
		GL_FALSE,
		transformation_matrix.mat4);
}

void send_projection_matrix(t_app *app)
{
	t_mat4 projection_matrix = get_identity_matrix();

	//Add projection to the matrix
	projection_matrix = multiply_matrices(
		projection_matrix,
		get_projection_matrix(
			DEFAULT_FOV,
			DEFAULT_SCREEN_WIDTH / DEFAULT_SCREEN_HEIGHT,
			0.1f,
			100.0f));

	//Send the projection matrix to the shader program
	glUseProgram(app->shader_program);
	glUniformMatrix4fv(
		glGetUniformLocation(app->shader_program, "projectionmatrix"),
		1,
		GL_FALSE,
		projection_matrix.mat4);
}

void send_view_matrix(t_app *app)
{
	t_mat4 view_matrix = get_identity_matrix();
	float cam_direction[3];

	app->camera.cam_pos[0] = 0.0f;
	app->camera.cam_pos[1] = 0.0f;
	app->camera.cam_pos[2] = 3.0f;

	app->camera.cam_target[0] = 0.0f;
	app->camera.cam_target[1] = 0.0f;
	app->camera.cam_target[2] = 0.0f;

	cam_direction[0] = app->camera.cam_pos[0] - app->camera.cam_target[0];
	cam_direction[1] = app->camera.cam_pos[1] - app->camera.cam_target[1];
	cam_direction[2] = app->camera.cam_pos[2] - app->camera.cam_target[2];
	normalize_vector(&cam_direction, &cam_direction, 3);

	app->camera.cam_up[0] = 0.0f;
	app->camera.cam_up[1] = 1.0f;
	app->camera.cam_up[2] = 0.0f;

	cross_product_vectors(
		app->camera.cam_right,
		app->camera.cam_up,
		&cam_direction);
	normalize_vector(&app->camera.cam_right, &app->camera.cam_right, 3);

	//Add world space transformation to matrix
	view_matrix = multiply_matrices(
		view_matrix,
		get_translation_matrix(
			-app->obj_pos[0],
			-app->obj_pos[1],
			-app->obj_pos[2]));
}
