#include "../../include/logic/ToolsArgsSet.h"

bool tools::Tools_ArgsSet::check_attack(std::uint32_t argument, std::list<std::uint32_t> &set_arguments, AF &framework)
{
	//iterate through arguments of the set
	for (list<uint32_t>::iterator mIter = set_arguments.begin(); mIter != set_arguments.end(); ++mIter) {
		//check if query argument is victim of the set
		if(framework.exists_attack(*mIter, argument)){
			return true;
		}
	}

	return false;
}