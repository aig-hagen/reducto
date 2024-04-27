#ifndef SAT_SOLVER_CMS_H
#define SAT_SOLVER_CMS_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <vector>

#include "SatSolver.h"
#include <cryptominisat.h>

using namespace CMSat;

/*
Class implement interface with CMS solver
*/
class SatSolver_cms : public SatSolver {
    public:
		SatSolver_cms(uint64_t num_var);

		void AddVariableToClause(int64_t variable, std::vector<CMSat::Lit> &lits);

		/// <summary>
		/// Adds a new clause to the SAT-problem of the solver.
		/// </summary>
		/// <param name="clause">The SAT-clause to add.</param>
		void add_clause(std::vector<int64_t> clause);

		/// <summary>
		/// Adds a new clause, containing up to 2 literals, to the SAT-problem of the solver.
		/// </summary>
		/// <param name="lit1">First literal of the clause to add.</param>
		/// <param name="lit2">Second literal of the clause to add. In case clause has only 1 literal, this parameter is NULL</param>
		void add_clause_short(int64_t lit1, int64_t lit2);

		/// <summary>
		/// Returns the solution for the SAT-problem of the solver. Rejecting variable are returned after all accepting variables are returned.
		/// </summary>
		/// <returns>Array of type bool, indicating for each variable if it's set or not</returns>
		std::vector<bool> get_model();

		/// <summary>
		/// This method calculates a solution for the SAT-Problem of the solver.
		/// </summary>
		/// <returns> <c>TRUE</c> iff a solution was found. Otherwise <c>FALSE</c></returns>
		bool solve();

	private: 
		/// <summary>
		/// The cryptominisat-solver used to solve the SAT-problem.
		/// </summary>
		SATSolver cms_solver;
};

#endif