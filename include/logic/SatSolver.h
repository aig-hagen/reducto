#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <vector>

class SatSolver {
public:

	/// <summary>
	/// Adds a new clause to the SAT-problem of the solver.
	/// </summary>
	/// <param name="clause">The SAT-clause to add.</param>
	virtual void add_clause(std::vector<int64_t> clause) = 0;

	/// <summary>
	/// Returns the solution for the SAT-problem of the solver. Rejecting variable are returned after all accepting variables are returned.
	/// </summary>
	/// <returns>Array of type bool, indicating for each variable if it's set or not</returns>
	virtual std::vector<bool> get_model() = 0;

	/// <summary>
	/// This method calculates a solution for the SAT-Problem of the solver.
	/// </summary>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	virtual bool solve() = 0;
};


#endif