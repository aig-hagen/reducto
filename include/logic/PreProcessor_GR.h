#ifndef PREPROC_GR_H
#define PREPROC_GR_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "Reduct.h"
#include "ConeOfInfluence.h"

#include "omp.h"

#include "../util/ArrayBitSet.h"
#include "../logic/Enums.h"

extern "C" {
#include "../util/MemoryWatchDog.h"
}



/// <summary>
/// This class is responsible for preparing data, before the data is used in the actual process of solving the problem.
/// The preprocessor checks if the problem can be quicker solved by the use of some heuristics, without actually
/// calculating a solution.
/// </summary>
class PreProc_GR {
public:

	/// <summary>
	/// Calculates the cone of influence of a specified argumentation framework, starting with a specified argument of that framework.
	/// </summary>
	/// <param name="framework">The abstract argumentation framework, in which the cone if situated</param>
	/// <param name="query">The starting argument of the cone of influence</param>
	/// <param name="out_coi">[Output-Parameter] Object containing information about the distances of other arguments to the starting argument in the cone of influence.</param>
	/// <returns>A reduct of the framework, setting only those argument as 'active', which are part of the cone of influence.</returns>
	static ArrayBitSet calculate_cone_influence(AF &framework, uint32_t query, ConeOfInfluence &out_coi);

	/// <summary>
	/// Calculates the cone of influence of a specified reduced state of a specified argumentation framework, starting with a specified argument of that framework.
	/// </summary>
	/// <param name="framework">The abstract argumentation framework, in which the cone if situated</param>
	/// <param name="reduct">The current state of the framework, which means the active arguments of the framework in the current state. </param>
	/// <returns>A reduct of the framework, setting only those argument as 'active', which are part of the cone of influence.</returns>
	static ArrayBitSet calculate_cone_influence_reduct(AF &framework, ArrayBitSet reduct, uint32_t query);

	/// <summary>
	/// This method prepares the data before it is being processed by the solver and checks if a solution can be easily drawn
	/// by the help of some properties.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="query">The query argument, whose acceptance is to be checked.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.</param>
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.</param>
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor if finished.</param>
	/// <param name="out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <param name="out_coi">[Output-Parameter] Object containing information about the distances of other arguments to the starting argument in the cone of influence.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result process(AF &framework, uint32_t query, bool break_acception, bool break_rejection,
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension, ConeOfInfluence &out_coi);

	/// <summary>
	/// This method prepares the data before it is being processed by the solver and checks if a solution can be easily drawn
	/// by reducing the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns the set of active arguments, which remain of the original framework after the preprocessor is finished.</returns>
	static ArrayBitSet process_only_grounded(AF &framework, list<uint32_t> &out_gr_extension);

	/// <summary>
	/// This method prepares the data before it is being processed by the solver and checks if a solution can be easily drawn
	/// by reducing the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="query">The query argument, whose acceptance is to be checked.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.</param>
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.</param>
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor is finished.</param>
	/// <param name="out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result process_only_grounded(AF &framework, uint32_t query, bool break_acception, bool break_rejection,
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension);

	/// <summary>
	/// This method checks if a solution can be easily drawn by reducing the current state of the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="active_args">Current active arguments in the framework.</param>
	/// <param name="query">The query argument, whose acceptance is to be checked.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.</param>
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.</param>
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor is finished.</param>
	/// <param name="out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result reduce_by_grounded(AF &framework, ArrayBitSet &active_args, uint32_t query, bool break_acception, bool break_rejection,
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension);
};
#endif