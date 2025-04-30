#ifndef ENCODING_H
#define ENCODING_H

#include <cstdint>
#include <vector>
#include <unordered_set>

#include "AF.h"

#include "../util/ArrayBitSet.h"
#include "../util/Printer.h"


#if defined(SAT_CMSAT)
#include "SatSolverCrypto.h"
typedef SatSolverCrypto SAT_Solver;
#elif defined(SAT_CADICAL)
#include "SatSolverCadical.h"
typedef SatSolverCadical SAT_Solver;
#elif defined(SAT_GLUCOSE)
#include "SatSolverGlucose.h"
typedef SatSolverGlucose SAT_Solver;
#else
#error "No SAT Solver defined"
#endif

using namespace std;

/// <summary>
/// This class is responsible for encoding the argumentation structure in a SAT-problem.
/// </summary>
class Encoding {
public:
    /// <summary>
    /// Calculates the accepting literal of the SAT-problem for the specified argument.
    /// </summary>
    /// <param name="argument">Argument to be converted into a literal of the SAT-problem.</param>
    /// <param name="isPositive">Bool indicating wheter the literal is positive. Negative literals are also called 'inverted'.</param>
    /// <returns>Returns an accepting literal.</returns>
    static int64_t get_literal_accepted(uint32_t argument, bool isPositive);
    /// <summary>
    /// Calculates the rejecting literal of the SAT-problem for the specified argument.
    /// </summary>
    /// <param name="framework">The framework of the problem.</param>
    /// <param name="argument">Argument to be converted into a literal of the SAT-problem.</param>
    /// <param name="isPositive">Bool indicating whether the literal is positive. Negative literals are also called 'inverted'.</param>
    /// <returns>Returns an rejecting literal.</returns>
    static int64_t get_literal_rejected(AF &framework, uint32_t argument, bool isPositive);
    /// <summary>
    /// Adds all clauses necessary to encode the calculation of nonempty admissible sets.
    /// </summary>
    /// <param name="solver">The SATSolver, to which the clauses will be added.</param>
    /// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_clauses_nonempty_admissible_set(SAT_Solver &solver, AF &framework, ArrayBitSet &activeArgs);
    /// <summary>
    /// Adds all clauses necessary to encode the calculation of nonempty complete extensions.
    /// </summary>
    /// <param name="solver">The SATSolver, to which the clauses will be added.</param>
    /// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_clauses_nonempty_complete_set(SAT_Solver &solver, AF &framework, ArrayBitSet &activeArgs);
    /// <summary>
    /// Adds all clauses necessary to encode the calculation of nonempty stable extensions.
    /// </summary>
    /// <param name="solver">The SATSolver, to which the clauses will be added.</param>
    /// <param name="framework">The abstract argumentation framework, based upon which the attacks are analysed.</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_clauses_nonempty_stable_set(SAT_Solver &solver, AF &framework, ArrayBitSet &activeArgs);
    /// <summary>
    /// Adds a complement clause of a found solution to a specified SATSolver. This clause is necessary to provoke the solver to
    /// find another solution to the problem.
    /// </summary>
    /// <param name="solver">The SATSolver, which has found a solution</param>
    /// <param name="activeArgs">The set of active arguments, in the current state of the framework.</param>
    static void add_complement_clause(SAT_Solver &solver, ArrayBitSet &activeArgs);
};

#endif