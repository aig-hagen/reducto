#ifndef REDUCT_H
#define REDUCT_H

#include <stdint.h>
#include "AF.h"
#include "Actives.h"

/// <summary>
/// Reduces the specified framework by the specified argument, using the specified set of active arguments.
/// </summary>
/// <param name="activeArguments">Set of arguments, which are active in the current transition state.</param>
/// <param name="framework">The framework to reduce.</param>
/// <param name="argument">The argument, by which the framework will be reduced.</param>
/// <returns>Set of active arguments in the new transition state after the reduction.</returns>
activeArgs_t* get_reduct(activeArgs_t *activeArguments, argFramework_t *framework, uint32_t argument);

/// <summary>
/// Reduces the specified framework by the specified set of arguments, using the specified set of active arguments.
/// </summary>
/// <param name="activeArguments">Set of arguments, which are active in the current transition state.</param>
/// <param name="framework">The framework to reduce.</param>
/// <param name="set_arguments">The set of argument, by which the framework will be reduced.</param>
/// <returns>Set of active arguments in the new transition state after the reduction.</returns>
activeArgs_t *get_reduct_set(activeArgs_t *activeArguments, argFramework_t *framework, nodeUInt32_t *set_arguments);

#endif
