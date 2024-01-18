#include "../include/Decodings.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint32_t get_argument(int64_t label_variable, activeArgs_t *activeArgs)
{
	return activeArgs->encodingToArgument->elements[label_variable];
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeUInt32_t *get_set_from_problem(SATProblem_t *problem, activeArgs_t *activeArgs)
{
	//printf("\n");														//DEBUG
	//print_array(problem->solution);										//DEBUG
	//printf("\n");														//DEBUG
	return get_set_from_solution(problem->solution, activeArgs);
}

//nodeUInt32_t *get_set_from_problem(SATProblem_t *problem, activeArgs_t *activeArgs)
//{
//	nodeUInt32_t *output = NULL;
//	for (uint32_t i = 1; i < problem->numberOfVariables + 1; i++)
//	{
//		if (problem->solution->elements[i] == 1)
//		{
//			uint32_t currentArgument = get_argument(i, activeArgs);
//			if (output == NULL)
//			{
//				output = create_list_uint32(currentArgument);
//			}
//			else
//			{
//				push_uint32(output, currentArgument);
//			}
//		}
//	}
//	//printf("\n");														//DEBUG
//	//print_array(problem->solution);										//DEBUG
//	//printf("\n");														//DEBUG
//	return output;
//}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeUInt32_t *get_set_from_solution(arrayInt_t *solution, activeArgs_t *activeArgs)
{
	nodeUInt32_t *output = NULL;
	for (uint32_t i = 1; i < (solution->length / 2) + 1; i++)
	{
		if (solution->elements[i] == 1)
		{
			uint32_t currentArgument = get_argument(i, activeArgs);
			if (output == NULL)
			{
				output = create_list_uint32(currentArgument);
			}
			else
			{
				push_uint32(output, currentArgument);
			}
		}
	}

	return output;
}