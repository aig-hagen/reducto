#ifndef REDUCT_H
#define REDUCT_H

#include <vector>
#include <unordered_set>
#include <cstdint>
#include <iterator>
#include <list>

#include "AF.h"

using namespace std;

	class Reduct {

		public:
			/// <summary>
			/// Reduces the specified framework by the specified argument, using the specified set of active arguments.
			/// </summary>
			/// <param name="activeArguments">Set of arguments, which are active in the current transition state.</param>
			/// <param name="framework">The framework to reduce.</param>
			/// <param name="argument">The argument, by which the framework will be reduced.</param>
			/// <returns>Set of active arguments in the new transition state after the reduction.</returns>
			static vector<uint32_t> get_reduct(vector<uint32_t> &activeArguments, AF &framework, uint32_t argument);

			/// <summary>
			/// Reduces the specified framework by the specified set of arguments, using the specified set of active arguments.
			/// </summary>
			/// <param name="activeArguments">Set of arguments, which are active in the current transition state.</param>
			/// <param name="framework">The framework to reduce.</param>
			/// <param name="set_arguments">The set of argument, by which the framework will be reduced.</param>
			/// <returns>Set of active arguments in the new transition state after the reduction.</returns>
			static vector<uint32_t> get_reduct_set(vector<uint32_t> &activeArguments, AF &framework, list<uint32_t> &set_arguments);
	};

#endif
