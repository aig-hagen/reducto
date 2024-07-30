#ifndef PREPROC_DS_PR_H
#define PREPROC_DS_PR_H

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
/// Return values of the preprocessor, indicating if the preprocessor solved the problem, and if so to what conclusion
/// </summary>
enum pre_proc_result { accepted, rejected, unknown };

/// <summary>
/// This class is responsible for preparing data, before the data is used in the actual process of solving the problem.
/// The preprocessor checks if the problem can be trivially solved by the use of some properties, without actually
/// calculating a solution.
/// </summary>
class PreProc_DS_PR {
public:
	/// <summary>
	/// This method prepares the data before it being processed by the solver and checks if a solution can be easily drawn
	/// by the help of some properties.
	/// </summary>
	/// <param name="framework">The original abstract argumentation framework of the situation.</param>
	/// <param name="argument">The query argument, whose sceptical acceptance is to check.</param>
	/// <param name="out_reduct">[Output-Parameter]The set of active arguments, which remain of the original framework after the preprocessor if finished.
	/// Keep in mind, that the value of this parameter will be overwritten during this method, as it's an output-parameter.</param>
	/// <returns>Returns an Enum indicating if the method has solved the problem, and if so to which conclusion it came.</returns>
	static pre_proc_result process(AF &framework, uint32_t argument, ArrayBitSet &out_reduct);
};
#endif