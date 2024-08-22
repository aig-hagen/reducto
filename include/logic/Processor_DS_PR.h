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

class Proc_DS_PR {

public:
	static list<uint32_t> process_iteration(uint32_t query_argument, AF &framework, ArrayBitSet &activeArgs, bool &isRejected, bool &isTerminated,
		SatSolver &solver, bool &continue_calculation);
}

#endif

void set_is_rejected(bool &is_rejected, bool &is_terminated);
