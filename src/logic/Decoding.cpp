#include "../../include/logic/Decoding.h"

using namespace std;
using std::vector;

list<uint32_t> Decoding::get_set_from_solver(SatSolver &solver, ArrayBitSet &activeArgs)
{
	list<uint32_t> output;

	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		if (solver.check_var_model(activeArgs._array[i]))
		{
			output.push_back(activeArgs._array[i]);
		}
	}

	return output;
}