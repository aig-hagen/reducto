#ifndef SAT_SOLVER_CADICAL_H
#define SAT_SOLVER_CADICAL_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "SatSolver.h"
#include "./../../lib/cadical/src/cadical.hpp"

using namespace CaDiCaL;

/// <summary>
///	This class is responsible for the correct usage of the
/// SAT-Solver CaDiCaL, by implementing the interface for Sat-Solvers
/// in this application.
/// </summary>
class SatSolver_cadical : public SatSolver {
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	/// <param name="number_arguments">The number of arguments (not SAT-variables) 
	/// in the solution to calculate.</param>
	SatSolver_cadical(uint64_t number_arguments);

	/// <summary>
	/// Adds a new clause to the SAT-problem of the solver.
	/// </summary>
	/// <param name="clause">The SAT-clause to add.</param>
	void add_clause(std::vector<int64_t> &clause);

	/// <summary>
	/// Adds a new clause, containing up to 2 literals, to the SAT-problem of the solver.
	/// </summary>
	/// <param name="lit1">First literal of the clause to add.</param>
	/// <param name="lit2">Second literal of the clause to add. 
	/// In case, that the clause has only 1 literal, this parameter is NULL</param>
	void add_clause_short(int64_t lit1, int64_t lit2);

	/// <summary>
	/// Returns the solution for the SAT-problem of the solver. 
	/// Rejecting variables are listed in the second half of the returned data.
	/// </summary>
	/// <returns>Array of type bool, indicating for each variable if it's set or not</returns>
	std::vector<bool> get_model();

	/// <summary>
	/// Method to check if single variable is part of the model.
	/// </summary>
	/// <param name="variable">The variable of the SAT-problem to check.</param>
	/// <returns>TRUE if the variable is true in the model.</returns>
	std::uint8_t check_var_model(int64_t variable);

	/// <summary>
	/// This method calculates a solution for the SAT-Problem of the solver.
	/// </summary>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	bool solve();

private:
	/// <summary>
	/// The SAT-solver used to solve the SAT-problem.
	/// </summary>
	CaDiCaL::Solver cadical_solver;

	/// <summary>
	/// The number of arguments in the solution of this solver.
	/// </summary>
	int _num_arguments;
};

#endif