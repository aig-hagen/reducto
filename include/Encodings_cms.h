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
    static void add_clauses_nonempty_admissible_set(SATSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs);
    static void add_complement_clause(SATSolver *solver, activeArgs_t *activeArgs);
};

#endif