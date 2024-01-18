#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Array.h"

typedef struct matrix {
    uint32_t **content;
    uint32_t numberRows;
    uint32_t numberColumns;
} matrix_t;

/// <summary>
/// Creates a 2D-matrix of specified number of rows and columns and sets all entries to 0.
/// </summary>
/// <param name="numRows">The number rows.</param>
/// <param name="numColumns">The number columns.</param>
/// <returns>2D-matrix containing only 0's</returns>
matrix_t * create_matrix( uint32_t numRows,  uint32_t numColumns);
/// <summary>
/// Creates a 2D matrix of identical size as the specified matrix and copies all values of the 
/// specified matrix to the newly created matrix. 
/// </summary>
/// <param name="matrix">A 2D-matrix.</param>
/// <returns>A new 2D-matrix, containing all the values of the specified matrix.</returns>
matrix_t * copy_matrix(matrix_t *matrix);
/// <summary>
/// Deallocates the memory space of the specified matrix.
/// </summary>
/// <param name="matrix">A 2D-matrix.</param>
/// <returns>EXIT_SUCCESS iff deallocation was successful.</returns>
uint8_t free_matrix(matrix_t *matrix);
/// <summary>
/// Displays the specified matrix in the standard output-stream.
/// </summary>
/// <param name="matrix">Matrix to display.</param>
/// <returns><returns>EXIT_SUCCESS iff output was successful. EXIT_FAILURE otherwise.</returns></returns>
uint8_t print_matrix(matrix_t *matrix);
#endif
