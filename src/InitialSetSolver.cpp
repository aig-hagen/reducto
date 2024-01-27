#include "../include/InitialSetSolver.h"

using namespace std;


uint8_t InitialSetSolver::calculate_next_solution(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem)
{
	if (problem->isSolved)
	{
		add_complement_clause(problem);
	}

	//printf("Encoding SAT: ");																	 //DEBUG
	//print_list_list_int64(problem->clauses);													 //DEBUG
	//printf("\n");																				 //DEBUG

	/*if (ExternalSatSolver::solve_pstreams(problem, "cryptominisat5") == 20)
	{
		return EXIT_FAILURE;
	}*/

	if (ExternalSatSolver::solve_cms(problem) == 20)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}