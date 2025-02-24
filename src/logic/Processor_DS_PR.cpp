#include "../../include/logic/Processor_DS_PR.h"
/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static std::__cxx11::list<uint32_t> get_set_from_solver(SatSolver &solver, ArrayBitSet &active_args, uint32_t query_argument, AF &framework, 
	bool &is_attacked)
{
	list<uint32_t> initial_set = Decoding::get_set_from_solver(solver, active_args);

	if (tools::Tools_ArgsSet::check_attack(query_argument, initial_set, framework)) {
		is_attacked = true;
	}

	return initial_set;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

list<uint32_t> Proc_DS_PR::calculate_rejecting_set(uint32_t query_argument, AF &framework, ArrayBitSet &active_args, bool &is_rejected, bool &is_extension_certificate,
	SatSolver &solver, bool &has_solution_without_query, bool is_first_iteration) {
	has_solution_without_query = solver.solve(Encoding::get_literal_accepted(query_argument, false));
	if (!has_solution_without_query) {
		//there is no nonempty adm. set, which is not containing the query, there might be solutions containing the query
		if (is_first_iteration && !solver.solve(Encoding::get_literal_accepted(query_argument, true))) {
			// this is the first iteration, so there have been no solution excluded by a complement clause
			// there is no nonempty adm. set, with or without the query argument, that's why there is only the empty set as adm. set
			// which means we found a complete extension, which is not containing the query argument, hence we found a counter evidence
			is_rejected = true;
			is_extension_certificate = true;
		}
		return list<uint32_t>();
	}
	else {
		bool is_attacking_query = false;
		list<uint32_t> calculated_set = get_set_from_solver(solver, active_args, query_argument, framework, is_attacking_query);
		if (is_attacking_query) {
			is_rejected = true;
			is_extension_certificate = true;
		}
		else {
			//check if set is PR, by checking if reduct has CO set
			ArrayBitSet reduct = Reduct::get_reduct_set(active_args, framework, calculated_set);
			SatSolver *solver_reduct = NULL;
			solver_reduct = new SatSolver(reduct._array.size());
			Encoding::add_clauses_nonempty_complete_set(*solver_reduct, framework, reduct);
			if (!(*solver_reduct).solve())
			{
				// cannot calculate CO set in reduct, hence set used for reduction has to be a PR set
				// since the PR set does not contain the query, it's a counter-example
				is_rejected = true;
				//check if calculated CO extension rejects query
				list<uint32_t> calculated_set_2 = get_set_from_solver(solver, active_args, query_argument, framework, is_extension_certificate);
				//check if calculated CO extension contains query, if so then extend complement clause by the new extension
				//since it is uninteresting to visit the combined extension once again
				if (tools::Tools_List::contains(calculated_set_2, query_argument)) {
					list<uint32_t> calculated_set_tmp = tools::Tools_List::extend_list(calculated_set, calculated_set_2);
					calculated_set = calculated_set_tmp;
				}
			}
			delete solver_reduct;
		}

		return calculated_set;
	}
}