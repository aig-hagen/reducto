#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <iostream>
#include <cstdint>
#include <vector>

/// <summary>
/// This class is responsible for solving a problem encoded as a SAT-problem
/// </summary>
class SatSolver {
public:

	/// <summary>
	/// Adds a new clause to the SAT-problem of the solver.
	/// </summary>
	/// <param name="clause">The SAT-clause to add.</param>
	virtual void add_clause(std::vector<int64_t> &clause) = 0;

	/// <summary>
	/// Adds a new clause, containing up to 2 literals, to the SAT-problem of the solver.
	/// </summary>
	/// <param name="lit1">First literal of the clause to add.</param>
	/// <param name="lit2">Second literal of the clause to add. 
	/// In case, that the clause has only 1 literal, this parameter is NULL</param>
	virtual void add_clause_short(int64_t lit1, int64_t lit2) = 0;

	/// <summary>
	/// Returns the solution for the SAT-problem of the solver. 
	/// Rejecting variables are listed in the second half of the returned data.
	/// </summary>
	/// <returns>Array of type bool, indicating for each variable if it's set or not</returns>
	virtual std::vector<bool> get_model() = 0;

	/// <summary>
	/// Method to check if single variable is part of the model.
	/// </summary>
	/// <param name="variable">The variable of the SAT-problem to check.</param>
	/// <returns>TRUE if the variable is true in the model.</returns>
	virtual std::uint8_t check_var_model(int64_t variable) = 0;

	/// <summary>
	/// This method calculates a solution for the SAT-Problem of the solver.
	/// </summary>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	virtual bool solve() = 0;

	/// <summary>
	/// This method calculates a solution for the SAT-Problem of the solver.
	/// </summary>
	/// <param name="assumptions">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	virtual bool solve(int64_t assumption) = 0;

	/// <summary>
	/// This method calculates a solution for the SAT-Problem of the solver.
	/// </summary>
	/// <param name="assumption1">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// <param name="assumption2">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	virtual bool solve(int64_t assumption1, int64_t assumption2) = 0;
};


#endif