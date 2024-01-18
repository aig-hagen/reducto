#ifndef ACTIVES_H
#define ACTIVES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Matrix.h"
#include "Array.h"

///<summary>
/// Data structure enregistering which argument is active 
/// and mapping them to the labels used for encoding.
///</summary>
typedef struct activeArgs {
    matrix_t *matrix;
    uint32_t numberActiveArguments;
    arrayInt_t *encodingToArgument;
} activeArgs_t;


/// <summary>
/// Creates a new empty shell of the struct, which is not yet initialized.
/// </summary>
/// <param name="numberArguments">Number of arguments in total.</param>
/// <param name="numberActiveArguments">Number of active arguments.</param>
/// <returns>Non-initialized struct of type activeArgs_t.</returns>
activeArgs_t* create_active_arguments(uint32_t numberArguments, uint32_t numberActiveArguments);
/// <summary>
/// Creates a copy of the specified active arguments, all elements of the original struct are copied by value, 
/// therefor not referencing the same components.
/// </summary>
/// <param name="original">The active arguments to copy.</param>
/// <returns>A newly created struct, containing the same values as the specified original.</returns>
activeArgs_t *copy_active_arguments(activeArgs_t *original);
/// <summary>
/// Modifies the specified matrix, so that the specified argument is deactivated.
/// </summary>
/// <param name="matrix">Matrix of all active arguments.</param>
/// <param name="argument">The argument to deactivate.</param>
/// <returns>EXIT_SUCCESS iff the operation was successful.</returns>
uint8_t deactivate_argument(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Frees the memory space, which was allocated for the specified active arguments.
/// </summary>
/// <param name="activeArguments">Struct, which memory space is to be freed.</param>
/// <returns>EXIT_SUCCESS iff the operation was successful. EXIT_FAILURE otherwise.</returns>
uint8_t free_activeArguments(activeArgs_t *activeArguments);
/// <summary>
/// Returns the first active argument. Returns 0 if no argument is active.
/// </summary>
/// <param name="activeArguments">Set of active arguments.</param>
/// <returns>The first in line of the active arguments or 0.</returns>
uint32_t get_first_active(activeArgs_t *activeArguments);
/// <summary>
/// Returns the next active argument.
/// </summary>
/// <param name="activeArguments">Matrix registering, which argument is active.</param>
/// <param name="argument">The currently pointed to active argument.</param>
/// <returns>The next active argument.</returns>
uint32_t get_next_active(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Returns the active argument, which was located before the one specified in the matrix specified.
/// </summary>
/// <param name="activeArguments">Matrix registering, which argument is active.</param>
/// <param name="argument">The currently pointed to active argument.</param>
/// <returns>The active argument, which was located before the argument specified.</returns>
uint32_t get_predecessor_active(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Checks if a specified argument is the last in line of the active arguments.
/// </summary>
/// <param name="activeArguments">Matrix registering, which argument is active.</param>
/// <param name="argument">The currently pointed to active argument.</param>
/// <returns>TRUE iff there is a another active argument after the specified one. FALSE otherwise.</returns>
bool has_next_active(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Checks if a specified argument is the first of the active arguments.
/// </summary>
/// <param name="activeArguments">Matrix registering, which argument is active.</param>
/// <param name="argument">The currently pointed to active argument.</param>
/// <returns>TRUE iff there is a another active argument positioned before the specified one in the list of active arguments. FALSE otherwise.</returns>
bool has_predecessor_active(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Creates a 2D-matrix, allowing direct access to and efficient iterating through the active arguments of 
/// a framework. <br> - </br> The structure is the following: 1st column points to the last active predecessor; 
/// 2nd column points to the next active successor. <br> - </br>
/// Initially all arguments are registered to be active.
/// </summary>
/// <param name="numberOfArguments">The number of arguments in total.</param>
/// <returns>2D matrix of all indicating active arguments.</returns>
activeArgs_t * initialize_actives(uint32_t numberOfArguments);
/// <summary>
/// Checks if a specified argument is registered to be active in a specified matrix.
/// </summary>
/// <param name="activeArguments">Matrix registering, which argument is active.</param>
/// <param name="argument">The argument to be checked.</param>
/// <returns>Returns EXIT_SUCCESS iff specified argument is active; Returns EXIT_FAILURE otherwise</returns>
bool is_active(activeArgs_t *activeArguments, uint32_t argument);
/// <summary>
/// Prints the specified active arguments to the standard output stream.
/// </summary>
/// <param name="activeArguments">Active arguments, which shall be printed.</param>
void print_active_arguments(activeArgs_t *activeArguments);

#endif
