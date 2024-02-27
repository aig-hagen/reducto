#ifndef ENCODINGS_CMS_H
#define ENCODINGS_CMS_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
#include "Encodings.h"
#include "SatProblem.h"
}

#include <cryptominisat.h>
#include <vector>

using namespace CMSat;

/*
Class to encode a problem to be solved with a cryptominisat-solver
*/
class Encodings_CMS {
public:
    /// <summary>
    /// Adds all clauses necessary to encode an initial set.
    /// </summary>
    /// <param name="solver">The SATSolver, to which the clauses will be added.</param>
    /// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_clauses_nonempty_admissible_set(SATSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs);
    /// <summary>
    /// Adds a complement clause to a found solution to a specified SATSolver. This clause is necessary to provoke the solver to
    /// find another solution to the problem.
    /// </summary>
    /// <param name="solver">The SATSolver, to which a solution was found</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_complement_clause(SATSolver *solver, activeArgs_t *activeArgs);
};

#endif