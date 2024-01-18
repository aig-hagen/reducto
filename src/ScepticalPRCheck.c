#include "../include/ScepticalPRCheck.h"

bool check_rejection(uint32_t argument, nodeUInt32_t *initial_set, argFramework_t *framework)
{
	nodeUInt32_t *currentArgument = initial_set;
	//iterate through arguments in the initial set
	while (currentArgument != NULL)
	{
		uint32_t currentVictim = get_first_in_row(framework->victims, currentArgument->number);
		if (currentVictim != 0) {

			if (currentVictim == argument)
			{
				return true;
			}

			//iterate through victims
			while (has_next_in_row(framework->victims, currentArgument->number, currentVictim)) {
				currentVictim = get_next_in_row(framework->victims, currentArgument->number, currentVictim);
				
				if (currentVictim == argument)
				{
					return true;
				}
			}
		}

		currentArgument = currentArgument->next;
	}

	return false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool check_terminate_extension_build(uint32_t argument, nodeUInt32_t *initial_set)
{
	nodeUInt32_t *currentArgument = initial_set;
	//iterate through arguments in the initial set
	while (currentArgument != NULL)
	{
		if (currentArgument->number == argument)
		{
			return true;
		}

		currentArgument = currentArgument->next;
	}

	return false;
}