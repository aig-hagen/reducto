//#include "../include/ScepticalPRParallel.h"
//using namespace std;
//
//void static appendList(omp_lock_t *lock_list_reducts_tail, nodeActives_t *listReducts, activeArgs_t *reduct, uint32_t *&len_list_reducts)
//{
//	omp_set_lock(lock_list_reducts_tail);
//	if (listReducts->element == NULL)
//	{
//		*listReducts = *create_lst_active_args(reduct);
//		*len_list_reducts++;
//	}
//	else
//	{
//		add_end_lst_active_args(listReducts, reduct);
//		*len_list_reducts++;
//	}
//	omp_unset_lock(lock_list_reducts_tail);
//}
//
///*===========================================================================================================================================================*/
///*===========================================================================================================================================================*/
//
//// generate initial sets in this state and check if the arguments sceptical acceptance is already rejected by one of them
//bool static generate_initial_sets_in_state_parallel(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs,
//	SATProblem_t *problem, nodeActives_t *listReducts, omp_lock_t *lock_waiting, omp_lock_t *lock_list_reducts_head, omp_lock_t *lock_list_reducts_tail,
//	uint32_t *&len_list_reducts)
//{
//
//	uint8_t flag_has_initial_sets = EXIT_SUCCESS;
//
//	do
//	{
//		//do for all initial sets in the current state of the framework 		
//		arrayInt_t *solution = create_array(0);
//		flag_has_initial_sets = InitialSetSolver::calculate_next_solution(framework, activeArgs, problem, solution);
//		if (flag_has_initial_sets == EXIT_FAILURE)
//		{
//			break;
//		}
//		nodeUInt32_t *initial_set = get_set_from_solution(solution, activeArgs);
//
//		if (check_rejection(argument, initial_set, framework))
//		{
//			//sceptical acceptance is rejected -> abort all calculations
//			return true;
//		}
//
//		//only add reduct to list, if following the path of the initial set is useful
//		if (!check_terminate_extension_build(argument, initial_set))
//		{
//			activeArgs_t *reduct = get_reduct_set(activeArgs, framework, initial_set);
//			if (reduct->numberActiveArguments < 2)
//			{
//				// The only active element has to be the specified argument, therefore there will be no new information gained by analysing this state.
//				// Hence, he is not added to the list.
//				continue;
//			}
//
//			if (*len_list_reducts < 4)		//only lock head if the list is too short
//			{
//				omp_set_lock(lock_list_reducts_head);
//				appendList(lock_list_reducts_tail, listReducts, reduct, len_list_reducts);
//				omp_unset_lock(lock_list_reducts_head);
//			}
//			else
//			{
//				appendList(lock_list_reducts_tail, listReducts, reduct, len_list_reducts);
//			}
//			omp_unset_lock(lock_waiting);
//		}
//	} while (flag_has_initial_sets == EXIT_SUCCESS);
//	return false;
//}
//
///*===========================================================================================================================================================*/
///*===========================================================================================================================================================*/
//
//bool static check_list_empty(nodeActives_t *list_reducts)
//{
//	return list_reducts->element == NULL;
//}
//
///*===========================================================================================================================================================*/
///*===========================================================================================================================================================*/
//
//bool ScepticalPRParallel::check_rejection_parallel(uint32_t argument, argFramework_t *framework, activeArgs_t *activeArgs)
//{
//	nodeActives_t *list_reducts = create_empty_lst_active_args();
//	uint32_t *len_list_reducts = NULL;
//	len_list_reducts = (uint32_t *)malloc(sizeof * len_list_reducts);
//	if (len_list_reducts == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	*len_list_reducts = 0;
//
//	omp_lock_t *lock_list_reducts_head = NULL;
//	lock_list_reducts_head = (omp_lock_t *)malloc(sizeof * lock_list_reducts_head);
//	if (lock_list_reducts_head == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	omp_init_lock(lock_list_reducts_head);
//
//	omp_lock_t *lock_list_reducts_tail = NULL;
//	lock_list_reducts_tail = (omp_lock_t *)malloc(sizeof * lock_list_reducts_tail);
//	if (lock_list_reducts_tail == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	omp_init_lock(lock_list_reducts_tail);
//
//	uint32_t number_working = 0;
//
//	omp_lock_t *lock_num_working = NULL;
//	lock_num_working = (omp_lock_t *)malloc(sizeof * lock_num_working);
//	if (lock_num_working == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	omp_init_lock(lock_num_working);
//
//	omp_lock_t *lock_num_working_init = NULL;
//	lock_num_working_init = (omp_lock_t *)malloc(sizeof * lock_num_working_init);
//	if (lock_num_working_init == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	omp_init_lock(lock_num_working_init);
//
//	omp_lock_t *lock_waiting = NULL;
//	lock_waiting = (omp_lock_t *)malloc(sizeof * lock_waiting);
//	if (lock_waiting == NULL) {
//		printf("Memory allocation failed\n");
//		exit(1);
//	}
//	omp_init_lock(lock_waiting);
//
//	omp_set_lock(lock_num_working_init); //master takes lock to stop fast running threads
//
//#pragma omp parallel
//	{
//		//printf("Thread %d", omp_get_thread_num());												//DEBUG
//#pragma omp master nowait
//		{
//			omp_set_lock(lock_num_working);
//			number_working += 1;
//			omp_unset_lock(lock_num_working);
//			omp_unset_lock(lock_num_working_init);
//			//analyse the initial state
//			SATProblem_t *problem = create_sat_problem(activeArgs->numberActiveArguments);
//			add_clauses_nonempty_admissible_set(problem, framework, activeArgs);
//			if (generate_initial_sets_in_state_parallel(argument, framework, activeArgs, problem, list_reducts,
//				lock_waiting, lock_list_reducts_head, lock_list_reducts_tail, len_list_reducts))
//			{
//				return true;
//			}
//			omp_set_lock(lock_num_working);
//			number_working -= 1;
//			omp_unset_lock(lock_num_working);
//		}
//
//		omp_set_lock(lock_num_working_init); //prevents threads from running in finishing condition
//		omp_unset_lock(lock_num_working_init); //opens lock for other threads to follow
//
//		//iterate through all states, that are added along the way
//		while (true)
//		{
//			//check if reducts in list
//			omp_set_lock(lock_list_reducts_head);	// [thread safety] check shared list
//			bool list_is_empty = check_list_empty(list_reducts);
//			omp_unset_lock(lock_list_reducts_head);
//			if (list_is_empty && number_working == 0)  //[termination condition]
//			{
//				// all threads have finished
//				omp_unset_lock(lock_waiting); //release next thread
//				break;
//			}
//			else if (list_is_empty)
//			{
//				omp_set_lock(lock_waiting);  //einziges Lock wo eine PASSIVE wait policy sinn machen würde
//
//				//update bool list_is_emtpy
//				omp_set_lock(lock_list_reducts_head);	//[thread safety] check shared list
//				list_is_empty = check_list_empty(list_reducts);
//				omp_unset_lock(lock_list_reducts_head);
//
//				if (list_is_empty)  //[termination condition]
//				{
//					// thread was freed but list is still empty, therefore finish
//					omp_unset_lock(lock_waiting);
//					break;
//				}
//			}
//			activeArgs_t *currentState = NULL;
//
//			//get top reduct
//			omp_set_lock(lock_list_reducts_head); // [thread safety] manipulate shared list
//			currentState = list_reducts->element;
//			if (*len_list_reducts < 4)
//			{
//				omp_set_lock(lock_list_reducts_tail);
//				list_reducts = remove_top_lst_active_args(list_reducts);
//				omp_unset_lock(lock_list_reducts_tail);
//			}
//			else
//			{
//				list_reducts = remove_top_lst_active_args(list_reducts);
//			}
//			omp_unset_lock(lock_list_reducts_head);
//
//			SATProblem_t *newProblem = create_sat_problem(currentState->numberActiveArguments);
//			add_clauses_nonempty_admissible_set(newProblem, framework, currentState);
//
//			//signal that this thread is working on new sets
//			omp_set_lock(lock_num_working);	//[thread safety] manipulate shared variable
//			number_working += 1;
//			omp_unset_lock(lock_num_working);
//			//analyse chosen reduct
//			if (generate_initial_sets_in_state_parallel(argument, framework, currentState, newProblem, list_reducts,
//				lock_waiting, lock_list_reducts_head, lock_list_reducts_tail, len_list_reducts))
//			{
//				return true;
//			}
//			//signal that this thread is no longer working on new sets
//			omp_set_lock(lock_num_working); //[thread safety] manipulate shared variable
//			number_working -= 1;
//			omp_unset_lock(lock_num_working);
//		}
//		omp_destroy_lock(lock_waiting);
//		omp_destroy_lock(lock_num_working);
//		omp_destroy_lock(lock_num_working_init);
//		omp_destroy_lock(lock_list_reducts_head);
//		omp_destroy_lock(lock_list_reducts_tail);
//	}
//	return false;
//}
//
//
