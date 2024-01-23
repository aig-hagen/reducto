#include "../include/ExternalSolver.h"

using namespace std;
using namespace CMSat;

void static add_clauses(SATSolver solver, listInt64_t *clauses)
{
	listInt64_t *currentClause = clauses;

	while (currentClause != NULL)
	{
		nodeInt64_t *currentLiteral = currentClause->list;
		vector<Lit> lits(count_list_int64(currentLiteral));
		uint32_t i = 0;

		while (currentLiteral != NULL)
		{
			uint32_t var = abs(currentLiteral->number);
			lits[i] = Lit(var, (currentLiteral->number > 0) ? false : true);

			i++;
			currentLiteral = currentLiteral->next;
		}

		solver.add_clause(lits);
		currentClause = currentClause->next;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t ExternalSatSolver::solve_cms(SATProblem_t *problem)
{
	uint32_t numVars = problem->numberOfVariables * 2 + 1;
	CMSat::SATSolver solver;
	solver.set_num_threads(1);
	solver.new_vars(numVars);
	add_clauses(solver, problem->clauses);

	bool sat = (solver.solve() == l_True);
	if (sat) {
		for (int i = 0; i < numVars; i++) {
			problem->solution->elements[i] = (solver.get_model()[i] == l_True) ? 1 : 0;
		}
	}
	return sat ? 10 : 20;
}

