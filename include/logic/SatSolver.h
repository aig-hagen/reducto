#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <iostream>
#include <cstdint>
#include <vector>

extern "C" {
	#include "../../sat/ipasir.h"
}

/// <summary>
/// This class is responsible for realizing an interface to the SATSolver, an extern program to solve the satisfiability problem (SAT) of a propositional formula.
/// </summary>
class SatSolver {
public:

	/// <summary>
	/// Instantiates the a new SATSolver.
	/// </summary>
	/// <param name="num_args">Number of arguments in the current state of the framework.</param>
	SatSolver(uint32_t num_args);

	/// <summary>
	/// This method adds a temporary assumption to the solver, which will be deleted after the solver calculates the next solution.
	/// </summary>
	/// <param name="clause">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	void add_assumption(int64_t assumption);

	/// <summary>
	/// Adds a new clause to the SAT-problem of the solver. This method is used to define the propositional formula of the SatSolver, which is formulated in conjunctive normal form.
	/// </summary>
	/// <param name="clause">List of numbers, representing the literals, which are combined by an disjunction.</param>
	void add_clause(std::vector<int64_t> &clause);

	/// <summary>
	/// Adds a new clause, containing up to 2 literals, to the SAT-problem of the solver.
	/// </summary>
	/// <param name="lit1">First literal of the clause to add.</param>
	/// <param name="lit2">Second literal of the clause to add. 
	/// In case, that the clause has only 1 literal, this parameter is NULL</param>
	void add_clause_short(int64_t lit1, int64_t lit2);

	/// <summary>
	/// This method is used after the SatSolver has finished calculating a solution to get the calculated solution.
	/// Returns an assignment of the variables, which evaluates the propositional formula of the problem to true. 
	/// Rejecting variables are listed in the second half of the returned data.
	/// </summary>
	/// <returns>Array of type bool, indicating for each variable if it's set or not</returns>
	std::vector<bool> get_model();

	/// <summary>
	/// Method to check if single variable is set true in the solution of the SATProblem. An assignment of variables solving the SATProblem is also called a "model".
	/// </summary>
	/// <param name="variable">The variable of the SAT-problem to check.</param>
	/// <returns>TRUE if the variable is set true in the model.</returns>
	std::uint8_t check_var_model(int64_t variable);

	/// <summary>
	/// This method calculates an assignment, which evaluates the propositional formula of the SatSolver to true.
	/// </summary>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	bool solve();

	/// <summary>
	/// This method calculates an assignment, which evaluates the propositional formula of the SatSolver to true, under the specified assumption.
	/// </summary>
	/// <param name="assumption">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	bool solve(int64_t assumption);

	/// <summary>
	/// This method calculates an assignment, which evaluates the propositional formula of the SatSolver to true, under the specified assumptions.
	/// </summary>
	/// <param name="assumption1">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// /// <param name="assumption2">Variable the SAT-solver has to assume to be true if positive and false if negative.</param>
	/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
	bool solve(int64_t assumption1, int64_t assumption2);

private:
	void *_solver;
	uint32_t _num_arguments;
};


#endif