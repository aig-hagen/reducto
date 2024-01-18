#include "../include/Matrix.h"

static uint32_t *create_row(uint32_t length)
{
	uint32_t *elements = NULL;
	elements = (uint32_t *)calloc(length, sizeof * elements);
	if (elements == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	else {
		return elements;
	}
}

static uint32_t ** create_rows(uint32_t numRows) {
	uint32_t **matrix = NULL;
	matrix = (uint32_t**) calloc(numRows, sizeof *matrix );
	if (matrix == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	else {
		return matrix;
	}
}


matrix_t * create_matrix(uint32_t numRows, uint32_t numColumns) {
	matrix_t *matrix = NULL;
	matrix = (matrix_t*) malloc(sizeof *matrix );
	if (matrix == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	else {
		matrix->numberRows = numRows;
		matrix->numberColumns = numColumns;
		matrix->content = create_rows(numRows);
		for (uint32_t i = 0; i < numRows; i++) {
			matrix->content[i] = create_row(numColumns);
		}
		return matrix;
	}
}

matrix_t * copy_matrix(matrix_t *matrix)
{
	matrix_t *newMatrix = create_matrix(matrix->numberRows, matrix->numberColumns);
	for (uint32_t i = 0; i < newMatrix->numberRows; i++) {
		for (uint32_t j = 0; j < newMatrix->numberColumns; j++)
		{
			newMatrix->content[i][j] = matrix->content[i][j];
		}
	}
	return newMatrix;
}

uint8_t free_matrix(matrix_t *matrix) {
	for ( uint32_t i = 0; i < matrix->numberRows; i++) {
		free(matrix->content[i]);
	}
	free(matrix->content);
	free(matrix);
	return EXIT_SUCCESS;
}

uint8_t print_matrix(matrix_t *matrix) {
	//printf("\n------------------\nMatrix\nNumber of Rows: %d\nNumber of Collumns: %d\n", matrix->numberRows, matrix->numberColumns);
	for (uint32_t i = (uint32_t) 0; i < matrix->numberRows; i++) {
		for (uint32_t j = (uint32_t) 0; j < matrix->numberColumns; j++) {
			printf("%d\t", matrix->content[i][j]);
		}
		printf("\n");
	}
	//printf("------------------\n");
	return EXIT_SUCCESS;
}

