#ifndef SAT_PROBLEM_H
#define SAT_PROBLEM_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "List.h"
#include "Array.h"

///<summary>
/// Model, which describes parts of the abstract argumentation framework as an SAT-problem,
/// in order to find extensions of interest.
///</summary>
typedef struct SATProblem {
    nodeInt64_t *assumptions;
    listInt64_t *clauses;
    uint32_t numberOfAssumptions;
    uint32_t numberOfClauses;
    uint32_t numberOfVariables;
    arrayInt_t *solution;
    bool isSolved;
} SATProblem_t;

extern SATProblem_t SATProblem;

/// <summary>
/// Adds an assumption to the specified SAT-problem.
/// </summary>
/// <param name="problem">The SAT-problem, to which the element is to add.</param>
/// <param name="literal">Number of the encoded argument to seen as an assumption.</param>
/// <returns>
/// EXIT_SUCCESS iff the operation was succesfull. 
/// EXIT_FAILURE otherwise.
/// </returns>
uint8_t add_assumption(SATProblem_t *problem, int64_t literal);
/// <summary>
/// Adds a clause to the specified SAT-problem.
/// </summary>
/// <param name="problem">The SAT-problem, to which the element is to add.</param>
/// <param name="clause">The list of encoded argument, that represents a clause.</param>
/// <returns>
/// EXIT_SUCCESS iff the operation was succesfull. 
/// EXIT_FAILURE otherwise.
/// </returns>
uint8_t add_clause(SATProblem_t *problem, nodeInt64_t *clause);
/// <summary>
/// Creates the data structure of a SAT-problem.
/// </summary>
/// <param name="number_variables">The number of variables in the SAT-Problem.
/// <returns>The empty data structure.</returns>
SATProblem_t* create_sat_problem(uint32_t number_variables);
/// <summary>
/// Deallocates the memory space of the specified problem and all it's elements.
/// </summary>
/// <param name="problem">The SAT-problem, which memory space is to be deallocated.</param>
/// <returns>
/// EXIT_SUCCESS iff the operation was succesfull. 
/// EXIT_FAILURE otherwise.
/// </returns>
uint8_t free_problem(SATProblem_t *problem);

#endif