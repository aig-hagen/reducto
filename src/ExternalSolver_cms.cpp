#include "../include/ExternalSolver.h"

using namespace std;
using std::vector;
using namespace CMSat;

void static add_clauses(SATSolver *solver, listInt64_t *clauses)
{
	listInt64_t *currentClause = clauses;

	while (currentClause != NULL)
	{
		nodeInt64_t *currentLiteral = currentClause->list;
		vector<Lit> clause;

		while (currentLiteral != NULL)
		{
			uint32_t var = abs(currentLiteral->number) - 1;
			clause.push_back(Lit(var, (currentLiteral->number > 0) ? false : true));

			currentLiteral = currentLiteral->next;
		}

		(*solver).add_clause(clause);
		currentClause = currentClause->next;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t ExternalSatSolver::solve_cms(SATProblem_t *problem)
{
	uint32_t numVars = problem->numberOfVariables * 2;
	SATSolver solver;
	solver.set_num_threads(1);
	solver.new_vars(numVars);
	add_clauses(&solver, problem->clauses);

	lbool ret = solver.solve();
	bool sat = ( ret == l_True);
	if (sat) {
		//printf("Solution: ");																					//DEBUG
		for (int i = 0; i < numVars; i++) {
			problem->solution->elements[i + 1] = (solver.get_model()[i] == l_True) ? 1 : 0;
			/*lbool variable = solver.get_model()[i];															//DEBUG
			problem->solution->elements[i + 1] = variable == l_True ? 1 : 0;									//DEBUG
			printf(" [");																						//DEBUG
			printf("%d]", problem->solution->elements[i + 1]);*/												//DEBUG
		}
		//printf("\n");																							//DEBUG
	}

	//printf("Found solution: ");																				//DEBUG
	//print_array(problem->solution);																			//DEBUG
	//printf("\n");																								//DEBUG

	problem->isSolved = true;
	return sat ? 10 : 20;
}

