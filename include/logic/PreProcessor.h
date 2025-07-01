#ifndef PREPROC_H
#define PREPROC_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "Reduct.h"
#include "ConeOfInfluence.h"

#include "../util/ArrayBitSet.h"

extern "C" {
#include "../util/MemoryWatchDog.h"
}

/// <summary>
/// This class is responsible for preparing data, before the data is used in the actual process of solving the problem.
/// The preprocessor checks if the problem can simplified by replacing or removing parts of the argumentation framework.
/// </summary>
class PreProcessor {
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
};
#endif


#ifdef DO_PREPROC
#else
#endif