#ifndef DECODINGS_H
#define DECODINGS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "AF.h"
#include "Actives.h"
#include "SatProblem.h"

/// <summary>
/// Returns the argument which is associated to the specified label of an acceptance-variable in an SAT-encoding for the specified set of active arguments.
/// </summary>
/// <param name="encoding">Label of an acceptance variable used in a SAT-problem to encode a variable in the framework.</param>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
/// <returns>Argument, which has been encoded to the specified variable.</returns>
uint32_t get_argument(int64_t label_variable, activeArgs_t *activeArgs);

/// <summary>
/// Translates the solution of a specified SAT-problem to a set of argument, in the context of a specified set of active arguments.
/// </summary>
/// <param name="problem">The SAT-problem, which solution if to be tranlated.</param>
/// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
/// <returns>Set of arguments, which solve the specified SAT-problem.</returns>
nodeUInt32_t *get_set_from_problem(SATProblem_t *problem, activeArgs_t *activeArgs);

nodeUInt32_t *get_set_from_solution(arrayInt_t *solution, activeArgs_t *activeArgs);
#endif
