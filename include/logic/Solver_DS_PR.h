#ifndef SOLVER_DS_PR_H
#define SOLVER_DS_PR_H

#include <iostream>
#include <cstdint>
#include <string_view>
#include <functional>
#include <vector>
#include <queue>

extern "C" {
	#include "../util/MemoryWatchDog.h"
}

#include "AF.h"
#include "Decoding.h"
#include "Encoding.h"
#include "Enums.h"
#include "ExtensionPrioritised.h"
#include "HeuristicDummy.h"
#include "Heuristic1.h"
#include "Heuristic2.h"
#include "Heuristic3.h"
#include "Heuristic4.h"
#include "Heuristic5.h"
#include "IPrioHeuristic.h"
#include "PreProcessor_GR.h"
#include "PreProcessor_GR_parallel.h"
#include "PriorityStackManager.h"
#include "Processor_DS_PR.h"
#include "Reduct.h"
#include "SatSolver.h"
#include "SatSolver_cadical.h"
#include "ScepticalCheck.h"
#include "ToolsSolver.h"
#include "omp.h"

#include "../util/Printer.h"
#include "../util/ToolList.h"
#include "../util/Tools_omp.h"

using namespace std;

/// <summary>
/// This class is responsible for solving a problem of sceptical acceptance of an query-argument
/// in extensions of the preferred semantics.
/// </summary>
class Solver_DS_PR {
public:

	/// <summary>
	/// Checks if a specified argument is sceptically accepted by all preferred extensions.
	/// </summary>
	/// <param name="query_argument">The argument, which could be sceptical accepted or not.</param>
	/// <param name="framework">The abstract argumentation framework, specifying the underlying attack relations between the arguments.</param>
	/// <param name="proof_extension"> Extension proving, that the argument cannot be sceptically accepted.</param>
	/// <param name="numCores"> Number of cores requested to be used to solve the problem. Actual number can be lower depending on the OS scheduler.</param>
	/// <returns>TRUE iff the argument is sceptically accepted. FALSE otherwise.</returns>
	static bool solve(uint32_t query_argument, AF &framework, list<uint32_t> &proof_extension, uint16_t numCores);
};

#endif

static void process_sat_solution(bool has_found_set, std::__cxx11::list<uint32_t> &extension_build, std::__cxx11::list<uint32_t> &calculated_set, bool &is_rejected, std::__cxx11::list<uint32_t> &certificate_extension, PriorityStackManager &prio_queue, uint32_t query_argument, AF &framework, IPrioHeuristic &heuristic);

static void process_state(pre_proc_result result_preProcessor, bool &is_rejected, bool &is_terminated, std::__cxx11::list<uint32_t> &certificate_extension, std::__cxx11::list<uint32_t> &extension_build, AF &framework, ArrayBitSet &reduct, uint32_t query_argument, PriorityStackManager &prio_queue, IPrioHeuristic &heuristic);

static void search_for_rejecting_sets_in_reduct(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_terminated, std::__cxx11::list<uint32_t> &extension, std::__cxx11::list<uint32_t> &certificate_extension, IPrioHeuristic *heuristic, PriorityStackManager &prio_stack);

static void search_for_rejecting_sets_in_origin_state(AF &framework, ArrayBitSet &active_args, uint32_t query_argument, bool &is_rejected, bool &is_terminated, std::__cxx11::list<uint32_t> &certificate_extension, PriorityStackManager &prio_stack, IPrioHeuristic *heuristic, bool &is_finished);

static void process_sat_solution(bool has_found_solution, bool &is_rejected, std::__cxx11::list<uint32_t> &extension_build, std::__cxx11::list<uint32_t> &calculated_set, std::__cxx11::list<uint32_t> &certificate_extension);
