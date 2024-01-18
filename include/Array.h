#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arrayInt {
    uint32_t length;
    uint32_t *elements;
} arrayInt_t;

/// <summary>
/// Creates a new 1D array for uint32_t of the specified length, containing only 0's.
/// </summary>
/// <param name="length">The length of the array to create</param>
/// <returns>1D Array of uint32_t of specified length.</returns>
arrayInt_t *create_array(uint32_t length);
/// <summary>
/// Copies a specified array, creating a new array containing 
/// elements of the same values as the original.
/// </summary>
/// <param name="original">The 1D array of uint32_t values to copy by value.</param>
/// <returns>Newly created 1D array, containing same values as the original.</returns>
arrayInt_t *copy_array(arrayInt_t *original);
/// <summary>
/// Deallocates the space, which was reserved for the specified array.
/// </summary>
/// <param name="array">Array whose memory shall be released.</param>
/// <returns>EXIT_SUCCESS iff the operation was successfull. EXIT_FAILURE otherwise.</returns>
uint8_t free_array(arrayInt_t *array);
/// <summary>
/// Prints the specified 1D array.
/// </summary>
/// <param name="array">1D array of uint32_t's to print.</param>
void print_array(arrayInt_t *array);

#endif
