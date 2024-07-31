#include "../../include/logic/ScepticalCheck.h"

bool ScepticalCheck::check_rejection(uint32_t query, list<uint32_t> &set_arguments, AF &framework)
{
	//iterate through arguments of the set
	for (list<uint32_t>::iterator mIter = set_arguments.begin(); mIter != set_arguments.end(); ++mIter) {
		//check if query argument is victim of the set
		if(framework.exists_attack(*mIter, query)){
			return true;
		}
	}

	return false;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool ScepticalCheck::check_terminate_extension_build(uint32_t query, list<uint32_t> &set_arguments)
{
	for (list<uint32_t>::iterator mIter = set_arguments.begin(); mIter != set_arguments.end(); ++mIter) {
		//check if query argument is member of set
		if (*mIter == query) {
			return true;
		}
	}

	return false;
}