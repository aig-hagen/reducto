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
	activeArgs_t *reduct = activeArguments;
	nodeUInt32_t *currentArgument = set_arguments;
	while (currentArgument != NULL)
	{
		reduct = get_reduct(reduct, framework, currentArgument->number);

		currentArgument = currentArgument->next;
	}

	return reduct;
}
