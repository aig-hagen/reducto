#ifndef PREPROC_GR_H
#define PREPROC_GR_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "Reduct.h"

#include "omp.h"

#include "../util/ArrayBitSet.h"
#include "../logic/Enums.h"

extern "C" {
	#include "../util/MemoryWatchDog.h"
}



/// <summary>
/// This class is responsible for preparing data, before the data is used in the actual process of solving the problem.
/// The preprocessor checks if the problem can be trivially solved by the use of some properties, without actually
/// calculating a solution.
/// </summary>
class PreProc_GR {
public:

	static ArrayBitSet calculate_cone_influence(AF &framework, uint32_t query);
	static ArrayBitSet calculate_cone_influence(AF &framework, ArrayBitSet reduct, uint32_t query, std::list<uint32_t> &list_remaining_args);

	/// <summary>
	/// This method prepares the data before it being processed by the solver and checks if a solution can be easily drawn
	/// by the help of some properties.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="query">The query argument, whose acceptance is to check.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor if finished.
	/// Keep in mind, that the value of this parameter will be overwritten during this method, as it's an output-parameter.</param>
	/// <param name"out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result process(AF &framework, uint32_t query, bool break_acception, bool break_rejection, 
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension);

	/// <summary>
	/// This method prepares the data before it being processed by the solver and checks if a solution can be easily drawn
	/// by reducing the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name"out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns the set of active arguments, which remain of the original framework after the preprocessor if finished.</returns>
	static ArrayBitSet process_only_grounded(AF &framework, list<uint32_t> &out_gr_extension);

	/// <summary>
	/// This method prepares the data before it being processed by the solver and checks if a solution can be easily drawn
	/// by reducing the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="query">The query argument, whose acceptance is to check.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor if finished.
	/// Keep in mind, that the value of this parameter will be overwritten during this method, as it's an output-parameter.</param>
	/// <param name"out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result process_only_grounded(AF &framework, uint32_t query, bool break_acception, bool break_rejection, 
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension);

	/// <summary>
	/// This method checks if a solution can be easily drawn by reducing the current state of the framework by the grounded extension.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="active_args">Current active arguments in the framework.</param>
	/// <param name="query">The query argument, whose acceptance is to check.</param>
	/// <param name="break_acception"> If TRUE, the preprocessing will abort the process, in case the grounded extension contains the query argument.
	/// <param name="break_rejection"> If TRUE, the preprocessing will abort the process, in case the grounded extension attacks the query argument.
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor if finished.
	/// Keep in mind, that the value of this parameter will be overwritten during this method, as it's an output-parameter.</param>
	/// <param name"out_gr_extension">[Output-Parameter] The calculated grounded extension of the framework.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result reduce_by_grounded(AF &framework, ArrayBitSet &active_args, uint32_t query,bool break_acception, bool break_rejection,
		ArrayBitSet &out_reduct, list<uint32_t> &out_gr_extension);
};
#endif