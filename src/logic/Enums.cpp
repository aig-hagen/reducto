#include "../../include/logic/Enums.h"


task Enums::string_to_task(string task)
{
	if (task == "DC") return DC;
	if (task == "DS") return DS;
	if (task == "SE") return SE;
	return UNKNOWN_TASK;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

semantics Enums::string_to_sem(string semantics)
{
	if (semantics == "PR") return PR;
	if (semantics == "CO") return CO;
	if (semantics == "ST") return ST;
	return UNKNOWN_SEM;
}