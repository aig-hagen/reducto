#include "../../include/logic/Solver_DS_PR.h"
using namespace std;

static list<uint32_t> ExtendExtension(list<uint32_t> &extension_build, list<uint32_t> &initial_set)
{
	list<uint32_t> tmpCopy_1, tmpCopy_2;
	std::copy(extension_build.begin(), extension_build.end(), std::back_inserter(tmpCopy_1));
	std::copy(initial_set.begin(), initial_set.end(), std::back_inserter(tmpCopy_2));
	tmpCopy_1.merge(tmpCopy_2);
	return tmpCopy_1;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void check_rejection_parallel_recursiv(uint32_t argument, AF &framework, ArrayBitSet &activeArgs, bool *isRejected,
	list<uint32_t> &extension_build, list<uint32_t> &output_extension)
{
	int id = omp_get_thread_num();
	bool isRejected_tmp = false;

#pragma omp flush(isRejected)
#pragma omp atomic read
	isRejected_tmp = *isRejected;
	if (isRejected_tmp == true)
	{
		return;
	}

	ArrayBitSet reduct = extension_build.empty() ? activeArgs.copy() : Reduct::get_reduct_set(activeArgs, framework, extension_build);

	if (reduct._array.size() < 2)
	{
		// there is only 1 active argument, this has to be the argument to check, 
		// if not then there should have been a rejection check earlier who did not work
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
	uint64_t numVars = reduct._array.size();
	SatSolver *solver = NULL;
	solver = new SatSolver_cadical(numVars);
	Encoding::add_clauses_nonempty_admissible_set(*solver, framework, reduct);

#pragma omp flush(isRejected)
#pragma omp atomic read
	isRejected_tmp = *isRejected;
	if (isRejected_tmp == true)
	{
		free(isSolved);
		free(isFirstCalculation);
		delete solver;
		return;
	}

	bool has_Solution = true;

	//iterate through initial sets
	do {
		if (*isSolved)
		{
			Encoding::add_complement_clause(*solver, reduct);
		}

		*isSolved = true;
		has_Solution = (*solver).solve();

		if (!has_Solution)
		{
			//no more initial sets to calculate after this one
			if (*isFirstCalculation)
			{
				// since this is the first calculation and no IS was found, this reduct has only the empty set as an admissible set, 
				// therefore the extension_build is complete and since only extensions not containing the query argument proceed 
				// in the calculation, extension_build cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected				
#pragma atomic write
				*isRejected = true;
#pragma omp flush(isRejected)

				free(isSolved);
				free(isFirstCalculation);
				delete solver;
				return;
			}

			break;
		}

		list<uint32_t> initial_set = Decoding::get_set_from_solver(*solver, reduct);		
		if (initial_set.empty())
		{
			if (*isFirstCalculation)
			{
				// since this is the first calculation and only the empty set as IS was found, the extension_build is complete
				// and since only extensions not containing the query argument proceed in the calculation, extension_build 
				// cannot contain the query argument, so that there exists
				// an extension not containing the query argument, so that the argument gets sceptical rejected				
#pragma atomic write
				*isRejected = true;
#pragma omp flush(isRejected)
				free(isSolved);
				free(isFirstCalculation);
				delete solver;
				initial_set.clear();
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
#pragma omp flush(isRejected)

			list<uint32_t> new_extension_build = ExtendExtension(extension_build, initial_set);
#pragma atomic write
			output_extension = new_extension_build;

			free(isSolved);
			free(isFirstCalculation);
			delete solver;
			initial_set.clear();
			return;
		}
		else if (ScepticalCheck::check_terminate_extension_build(argument, initial_set))
		{
			initial_set.clear();
			continue;
		}

		list<uint32_t> new_extension_build = ExtendExtension(extension_build, initial_set);		
		initial_set.clear();
		uint32_t prio = Prioritizer::calculate_priority(framework, new_extension_build);
#pragma omp task \
	firstprivate(new_extension_build) \
	private(reduct, initial_set, solver) \
	shared(argument, framework, activeArgs, isRejected, output_extension) \
	priority(prio)
		{
			check_rejection_parallel_recursiv(argument, framework, activeArgs, isRejected, new_extension_build, output_extension);
			new_extension_build.clear();																												
		}
		new_extension_build.clear();

#pragma omp flush(isRejected)
#pragma omp atomic read
		isRejected_tmp = *isRejected;
	} while (has_Solution && !isRejected_tmp);

	free(isSolved);
	free(isFirstCalculation);
	delete solver;
	return;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static bool check_rejection_parallel(uint32_t argument, AF &framework, ArrayBitSet &active_args, list<uint32_t> &proof_extension, uint8_t numCores)
{
	bool *isRejected = NULL;
	isRejected = (bool *)malloc(sizeof *isRejected);
	if (isRejected == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	*isRejected = false;

	if (numCores > 0)
	{
		omp_set_num_threads(numCores);
	}

#pragma omp parallel shared(argument, framework, active_args, isRejected, proof_extension)
#pragma omp single
	{
		list<uint32_t> extension_build;
		check_rejection_parallel_recursiv(argument, framework, active_args, isRejected, extension_build, proof_extension);
	}
	
	bool result = *isRejected;
	free(isRejected);
	return result;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool Solver_DS_PR::solve(uint32_t argument, AF &framework, list<uint32_t> &proof_extension, uint8_t numCores) 
{
	ArrayBitSet initial_reduct = ArrayBitSet();
	pre_proc_result result_preProcessor = PreProc_DS_PR::process(framework, argument, initial_reduct);

	switch (result_preProcessor){

		case accepted:
			return true;

		case rejected:
			return false;

		case unknown:
			return !check_rejection_parallel(argument, framework, initial_reduct, proof_extension, numCores);

		default:
			return unknown;
	}
}