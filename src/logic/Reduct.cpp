#include "../../include/logic/Reduct.h"

ArrayBitSet Reduct::get_reduct(ArrayBitSet &activeArguments, AF &framework, uint32_t argument)
{
	vector<uint32_t> reduct_vector;
	vector<uint8_t> reduct_bitset;
	reduct_bitset.resize(static_cast<uint64_t>(framework.num_args) + 1);

	for (std::vector<unsigned int>::size_type i = 0; i < activeArguments._array.size(); i++) {

		if (activeArguments._array[i] == argument || framework.exists_attack(argument, activeArguments._array[i])){
			continue;
		}

		reduct_vector.push_back(activeArguments._array[i]);
		reduct_bitset[activeArguments._array[i]] = true;
	}

	return ArrayBitSet(reduct_vector, reduct_bitset);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

ArrayBitSet Reduct::get_reduct_set(ArrayBitSet &activeArguments, AF &framework, list<uint32_t> &set_arguments)
{
	if (set_arguments.empty()) {
		cout << "get_reduct_set:: set of arguments is emptyget_reduct_set" << endl;
		throw new exception;
	}

	// do not free activeArguments, since it's a given parameter, it's out of this method's responsibility
	ArrayBitSet reduct = get_reduct(activeArguments, framework, *set_arguments.begin());
	if (reduct._array.empty()) {
		return reduct;
	}

	for (list<uint32_t>::iterator mIter = std::next(set_arguments.begin()); mIter != set_arguments.end(); ++mIter) {
		if (reduct._array.empty())
		{
			break;
		}

		// free all temporary reducts
		ArrayBitSet reduct_tmp = get_reduct(reduct, framework, *mIter);
		reduct.clear();
		reduct = reduct_tmp;
	}

	return reduct;
}