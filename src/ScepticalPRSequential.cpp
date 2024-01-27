#include "../include/ScepticalPRSequential.h"
using namespace std;

bool ScepticalPRSequential::check_rejection_per_reduct_recursiv(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs)
{
	if (activeArgs->numberActiveArguments < 2)
	{
		//printf("\n==================================================\n");					//DEBUG
		//print_active_arguments(activeArgs);													//DEBUG
		//printf("\n");																		//DEBUG
		//printf(" !! Only one active argument left: %d", get_first_active(activeArgs));		//DEBUG

		//there is only 1 active argument, this has to be the argument to check, if not then there should have been a rejection check earlier who did not work
		if (get_first_active(activeArgs) != argument)
		{
			printf("ERROR: argument was removed from state, without rejection_check noticing");
			exit(1);
		}


		//printf("\n --- FALSE --- \n");														//DEBUG
		return false;
	}


	SATProblem_t *problem = create_sat_problem(activeArgs->numberActiveArguments);
	add_clauses_nonempty_admissible_set(problem, framework, activeArgs);
	uint8_t flag_exit = EXIT_SUCCESS;

	//iterate through initial sets
	do {
		//printf("\n==================================================\n");					//DEBUG
		//print_active_arguments(activeArgs);													//DEBUG
		//printf("\n");																		//DEBUG

		flag_exit = InitialSetSolver::calculate_next_solution(framework, activeArgs, problem);
		nodeUInt32_t *initial_set = get_set_from_solution(problem->solution, activeArgs);
		if (flag_exit == EXIT_FAILURE)
		{
			//no initial set calculated
			break;
		}
		if (check_rejection(argument, initial_set, framework))
		{
			//printf("\n --- TRUE --- \n");													//DEBUG
			return true;
		}

		if (check_terminate_extension_build(argument, initial_set))
		{
			//printf("\n Terminate path, since argument is in initial set \n");				//DEBUG
			continue;
		}

		//printf("Reduct by ");																//DEBUG
		//print_list_uint32(initial_set);														//DEBUG
		//printf("\n");																		//DEBUG
		activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
		free_list_uint32(initial_set);
		//print_active_arguments(reduct);														//DEBUG
		if (check_rejection_per_reduct_recursiv(argument, framework, reduct))
		{
			//printf("\n --- TRUE --- \n");													//DEBUG
			return true;
		}
		free_activeArguments(reduct);

	} while (flag_exit != EXIT_FAILURE);

	//printf("\n --- FALSE --- \n");															//DEBUG

	free_problem(problem);
	return false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

// generate initial sets in this state and check if the arguments sceptical acceptance is already rejected by one of them
bool static generate_initial_sets_in_state(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs,
	SATProblem_t *problem, nodeActives_t *listReducts)
{
	uint8_t flag_has_initial_sets = EXIT_SUCCESS;
	while (true) {
		//do for all initial sets in the current state of the framework		
		

		flag_has_initial_sets = InitialSetSolver::calculate_next_solution(framework, activeArgs, problem);

		if (flag_has_initial_sets == EXIT_FAILURE)
		{
			return false;
		}

		nodeUInt32_t *initial_set = get_set_from_solution(problem->solution, activeArgs);
		

		if (check_rejection(argument, initial_set, framework))
		{
			//sceptical acceptance is rejected -> abort all calculations
			return true;
		}

		//only add reduct to list, if following the path of the initial set is useful
		if (!check_terminate_extension_build(argument, initial_set))
		{
			activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
			free_list_uint32(initial_set);

			//printf("================================================================\n");				//DEBUG
			//printf("origin: \n");																		//DEBUG
			//print_active_arguments(activeArgs);															//DEBUG
			//printf("\n");																				//DEBUG
			//printf("calculated solution: ");															//DEBUG
			//print_array(problem->solution);																//DEBUG
			//printf("\n");																				//DEBUG
			//printf("initial set: ");																	//DEBUG
			//print_list_uint32(initial_set);																//DEBUG
			//printf("\n");																				//DEBUG
			//printf("reduct: \n");																		//DEBUG
			//print_active_arguments(reduct);																//DEBUG
			//printf("\n");																				//DEBUG
			//printf("================================================================\n");				//DEBUG

			if (reduct->numberActiveArguments < 2)
			{
				// The only active element has to be the specified argument, therefore there will be no new information gained by analysing this state.
				// Hence, he is not added to the list.
				continue;
			}
			if (listReducts->element == NULL)
			{
				*listReducts = *create_lst_active_args(reduct);
			}
			else
			{
				add_end_lst_active_args(listReducts, reduct);
			}
		}
	};
}

bool ScepticalPRSequential::check_rejection_iterative(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs)
{
	//analyse the initial state
	SATProblem_t *problem = create_sat_problem(activeArgs->numberActiveArguments);
	add_clauses_nonempty_admissible_set(problem, framework, activeArgs);
	nodeActives_t *listReducts = create_empty_lst_active_args();
	if (generate_initial_sets_in_state(argument, framework, activeArgs, problem, listReducts))
	{
		return true;
	}
	free_problem(problem);

	//iterate through all possible following states
	while (listReducts->element != NULL)
	{
		activeArgs_t *currentState = listReducts->element;
		listReducts = remove_top_lst_active_args(listReducts);											

		SATProblem_t *newProblem = create_sat_problem(currentState->numberActiveArguments);
		add_clauses_nonempty_admissible_set(newProblem, framework, currentState);
		if (generate_initial_sets_in_state(argument, framework, currentState, newProblem, listReducts))
		{
			return true;
		}
		free_activeArguments(currentState);
		free_problem(newProblem);
	}

	free_lst_active_args(listReducts);
	return false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

