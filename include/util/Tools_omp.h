#ifndef TOOLS_OMP_H
#define TOOLS_OMP_H

#include<cstdint>

#include "../logic/PriorityStackManager.h"

namespace tools {

	/// <summary>
	/// This class is responsible for offering helping methods for lists.
	/// </summary>
	class ToolsOMP {

	public:
		static bool check_finished(bool &is_finished, PriorityStackManager &prio_stack);

		static bool check_termination(bool &is_terminated);

		static void update_is_finished(bool &is_terminated, bool &is_finished, PriorityStackManager &prio_stack);

		static void set_is_rejected(bool &is_rejected, bool &is_terminated);
	};
};
#endif