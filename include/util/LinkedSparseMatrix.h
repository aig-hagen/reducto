#ifndef LINKED_SPARSE_MATRIX_H
#define LINKED_SPARSE_MATRIX_H
#include <stdbool.h>
#include "Matrix.h"

/// <summary>
/// Creates a matrix according the definition of the linked sparse matrices.
/// </summary>
/// <param name="row">Number of rows in the matrix.</param>
/// <param name="column">Number of columns in the matrix</param>
/// <returns>A new linked sparse matrix, which entries are all 0;</returns>
matrix_t *create_linked_sparse_matrix(uint32_t row, uint32_t column);
/// <summary>
/// Returns the first cell in the row which is not 0. Returns 0 if no cell is set.
/// </summary>
/// <param name="matrix">The linked sparse matrix-</param>
/// <param name="row">Index to access the specified row in the matrix.</param>
/// <returns>Index of the first cell in the row, which is not 0. Returns 0 if no cell is set.</returns>
uint32_t get_first_in_row(matrix_t *matrix, uint32_t row);
/// <summary>
/// Returns the index of the next cell which is not 0 in the specified row. Returns 0 if no next cell exists.
/// </summary>
/// <param name="matrix">The linked sparse matrix-</param>
/// <param name="row">Index to access the specified row in the matrix.</param>
/// <param name="currentCell">The currently pointed cell. For initial call use '0'.</param>
/// <returns>Index of the next cell which is != 0, or 0 if no next cell exists</returns>
uint32_t get_next_in_row(matrix_t *matrix, uint32_t row, uint32_t currentCell);
/// <summary>
/// Signals if there is a cell in the specified row further right than the one specified, which is set to non 0. 
/// </summary>
/// <param name="matrix">The linked sparse matrix-</param>
/// <param name="row">Index to access the specified row in the matrix.</param>
/// <param name="currentCell">The currently pointed cell. For initial call use '0'.</param>
/// <returns>TRUE iff there is a next cell. FALSE otherwise</returns>
bool has_next_in_row(matrix_t *matrix, uint32_t row, uint32_t currentCell);
/// <summary>
/// Methods checks if a cell in a specified row and specified collumn is set or not.
/// </summary>
/// <param name="matrix">A linked sparse matrix.</param>
/// <param name="row">Index of a row.</param>
/// <param name="collumn">Index of a column.</param>
/// <returns>TRUE iff the cell is set. FALSE otherwise.</returns>
bool is_cell_filled(matrix_t *matrix, uint32_t row, uint32_t column);
/// <summary>
/// Sets the flag in a cell of a specified matrix.
/// </summary>
/// <param name="matrix">A linked sparse matrix.</param>
/// <param name="row">Index of a row.</param>
/// <param name="collumn">Index of a column.</param>
/// <returns>EXIT_SUCCESS the operation was successful. EXIT_FAILURE if there was an error.</returns>
uint8_t set_cell(matrix_t *matrix, uint32_t row, uint32_t column);


#endif