#include "../../include/logic/Enums.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

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

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

format Enums::string_to_format(string format_str) {
	if (format_str == "i23") return I23;
	if (format_str == "tgf") return TGF;
	return UNKNOWN_FORMAT;
}