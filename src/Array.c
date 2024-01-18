//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "../include/Array.h"

static uint32_t* create_elements(uint32_t length)
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

arrayInt_t *create_array(uint32_t length)
{
	arrayInt_t *output = NULL;
	output = (arrayInt_t *)malloc(sizeof * output);
	if (output == NULL)
	{
		printf("Memory allocation failed\n");
		exit(1);
	}
	else {
		output->elements = create_elements(length);
		output->length = length;
		return output;
	}
}

arrayInt_t* copy_array(arrayInt_t *original)
{
	arrayInt_t *copy = create_array(original->length);
	for (uint32_t i = (uint32_t) 0; i < original->length; i++)
	{
		copy->elements[i] = original->elements[i];
	}

	return copy;
}

uint8_t free_array(arrayInt_t *array)
{
	free(array->elements);
	free(array);

	return EXIT_SUCCESS;
}

void print_array(arrayInt_t *array)
{
	//printf("\n==\n");
	for (uint32_t i = (uint32_t) 0; i < array->length; i++)
	{
		printf("[%d]", array->elements[i]);
	}
	//printf("\n");
	//printf("==\n");
}
