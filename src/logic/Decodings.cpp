#include "../../include/logic/Decodings.h"

using namespace std;
using std::vector;

list<uint32_t> Decoding::get_set_from_solver(SatSolver &solver, vector<uint32_t> &activeArgs)
{
	vector<bool> model = solver.get_model();
	list<uint32_t> output;

	for (uint32_t i = 0; i < activeArgs.size(); i++) {

		if (model[i] == true)
		{
			output.push_back(activeArgs[i]);
		}
	}

	return output;
}