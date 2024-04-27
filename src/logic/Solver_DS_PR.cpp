#include "../../include/logic/Solver_DS_PR.h"
using namespace std;

static list<uint32_t> ExtendExtension(list<uint32_t> &extension_build, list<uint32_t> &initial_set)
{
	list<uint32_t> tmpCopy;
	tmpCopy.insert(tmpCopy.end(), extension_build.begin(), extension_build.end());
	tmpCopy.insert(tmpCopy.end(), initial_set.begin(), initial_set.end());
	return tmpCopy;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection_parallel_recursiv(uint32_t argument, AF &framework, vector<uint32_t> &activeArgs, bool *isRejected,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension, SOLVERS solver_type)
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
	
	vector<uint32_t> reduct;
	if (extension_build.empty())
	{
		reduct.insert(reduct.end(), activeArgs.begin(), activeArgs.end());
		//printf("%d: ------- reduct copied --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
	}
	else
	{
		//printf("%d: ------- before new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());										//DEBUG
		reduct = Reduct::get_reduct_set(activeArgs, framework, extension_build);
		//printf("%d: ------- new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
		//printf("%d: created reduct: \n ", id);																								//DEBUG
		//print_active_arguments(reduct);																										//DEBUG
		//printf("\n");																															//DEBUG
	}

	if (reduct.size() < 2)
	{
		//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
		//printf("%d: only 1 argument left -> skip reduct\n", omp_get_thread_num());															//DEBUG
		
		reduct.clear();
		//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG

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
	
	uint64_t numVars = reduct.size();

	SatSolver *solver = NULL;
	if (solver_type == SOLVERS::CMS)
	{
		solver = new SatSolver_cms(numVars);
	}
	else
	{
		solver = new SatSolver_cadical(numVars);
	}
	
	//printf("%d: ------- SAT solver initialized --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
	//printf("%d Encode: \n", id);																												//DEBUG
	Encodings_SatSolver::add_clauses_nonempty_admissible_set(*solver, framework, reduct);
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
		reduct.clear();
		//printf("115: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		delete solver;
		//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
		return;
	}

	bool has_Solution = true;

	//iterate through initial sets
	do {
		if (*isSolved)
		{
			//printf("%d: added complement clause \n", omp_get_thread_num());																			//DEBUG
			Encodings_SatSolver::add_complement_clause(*solver, activeArgs);
		}

		*isSolved = true;

		has_Solution = (*solver).solve();
		//printf("%d: ------- initial set calculated --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG

		if (!has_Solution)
		{
			//no more initial sets to calculate after this one
			//printf("%d: no set \n", id);																										//DEBUG

			if (*isFirstCalculation)
			{
				//since this is the first calculation and no IS was found, this reduct has only the empty set as an admissible set, therefore the extension_build is complete
				// and since only extensions not containing the query argument proceed in the calculation, extension_build cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected
				//printf("%d: found only empty set in reduct\n", omp_get_thread_num());																	//DEBUG
				
#pragma atomic write
				*isRejected = true;
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed

				//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
				reduct.clear();
				//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isSolved);
				//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isFirstCalculation);
				//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
				delete solver;
				//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
				return;
			}

			break;
		}

		list<uint32_t> initial_set = Decoding::get_set_from_solver(*solver, reduct);
		//printf("%d: ------- initial set allocated --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG

		//printf("%d: computed initial set: ", id);																							//DEBUG
		//print_list_uint32(initial_set);																									//DEBUG
		//printf("\n");																														//DEBUG
		
		if (initial_set.empty())
		{
			if (*isFirstCalculation)
			{
				//since this is the first calculation and only the empty set as IS was found, the extension_build is complete
				// and since only extensions not containing the query argument proceed in the calculation, extension_build cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected
				//printf("%d: found only empty set in reduct\n", omp_get_thread_num());																	//DEBUG
				
#pragma atomic write
				*isRejected = true;
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed

				//printf("%d: preliminary terminated.\n", omp_get_thread_num());																		//DEBUG
				reduct.clear();
				//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isSolved);
				//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
				free(isFirstCalculation);
				//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG
				delete solver;
				//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);						//DEBUG
				initial_set.clear();
				//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
				return;
			}
			else
			{
				printf("ERROR impossible that empty IS calculated and is not first calculation");
				exit(1);
			}
		}

		*isFirstCalculation = false;

		if (ScepticalCheck::check_rejection(argument, initial_set, framework))
		{
#pragma atomic write
			*isRejected = true;			
#pragma omp flush(isRejected)		//maybe flush is not needed since isRejected point so a memory address, which content is changed
			//printf("%d: initial set ", id);																									//DEBUG
			//print_list_uint32(initial_set);																									//DEBUG
			//printf(" rejects argument %d \n", argument);																						//DEBUG
			//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);					//DEBUG

			list<uint32_t> new_extension_build = ExtendExtension(extension_build, initial_set);
			//printf("%d: ------- new extension created --- memory usage: %ld\n", id, get_mem_usage());											//DEBUG	
			
#pragma atomic write
			output_extension = new_extension_build;

			free(isSolved);
			//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			free(isFirstCalculation);
			//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());										//DEBUG
			reduct.clear();
			//printf("216: %d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			delete solver;
			initial_set.clear();
			//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			return;
		}
		else if (ScepticalCheck::check_terminate_extension_build(argument, initial_set))
		{
			//printf("%d: path of initial set ", id);																							//DEBUG
			//print_list_uint32(initial_set);																									//DEBUG
			//printf(" aborted\n");																												//DEBUG

			initial_set.clear();
			//printf("%d: ------- initial set freed --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			continue;
		}

		list<uint32_t> new_extension_build = ExtendExtension(extension_build, initial_set);
		//printf("%d: ------- new extension created --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
		
		reduct.clear();
		//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		initial_set.clear();
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
	private(id, isRejected_tmp, isSolved , isFirstCalculation , numVars, has_Solution, initial_set, solver, reduct) \
	shared(argument, framework, activeArgs, isRejected, new_extension_build, output_extension) \
	priority(0)
		{
			//printf("%d: ------- task started --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG
			check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected, new_extension_build, output_extension, solver_type); //, num_tasks, num_tasks_max
			new_extension_build.clear();
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
		
		vector<uint32_t> reduct;
		if (extension_build.empty())
		{
			reduct.insert(reduct.end(), activeArgs.begin(), activeArgs.end());
			//printf("%d: ------- reduct copied --- memory usage: %ld\n", id, get_mem_usage());														//DEBUG
		}
		else
		{
			//printf("%d: ------- before new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());										//DEBUG
			reduct = Reduct::get_reduct_set(activeArgs, framework, extension_build);
			//printf("%d: ------- new reduct allocated --- memory usage: %ld\n", id, get_mem_usage());												//DEBUG
			//printf("%d: created reduct: \n ", id);																								//DEBUG
			//print_active_arguments(reduct);																										//DEBUG
			//printf("\n");																															//DEBUG
		}

#pragma omp flush(isRejected)
#pragma omp atomic read
		isRejected_tmp = *isRejected;
	} while (has_Solution && !isRejected_tmp);

	//if (has_Solution && isRejected_tmp)																											//DEBUG
	//{																																				//DEBUG
	//	printf("%d: preliminary terminated.\n", id);																								//DEBUG
	//}																																				//DEBUG

	free(isSolved);
	//printf("%d: ------- isSolved freed --- memory usage: %ld\n", id, get_mem_usage());															//DEBUG
	free(isFirstCalculation);
	//printf("%d: ------- isFirstCalculation freed --- memory usage: %ld\n", id, get_mem_usage());													//DEBUG
	delete solver;
	reduct.clear();
	//printf("%d: ------- reduct freed --- memory usage: %ld\n", id, get_mem_usage());																//DEBUG

	//printf("%d: finished task - argument %d ----------------------------------\n", omp_get_thread_num(), argument);								//DEBUG
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool check_rejection_parallel(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores, SOLVERS solver_type)
{
	//float start_time = omp_get_wtime();																											//DEBUG
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

	//long mem_base = get_mem_usage();																										//DEBUG
	vector<uint32_t> active_args;
	for (int i = 1; i < framework.num_args + 1; i++) {
		active_args.push_back(i);
	}
	//printf("Memory space of initialized active arguments: %ld [kB]\n", get_mem_usage() - mem_base);										//DEBUG
	
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

#pragma omp parallel shared(argument, framework, active_args, isRejected, proof_extension)   //, num_tasks, num_tasks_max
#pragma omp single
	{
		//printf("Number of threads: %d\n", omp_get_num_threads());																				//DEBUG

		//printf("%d: ------- started omp parallel -- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());								//DEBUG

		list<uint32_t> extension_build;

		check_rejection_parallel_recursiv(argument, framework, active_args, isRejected, extension_build, proof_extension, solver_type);					// , num_tasks, num_tasks_max
	}
	
	bool result = *isRejected;
	free(isRejected);
	active_args.clear();

	/*printf("Finished program - voluntary context switches: %ld - involuntary context switches: %ld - memory usage: %ld [kB]\n",
		get_ctxt_switches_volun(), get_ctxt_switches_involun(), get_mem_usage());*/																//DEBUG
	//float end_time = omp_get_wtime();																											//DEBUG
	//float duration = end_time - start_time;																									//DEBUG
	//printf("runtime check_rejection_parallel [s]: %.2f s\n", duration);																		//DEBUG

	return result;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores, SOLVERS solver_type) {
	pre_proc_result result_preProcessor = PreProc_DS_PR::process(framework, argument);

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return !check_rejection_parallel(argument, framework, proof_extension, numCores, solver_type);

		default:
			return unknown;
	}
}