#ifndef INITIAL_SET_SOLVER_H
#define INITIAL_SET_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
#include "Actives.h"
#include "AF.h"
#include "Decodings.h"
#include "Encodings.h"
#include "List.h"
#include "SatProblem.h"
}

#include "ExternalSolver.h"

class InitialSetSolver {
public:
	
	static uint8_t calculate_next_initial_set(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem, nodeUInt32_t *out_initial_set);

	static uint8_t calculate_next_solution(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem, arrayInt_t *out_solution);
};

#endif
