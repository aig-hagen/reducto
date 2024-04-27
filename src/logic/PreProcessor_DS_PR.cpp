#include "../../include/logic/PreProcessor_DS_PR.h"

pre_proc_result PreProc_DS_PR::process(AF &framework, uint32_t argument) {
	
	if (framework.victims[argument].count(argument))
	{
		return pre_proc_result::rejected;
	}

	return unknown;
}
