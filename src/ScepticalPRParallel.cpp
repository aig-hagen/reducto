#include "../include/ScepticalPRParallel.h"
using namespace std;

static void check_rejection_parallel_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs, bool *isRejected)
{
	//printf("--------------------------------------------------------------");											//DEBUG
	//printf("Thread number %d computing reduct: \n", omp_get_thread_num());												//DEBUG
	//print_active_arguments(activeArgs);																					//DEBUG
	//printf("\n");																										//DEBUG

	if (activeArgs->numberActiveArguments < 2)
	{
		//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
		if (get_first_active(activeArgs) != argument)
		{
			printf("ERROR: argument was removed from state, without rejection_check noticing");
			exit(1);
		}

		return;
	}

	SATProblem_t *problem = create_sat_problem(activeArgs->numberActiveArguments);
	add_clauses_nonempty_admissible_set(problem, framework, activeArgs);
	uint8_t flag_exit = EXIT_SUCCESS;

	//iterate through initial sets
	do {
		flag_exit = InitialSetSolver::calculate_next_solution(framework, activeArgs, problem);
		nodeUInt32_t *initial_set = get_set_from_solution(problem->solution, activeArgs);
		if (flag_exit == EXIT_FAILURE)
		{
			//no initial set calculated
			break;
		}
		if (check_rejection(argument, initial_set, framework))
		{
			*isRejected = true;
			return;
		}

		if (check_terminate_extension_build(argument, initial_set))
		{
			continue;
		}

		activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
		free_list_uint32(initial_set);

#pragma omp task firstprivate(reduct) priority(0)
		{
			check_rejection_parallel_recursiv(argument, framework, reduct, isRejected);
			free_activeArguments(reduct);
		}

		if (flag_exit != EXIT_FAILURE && *isRejected == true)
		{
			printf("Thread %d preliminary terminated.\n", omp_get_thread_num());
		}

	} while (flag_exit != EXIT_FAILURE && *isRejected == false);

	free_problem(problem);

	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool ScepticalPRParallel::check_rejection_parallel(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs)
{
	bool *isRejected = NULL;
	isRejected = (bool *)malloc(sizeof * isRejected);
	if (isRejected == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	else {
		*isRejected = false;

#pragma omp parallel shared(argument, framework, activeArgs, isRejected)
#pragma omp single
		{
			check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected);
		}
	}

	return *isRejected;
}