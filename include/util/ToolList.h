#ifndef TOOLS_LIST_H
#define TOOLS_LIST_H

#include<cstdint>
#include<list>

namespace tools {

	/// <summary>
	/// This class is responsible for offering helping methods for lists.
	/// </summary>
	class ToolList {

	public:
		/// <summary>
		/// This method copies the specified lists in a newly created third list, which starts with the elements from ListA.
		/// </summary>
		/// <param name="listA">List of element to be in the front of the list to create.</param>
		/// <param name="ListB">List of element to be in the back of the list to create.</param>
		/// <returns>Newly created list hold copies of the specified input lists.</returns>
		static std::list<uint32_t> extend_list(std::list<uint32_t> &listA, std::list<uint32_t> &listB);
	};
};
#endif