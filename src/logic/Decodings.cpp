#include "../../include/logic/Decodings.h"

using namespace std;
using std::vector;

list<uint32_t> Decoding::get_set_from_solver(SatSolver &solver, unordered_set<uint32_t> &activeArgs)
{
	list<uint32_t> output;

	for (size_t bno = 0; bno < activeArgs.bucket_count(); ++bno) {
		for (auto bit = activeArgs.begin(bno), end = activeArgs.end(bno); bit != end; ++bit) {
			const auto &element = *bit;
			
			if (solver.check_var_model(element))
			{
				output.push_back(element);
			}
		}
	}

	return output;
}