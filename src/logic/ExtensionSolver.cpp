#include "../../include/logic/ExtensionSolver.h"
using namespace std;

static void CalculateOneExtension(argFramework_t *framework, activeArgs_t *activeArgs, nodeUInt32_t **extension, SOLVERS solver_type)
{
	bool *isSolved = NULL;
	isSolved = (bool *)malloc(sizeof * isSolved);
	*isSolved = false;
	uint64_t numVars = activeArgs->numberActiveArguments;
	SatSolver *solver;
	
	if (solver_type == SOLVERS::CMS)
	{
		solver = new SatSolver_cms(numVars);
	}
	else
	{
		solver = new SatSolver_cadical(numVars);
	}
	
	Encodings_SatSolver::add_clauses_nonempty_admissible_set(solver, framework, activeArgs);
	bool flag_no_sets = InitialSetSolver::calculate_next_solution(framework, activeArgs, solver, isSolved);
	free(isSolved);
	if (flag_no_sets == EXIT_FAILURE)
	{
		return;			//[TERMINATION CONDITION]
	}

	nodeUInt32_t *initial_set = Decoding_SatSolver::get_set_from_solver(solver, activeArgs);

	//printf("Extend extension by: ");																				//DEBUG
	//print_list_uint32(initial_set);																				//DEBUG
	//printf("\n");																									//DEBUG

	nodeUInt32_t *p = NULL;
	for (p = initial_set; p; p = p->next)
	{
		push_uint32((*extension), p->number);
	}

	activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
	free_list_uint32(initial_set);
	
	if (reduct->numberActiveArguments == 0)
	{
		return;		//[TERMINATION CONDITION]
	}

	//printf("Search for completing extension in reduct: \n");														//DEBUG
	//print_active_arguments(reduct);																				//DEBUG
	//printf("\n");																									//DEBUG

	CalculateOneExtension(framework, reduct, extension, solver_type);		//[RECURSIVE CALL]
	
	free_activeArguments(reduct);
	
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void EXTENSIONSOLVER_CMS::BuildExtension(argFramework_t *framework, activeArgs_t *activeArgs, nodeUInt32_t **extension, SOLVERS solver_type)
{
	if (*extension == NULL)
	{
		printf("ERROR: [EXTENSIONSOLVER_CMS::BuildExtension] no extension to build upon");
		exit(1);
	}


	activeArgs_t *reduct = get_reduct_set(activeArgs, framework, *extension);

	if (reduct->numberActiveArguments == 0)
	{
		return;		//[TERMINATION CONDITION]
	}

	//printf("Search for completing extension in reduct: \n");														//DEBUG
	//print_active_arguments(reduct);																				//DEBUG
	//printf("\n");																									//DEBUG

	CalculateOneExtension(framework, reduct, extension, solver_type);

	free_activeArguments(reduct);
}