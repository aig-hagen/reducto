#ifndef INITIAL_SET_SOLVER_H
#define INITIAL_SET_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "omp.h"

extern "C" {
#include "Actives.h"
#include "AF.h"
#include "Decodings.h"
#include "Encodings.h"
#include "List.h"
#include "SatProblem.h"
}

#include "Encodings_cms.h"
#include "ExternalSolver.h"

using namespace CMSat;

class InitialSetSolver {
public:

	static uint8_t calculate_next_solution(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem);
	static uint8_t calculate_next_solution(argFramework_t *framework, activeArgs_t *activeArgs, SATSolver *solver, bool *isSolved);
};

#endif
