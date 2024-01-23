#include "../include/InitialSetSolver.h"

using namespace std;


//uint8_t InitialSetSolver::calculate_next_initial_set(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem, nodeUInt32_t *out_initial_set)
//{
//	if (problem->isSolved)
//	{
//		add_complement_clause(problem);
//	}
//	
//	if (ExternalSatSolver::solve(problem, "cryptominisat5") == 20)
//	{
//		return EXIT_FAILURE;
//	}
//	
//	//printf("\n");																					//DEBUG
//	//print_active_arguments(activeArgs);															//DEBUG
//	printf("\n");																					//DEBUG
//	printf("computed solution: ");																	//DEBUG
//	print_array(problem->solution);																	//DEBUG
//	printf("\n");																					//DEBUG
//
//	*out_initial_set = *get_set_from_problem(problem, activeArgs);
//	return EXIT_SUCCESS;
//}

uint8_t InitialSetSolver::calculate_next_initial_set(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem, nodeUInt32_t *out_initial_set)
{
	arrayInt_t *solution = create_array(problem->solution->length);
	if (calculate_next_solution(framework, activeArgs, problem, solution) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	//printf("\n");																					//DEBUG
	//print_active_arguments(activeArgs);															//DEBUG
	printf("\n");																					//DEBUG
	printf("computed solution: ");																	//DEBUG
	print_array(problem->solution);																	//DEBUG
	printf("\n");																					//DEBUG

	*out_initial_set = *get_set_from_solution(solution, activeArgs);
	return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t InitialSetSolver::calculate_next_solution(argFramework_t *framework, activeArgs_t *activeArgs, SATProblem_t *problem, arrayInt_t *out_solution)
{
	if (problem->isSolved)
	{
		add_complement_clause(problem);
	}

	/*if (ExternalSatSolver::solve_pstreams(problem, "cryptominisat5") == 20)
	{
		return EXIT_FAILURE;
	}*/

	if (ExternalSatSolver::solve_cms(problem) == 20)
	{
		return EXIT_FAILURE;
	}

	*out_solution = *problem->solution;
	return EXIT_SUCCESS;
}