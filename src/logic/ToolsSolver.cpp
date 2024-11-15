#include "../../include/logic/ToolsSolver.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Tools_Solver::UpdateCertificate(SatSolver *solver, ArrayBitSet &active_args, std::__cxx11::list<uint32_t> &certificate_extension)
{
	list<uint32_t> calculated_extension = Decoding::get_set_from_solver(*solver, active_args);
	Tools_Solver::UpdateCertificate(certificate_extension, calculated_extension);
	calculated_extension.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Tools_Solver::UpdateCertificate(std::__cxx11::list<uint32_t> &certificate_extension, std::__cxx11::list<uint32_t> &set_to_extend_with)
{
	{
		list<uint32_t> new_certificate = tools::ToolList::extend_list(certificate_extension, set_to_extend_with);
		certificate_extension.clear();
		certificate_extension = new_certificate;
	}
}