#ifndef PROC_DS_PR_H
#define PROC_DS_PR_H

#include <cstdint>
#include <list>
#include "omp.h"

#include "Encoding.h"
#include "Decoding.h"
#include "SatSolver.h"
#include "ScepticalCheck.h"

#include "../util/ArrayBitSet.h"

using namespace std;

/// <summary>
/// This class is responsible for processing solving steps for the DS-PR problem.
/// </summary>
class Proc_DS_PR {

public:
	static list<uint32_t> process_iteration(uint32_t query_argument, AF &framework, ArrayBitSet &activeArgs, bool &isRejected, bool &isTerminated,
		SatSolver &solver, bool &continue_calculation, bool is_first_iteration);
};
#endif