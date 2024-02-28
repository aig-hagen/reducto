#ifndef ENCODINGS_H
#define ENCODINGS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "AF.h"
#include "Actives.h"
#include "SatProblem.h"

/// <summary>
/// Returns the label of the acceptance variable of the specified argument, used
/// for the SAT encoding.
/// </summary>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
/// <param name="argument">Argument, for whom the acceptance variable shall be calculated.</param>
/// <returns>The label of the acceptance variable of the specified argument, used in the SAT encoding.</returns>
int64_t get_accepted_variable(activeArgs_t *activeArgs, uint32_t argument);
/// <summary>
/// Returns the label of the rejection variable of the specified argument, used
/// for the SAT encoding.
/// </summary>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
/// <param name="argument">Argument, for whom the rejection variable shall be calculated.</param>
/// <returns>The label of the rejection variable of the specified argument, used in the SAT encoding.</returns>
int64_t get_rejected_variable(activeArgs_t *activeArgs, uint32_t argument);
/// <summary>
/// Adds all clauses necessary to encode an initial set.
/// </summary>
/// <param name="solver">The SAT-problem, to which the clauses will be added.</param>
/// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
void add_clauses_nonempty_admissible_set(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs);
/// <summary>
/// Adds all clauses necessary to encode a complete extension.
/// </summary>
/// <param name="solver">The SAT-problem, to which the clauses will be added.</param>
/// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
void add_clauses_complete_extension(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs);
/// <summary>
/// Adds a complement clause to a found solution to a specified SAT-problem. This clause is necessary to provoke the solver to
/// find another solution to the problem.
/// </summary>
/// <param name="problem">The SAT-problem, to which a solution was found</param>
void add_complement_clause(SATProblem_t *problem);
#endif
