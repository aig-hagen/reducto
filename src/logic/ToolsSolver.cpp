#include "../../include/logic/ToolsSolver.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Tools_Solver::UpdateCertificate(SatSolver *solver, ArrayBitSet &active_args, std::__cxx11::list<uint32_t> &certificate_extension)
{
	list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, active_args);
	list<uint32_t> new_proof = tools::ToolList::extend_list(certificate_extension, calculated_extension);
	certificate_extension.clear();
	calculated_extension.clear();
	certificate_extension = new_proof;
}