#include "../../include/logic/ScepticalPRParallel.h"
using namespace std;

static nodeUInt32_t *ExtendExtension(nodeUInt32_t *extension_build, nodeUInt32_t *initial_set)
{
	nodeUInt32 *tmpCopy = copy_list_uint32(extension_build);
	nodeUInt32_t *p = NULL;
	for (p = initial_set; p; p = p->next)
	{
		push_uint32(tmpCopy, p->number);
	}

	return tmpCopy;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection_parallel_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs, bool *isRejected,
	nodeUInt32_t *extension_build, nodeUInt32_t **output_extension)
{
	bool isRejected_tmp = false;

#pragma omp flush(isRejected)
#pragma omp atomic read
	isRejected_tmp = *isRejected;
	if (isRejected_tmp == true)
	{
		//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
		//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
		return;
	}

	//printf("%d: starting task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);							//DEBUG
	//printf("Thread number %d checking argument %d computing state: \n", omp_get_thread_num() , argument);									//DEBUG
	//print_active_arguments(activeArgs);																									//DEBUG
	//printf("\n");																															//DEBUG
	//int id = omp_get_thread_num();																											//DEBUG

	bool *isSolved = NULL;
	isSolved = (bool *)malloc(sizeof * isSolved);
	*isSolved = false;
	
	uint32_t numVars = (2 * activeArgs->numberActiveArguments) + 1;
	SATSolver solver;
	solver.set_num_threads(1);
	solver.new_vars(numVars);
	//printf("%d Encode: \n", id);																											//DEBUG
	Encodings_CMS::add_clauses_nonempty_admissible_set(&solver, framework, activeArgs);

#pragma omp flush(isRejected)
#pragma omp atomic read
	isRejected_tmp = *isRejected;
	if (isRejected_tmp == true)
	{
		//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
		free(isSolved);
		//TODO			solver.free();
		//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
		return;
	}

	uint8_t flag_exit = EXIT_SUCCESS;

	//iterate through initial sets
	do {
		flag_exit = InitialSetSolver::calculate_next_solution(framework, activeArgs, &solver, isSolved);
		nodeUInt32_t *initial_set = NULL;

		if (flag_exit == EXIT_FAILURE)
		{
			//no more initial sets to calculate after this one
			//printf("%d: no set \n", id);																						//DEBUG
			break;
		}
		else
		{
			initial_set = DecodingCMS::get_set_from_solver(&solver, activeArgs);

			//printf("%d: computed initial set: ", id);																			//DEBUG
			//print_list_uint32(initial_set);																						//DEBUG
			//printf("\n");																										//DEBUG
		}

		if (check_rejection(argument, initial_set, framework))
		{
#pragma atomic write
			*isRejected = true;			
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed
			//printf("%d: initial set ", id);																					//DEBUG
			//print_list_uint32(initial_set);																					//DEBUG
			//printf(" rejects argument %d \n", argument);																	//DEBUG
			//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);	//DEBUG

			nodeUInt32_t *new_extension_build = ExtendExtension(extension_build, initial_set);
			free_list_uint32(initial_set);
#pragma atomic write
			*output_extension = new_extension_build;
			return;
		}

		if (check_terminate_extension_build(argument, initial_set))
		{
			//printf("%d: path of initial set ", id);																			//DEBUG
			//print_list_uint32(initial_set);																					//DEBUG
			//printf(" aborted\n");																							//DEBUG

			continue;
		}

		activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);

		if (reduct->numberActiveArguments < 2)
		{
			//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
			//printf("%d: only 1 argument left -> skip reduct\n", omp_get_thread_num());																			//DEBUG

			/*if (get_first_active(reduct) != argument)
			{
				printf("ERROR: argument was removed from state, without rejection_check noticing\n");
				exit(1);
			}*/

			continue;
		}

		//copy extension to extend it
		nodeUInt32_t *new_extension_build = ExtendExtension(extension_build, initial_set);
		free_list_uint32(initial_set);

		//printf("%d: created task for reduct: \n ", id);																		//DEBUG
		//print_active_arguments(reduct);																						//DEBUG
		//printf("\n");																										//DEBUG

#pragma omp task firstprivate(reduct) priority(0)
		{
			check_rejection_parallel_recursiv(argument, framework, reduct, isRejected, new_extension_build, output_extension);
			free_activeArguments(reduct);
			free_list_uint32(new_extension_build);
		}

#pragma omp flush(isRejected)
#pragma omp atomic read
		isRejected_tmp = *isRejected;
	} while (flag_exit != EXIT_FAILURE && !isRejected_tmp);

	//if (flag_exit != EXIT_FAILURE && isRejected_tmp)
	//{
	//	printf("%d: preliminary terminated.\n", omp_get_thread_num());													//DEBUG
	//}

	free(isSolved);
	//TODO			solver.free();

	//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);		//DEBUG
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool ScepticalPRParallel::check_rejection_parallel(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs, nodeUInt32_t **proof_extension, uint8_t numCores)
{
	bool *isRejected = NULL;
	isRejected = (bool *)malloc(sizeof *isRejected);
	if (isRejected == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	//else {
		*isRejected = false;
		nodeUInt32_t *extension_build = create_list_uint32(0);

		if (numCores > 0)
		{
			omp_set_num_threads(numCores);
		}
		
#pragma omp parallel shared(argument, framework, activeArgs, isRejected, proof_extension)
#pragma omp single
		{
			check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected, extension_build, proof_extension);
		}
		bool result = *isRejected;
		free(isRejected);
		free_list_uint32(extension_build);

		if ((*proof_extension)->number == 0)
		{
			*proof_extension = remove_head_list_uint32(*proof_extension);		//removed added 0 at the beginning
		}
	//}

	return result;
}