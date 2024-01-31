#include "../include/ScepticalPRParallel.h"
using namespace std;

static void check_rejection_parallel_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs, bool *isRejected)
{
	if (*isRejected == true)
	{
		printf("%d: preliminary terminated.\n", omp_get_thread_num());																									//DEBUG
		printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);													//DEBUG
		return;
	}

	printf("%d: starting task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);														//DEBUG
	//printf("Thread number %d checking argument %d computing state: \n", omp_get_thread_num() , argument);																//DEBUG
	//print_active_arguments(activeArgs);																																//DEBUG
	//printf("\n");																																						//DEBUG
	int id = omp_get_thread_num();																																		//DEBUG

	bool *isSolved = NULL;
	isSolved = (bool *)malloc(sizeof * isSolved);
	*isSolved = false;
	
	uint32_t numVars = (2 * activeArgs->numberActiveArguments) + 1;
	SATSolver solver;
	solver.set_num_threads(1);
	solver.new_vars(numVars);
	//printf("%d Encode: \n", id);																																		//DEBUG
	Encodings_CMS::add_clauses_nonempty_admissible_set(&solver, framework, activeArgs);

	uint8_t flag_exit = EXIT_SUCCESS;

	if (*isRejected == true)
	{
		printf("%d: preliminary terminated.\n", omp_get_thread_num());																									//DEBUG
		free(isSolved);
		//TODO			solver.free();
		printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);													//DEBUG
		return;
	}

	//iterate through initial sets
	do {
		flag_exit = InitialSetSolver::calculate_next_solution(framework, activeArgs, &solver, isSolved);
		nodeUInt32_t *initial_set = NULL;

		if (flag_exit == EXIT_FAILURE)
		{
			//no more initial sets to calculate after this one
			printf("%d: no set \n", id);																						//DEBUG
			break;
		}
		else
		{
			initial_set = DecodingCMS::get_set_from_solver(&solver, activeArgs);

			printf("%d: computed initial set: ", id);																			//DEBUG
			print_list_uint32(initial_set);																						//DEBUG
			printf("\n");																										//DEBUG
		}

		if (check_rejection(argument, initial_set, framework))
		{
			*isRejected = true;
			printf("%d: initial set ", id);																					//DEBUG
			print_list_uint32(initial_set);																					//DEBUG
			printf(" rejects argument %d \n", argument);																	//DEBUG
			printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);													//DEBUG
			return;
		}

		if (check_terminate_extension_build(argument, initial_set))
		{
			printf("%d: path of initial set ", id);																			//DEBUG
			print_list_uint32(initial_set);																					//DEBUG
			printf(" aborted\n");																							//DEBUG

			if (flag_exit != EXIT_FAILURE && *isRejected == true)
			{
				printf("%d: preliminary terminated.\n", omp_get_thread_num());												//DEBUG
			}
			continue;
		}

		activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
		free_list_uint32(initial_set);

		if (reduct->numberActiveArguments < 2)
		{
			//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
			printf("%d: only 1 argument left -> skip reduct\n", omp_get_thread_num());																			//DEBUG

			/*if (get_first_active(reduct) != argument)
			{
				printf("ERROR: argument was removed from state, without rejection_check noticing\n");
				exit(1);
			}*/

			if (flag_exit != EXIT_FAILURE && *isRejected == true)
			{
				printf("%d: preliminary terminated.\n", omp_get_thread_num());												//DEBUG
			}
			continue;
		}

		printf("%d: created task for reduct: \n ", id);																		//DEBUG
		print_active_arguments(reduct);																						//DEBUG
		printf("\n");																										//DEBUG

#pragma omp task firstprivate(reduct) priority(0)
		{
			check_rejection_parallel_recursiv(argument, framework, reduct, isRejected);
			free_activeArguments(reduct);
		}

		if (flag_exit != EXIT_FAILURE && *isRejected == true)
		{
			printf("%d: preliminary terminated.\n", omp_get_thread_num());													//DEBUG
		}

	} while (flag_exit != EXIT_FAILURE && *isRejected == false);

	free(isSolved);
	//TODO			solver.free();

	printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);													//DEBUG
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