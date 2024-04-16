#include "../../include/logic/Decodings_SatSolver.h"

using namespace std;
using std::vector;

nodeUInt32_t *Decoding_SatSolver::get_set_from_solver(SatSolver *solver, activeArgs_t *activeArgs) 
{
	nodeUInt32_t *output = NULL;
	for (uint32_t i = 0; i < activeArgs->numberActiveArguments; i++) {

		if ((*solver).get_model()[i] == true)
		{
			uint32_t currentArgument = get_argument(static_cast<int64_t>(i) + 1, activeArgs);
			if (output == NULL)
			{
				output = create_list_uint32(currentArgument);
			}
			else
			{
				push_uint32(output, currentArgument);
			}
		}
	}

	return output;
}