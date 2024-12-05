#include "../../include/util/ToolList.h"

std::list<uint32_t> tools::ToolList::extend_list(std::list<uint32_t> &listA, std::list<uint32_t> &listB)
{
	std::list<uint32_t> tmpCopy_1, tmpCopy_2;
	std::copy(listA.begin(), listA.end(), std::back_inserter(tmpCopy_1));
	std::copy(listB.begin(), listB.end(), std::back_inserter(tmpCopy_2));
	tmpCopy_1.merge(tmpCopy_2);
	return tmpCopy_1;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void tools::ToolList::copy_in_list(std::list<uint32_t> &list_dest, std::vector<uint32_t> vector_source) {
	std::list<uint32_t>::iterator it = list_dest.begin();
	list_dest.insert(it, vector_source.begin(), vector_source.end());
}