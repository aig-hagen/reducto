#include "../../include/logic/Reduct.h"

activeArgs_t* get_reduct(activeArgs_t *activeArguments, argFramework_t *framework, uint32_t argument)
{
	activeArgs_t *newActives = copy_active_arguments(activeArguments);
	deactivate_argument(newActives, argument);
	uint32_t currentVictim = 0;
	if (!has_next_in_row(framework->victims, argument, currentVictim)){
		return newActives;
	}

	do {
		currentVictim = get_next_in_row(framework->victims, argument, currentVictim);
		if (is_active(newActives, currentVictim)){
			deactivate_argument(newActives, currentVictim);
		}
	} while (has_next_in_row(framework->victims, argument, currentVictim));

	return newActives;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

activeArgs_t *get_reduct_set(activeArgs_t *activeArguments, argFramework_t *framework, nodeUInt32_t *set_arguments)
{
	nodeUInt32_t *currentArgument = set_arguments;
	if (currentArgument == NULL)
	{
		return NULL;
	}

	//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());												//DEBUG
	// do not free activeArguments, since its a given parameter and hence out of this method's responsibility
	activeArgs_t *reduct = get_reduct(activeArguments, framework, currentArgument->number);
	//printf("%d: ------- after getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());													//DEBUG
	currentArgument = currentArgument->next;

	while (currentArgument != NULL)
	{
		if (reduct->numberActiveArguments == 0)
		{
			break;
		}

		// free all temporary reducts
		//printf("%d: ------- before getReduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());											//DEBUG
		activeArgs_t *tmp_reduct = reduct;
		reduct = get_reduct(tmp_reduct, framework, currentArgument->number);
		free_activeArguments(tmp_reduct);
		//printf("%d: ------- after getReduct + free tmp_Reduct --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());							//DEBUG
		currentArgument = currentArgument->next;
	}

	return reduct;
}
