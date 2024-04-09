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

void TerminateRejectingQueryEmptyExtention(bool *isRejected, activeArgs_t *reduct, nodeUInt32_t **output_extension, bool *isSolved, bool *isFirstCalculation, CMSat::SATSolver *solver)
{
#pragma atomic write
	*isRejected = true;
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed
	
#pragma atomic write
	*output_extension = NULL;

	//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
	free_activeArguments(reduct);
	//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
	free(isSolved);
	//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
	free(isFirstCalculation);
	//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
	delete solver;
	//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection_parallel_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs, bool *isRejected,
	nodeUInt32_t *extension_build, nodeUInt32_t **output_extension)
{
	//, int *num_tasks, int *num_tasks_max
	int id = omp_get_thread_num();
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

	//printf("%d: starting task - argument %d ----------------------------------\n", id, argument);												//DEBUG
	//printf("Thread number %d checking argument %d\n", omp_get_thread_num() , argument);														//DEBUG
	//printf("Extension so far:");																												//DEBUG
	//print_list_uint32((extension_build));																										//DEBUG
	//printf("\n");																																//DEBUG
	
	//printf("Current active arguments:");
	//print_active_arguments(activeArgs);																										//DEBUG
	//printf("\n");																																//DEBUG
																																				//DEBUG
	
	activeArgs_t *reduct = NULL;
	if (extension_build == NULL)
	{
		reduct = copy_active_arguments(activeArgs);
		//printf("51: %d: ------- reduct copied --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
	}
	else
	{
		//printf("%d: ------- before new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());										//DEBUG
		reduct = get_reduct_set(activeArgs, framework, extension_build);
		//printf("57: %d: ------- new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
		//printf("%d: created reduct: \n ", id);																								//DEBUG
		//print_active_arguments(reduct);																										//DEBUG
		//printf("\n");																															//DEBUG
	}

	if (reduct->numberActiveArguments < 2)
	{
		//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
		//printf("%d: only 1 argument left -> check self-attack + skip reduct\n", omp_get_thread_num());															//DEBUG

		/*if (get_first_active(reduct) != argument)
		{
			printf("ERROR: argument was removed from state, without rejection_check noticing\n");
			exit(1);
		}*/

		if (framework->attackers->content[argument][argument] != 0)
		{
			//printf("%d: argument is attacking itself\n", omp_get_thread_num());
#pragma atomic write
			*isRejected = true;
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed
		}

#pragma atomic write
		*output_extension = NULL;
		
		free_activeArguments(reduct);
		//printf("86: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG

		return;
	}

	//flag used to signal that clauses need to be extended
	bool *isSolved = NULL;
	isSolved = (bool *)malloc(sizeof * isSolved);
	*isSolved = false;
	//flag used to signal that reduct has only empty set as admissible set
	bool *isFirstCalculation = NULL;
	isFirstCalculation = (bool *)malloc(sizeof * isFirstCalculation);
	*isFirstCalculation = true;
	//printf("%d: ------- isSolved allocated --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
	
	uint32_t numVars = (2 * reduct->numberActiveArguments) + 1;
	SATSolver *solver = new SATSolver();
	solver->set_num_threads(1);
	solver->new_vars(numVars);
	//printf("%d: ------- SAT solver initialized --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
	//printf("%d Encode: \n", id);																												//DEBUG
	Encodings_CMS::add_clauses_nonempty_admissible_set(solver, framework, reduct);
	//printf("%d: ------- SAT clauses added --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG

#pragma omp flush(isRejected)
#pragma omp atomic read
	isRejected_tmp = *isRejected;
	if (isRejected_tmp == true)
	{
		//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
		free(isSolved);
		//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
		free(isFirstCalculation);
		//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
		free_activeArguments(reduct);
		//printf("115: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		delete solver;
		//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
		return;
	}

	uint8_t flag_exit = EXIT_SUCCESS;

	//iterate through initial sets
	do {
		flag_exit = InitialSetSolver::calculate_next_solution(framework, reduct, solver, isSolved);
		//printf("%d: ------- initial set calculated --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
		nodeUInt32_t *initial_set = NULL;

		if (flag_exit == EXIT_FAILURE)
		{
			//no more initial sets to calculate after this one
			//printf("%d: no set \n", id);																										//DEBUG
			//no initial set found -> check for self-attacking argument
			if (framework->attackers->content[argument][argument] != 0)
			{
				//argument attacks itself
				return TerminateRejectingQueryEmptyExtention(isRejected, reduct, output_extension, isSolved, isFirstCalculation, solver);
			}
			else if (isFirstCalculation)
			{
				//since this is the first calculation and no IS was found, this reduct has only the empty set as an admissible set, therefore the extension_build is complete
				// and since only extensions not containing the query argument proceed in the calculation, extension_build cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected
				return TerminateRejectingQueryEmptyExtention(isRejected, reduct, output_extension, isSolved, isFirstCalculation, solver);
			}

			break;
		}

		*isFirstCalculation = false;

		initial_set = DecodingCMS::get_set_from_solver(solver, reduct);
		//printf("%d: ------- initial set allocated --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG

		//printf("%d: computed initial set: ", id);																							//DEBUG
		//print_list_uint32(initial_set);																									//DEBUG
		//printf("\n");																														//DEBUG
		
		if (initial_set == NULL)
		{
			//initial set is empty check for self-attacking argument
			if (framework->attackers->content[argument][argument] != 0)
			{
				//argument attacks itself
#pragma atomic write
				*isRejected = true;
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected points to a memory address, which content is changed
#pragma atomic write
				*output_extension = NULL;

				free_activeArguments(reduct);
				//printf("179: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isSolved);
				//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isFirstCalculation);
				//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
				delete solver;
				free_list_uint32(initial_set);
				//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
				return;
			}

			free_list_uint32(initial_set);
			//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			continue;
		}
		else if (check_rejection(argument, initial_set, framework))
		{
#pragma atomic write
			*isRejected = true;			
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed
			//printf("%d: initial set ", id);																									//DEBUG
			//print_list_uint32(initial_set);																									//DEBUG
			//printf(" rejects argument %d \n", argument);																						//DEBUG
			//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);					//DEBUG

			nodeUInt32_t *new_extension_build;
			if (extension_build == NULL)
			{
				new_extension_build = copy_list_uint32(initial_set);
			}
			else
			{
				new_extension_build = ExtendExtension(extension_build, initial_set);
				//printf("%d: ------- new extension allocated to extend --- memory usage: %ld\n", id, get_mem_usage());							//DEBUG
			}
			
#pragma atomic write
			*output_extension = new_extension_build;

			free(isSolved);
			//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			free(isFirstCalculation);
			//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
			free_activeArguments(reduct);
			//printf("216: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
			delete solver;
			free_list_uint32(initial_set);
			//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			return;
		}
		else if (check_terminate_extension_build(argument, initial_set))
		{
			//printf("%d: path of initial set ", id);																							//DEBUG
			//print_list_uint32(initial_set);																									//DEBUG
			//printf(" aborted\n");																												//DEBUG

			free_list_uint32(initial_set);
			//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			continue;
		}

		//copy extension to extend it
		nodeUInt32_t *new_extension_build;
		if (extension_build == NULL)
		{
			//nodeUInt32_t *extension_build = create_list_uint32(0);
			new_extension_build = copy_list_uint32(initial_set);
		}
		else
		{
			new_extension_build = ExtendExtension(extension_build, initial_set);
			//printf("%d: ------- new extension allocated to extend --- memory usage: %ld\n", id, get_mem_usage());								//DEBUG
		}

		free_activeArguments(reduct);
		//printf("247: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());															//DEBUG
		free_list_uint32(initial_set);
		//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG


//#pragma atomic write
//		*num_tasks = *num_tasks + 1;
//#pragma omp flush(num_tasks)

//		int tmp_num_tasks, tmp_num_tasks_max;
//#pragma omp atomic read
//			tmp_num_tasks = *num_tasks;
//#pragma omp flush(num_tasks_max)
//#pragma omp atomic read
//			tmp_num_tasks_max = *num_tasks_max;

//		if (tmp_num_tasks > tmp_num_tasks_max)
//		{
//#pragma atomic write
//			*num_tasks_max = tmp_num_tasks;
//
//			printf("%d: ------- New max Number of currently open tasks:  %d\n", id, tmp_num_tasks_max);
//		}

		//printf("%d: ------- Number of currently open tasks:  %d\n", id, tmp_num_tasks);

#pragma omp task \
	private(id, isRejected_tmp, isSolved , isFirstCalculation , numVars, flag_exit, initial_set, solver, reduct) \
	shared(argument, framework, activeArgs, isRejected, new_extension_build, output_extension) \
	priority(0) //firstprivate(new_extension_build) not working with untied // depend(in: argument, framework, activeArgs) depend(inout: isRejected, new_extension_build) depend(out: output_extension)
		{
			//printf("%d: ------- task started --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG
			check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected, new_extension_build, output_extension); //, num_tasks, num_tasks_max
			free_list_uint32(new_extension_build);
			//printf("%d: ------- extension freed --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG
			//int tmp_num_tasks = 0;																											//DEBUG
//#pragma atomic write
//			*num_tasks = *num_tasks - 1;
//#pragma omp flush(num_tasks)
//#pragma omp atomic read
			//tmp_num_tasks = *num_tasks;

			//printf("%d: ------- Number of currently open tasks:  %d\n", id, tmp_num_tasks);
		}
		//printf("%d: ------- task created --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		
		if (extension_build == NULL)
		{
			reduct = copy_active_arguments(activeArgs);
			//printf("294: %d: ------- reduct copied --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		}
		else
		{
			//printf("%d: ------- before new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());										//DEBUG
			reduct = get_reduct_set(activeArgs, framework, extension_build);
			//printf("300: %d: ------- new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			//printf("%d: created reduct: \n ", id);																								//DEBUG
			//print_active_arguments(reduct);																										//DEBUG
			//printf("\n");																															//DEBUG
		}

#pragma omp flush(isRejected)
#pragma omp atomic read
		isRejected_tmp = *isRejected;
	} while (flag_exit != EXIT_FAILURE && !isRejected_tmp);

	//if (flag_exit != EXIT_FAILURE && isRejected_tmp)																							//DEBUG
	//{																																			//DEBUG
	//	printf("%d: preliminary terminated.\n", id);																							//DEBUG
	//}																																			//DEBUG

	free(isSolved);
	//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
	free(isFirstCalculation);
	//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
	delete solver;
	free_activeArguments(reduct);
	//printf("320: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());															//DEBUG

	//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);							//DEBUG
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

	//printf("%d: ------- isRejected allocated --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());									//DEBUG

	*isRejected = false;

	if (numCores > 0)
	{
		omp_set_num_threads(numCores);
	}
	
	/*int *num_tasks = NULL;
	num_tasks = (int *)malloc(sizeof * num_tasks);
	if (num_tasks == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	*num_tasks = 0;*/

	/*int *num_tasks_max = NULL;
	num_tasks_max = (int *)malloc(sizeof * num_tasks_max);
	if (num_tasks_max == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	*num_tasks_max = 0;*/

#pragma omp parallel shared(argument, framework, activeArgs, isRejected, proof_extension)   //, num_tasks, num_tasks_max
#pragma omp single
	{
		// printf("%d: ------- started omp parallel -- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG

		check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected, NULL, proof_extension);					// , num_tasks, num_tasks_max
	}
	
	bool result = *isRejected;
	free(isRejected);

	/*printf("Finished program - voluntary context switches: %ld - involuntary context switches: %ld - memory usage: %ld [kB]\n",
		get_ctxt_switches_volun(), get_ctxt_switches_involun(), get_mem_usage());*/																 //DEBUG
	return result;
}