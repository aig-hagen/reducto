#include "../../include/logic/Encoding.h"

using namespace std;
using std::vector;

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int64_t Encoding::get_literal_accepted(uint32_t argument, bool isPositive)
{
	int64_t variable = static_cast<int64_t>(argument);
	return isPositive ? variable : -1 * variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int64_t Encoding::get_literal_rejected(AF &framework, uint32_t argument, bool isPositive)
{
	int64_t variable = static_cast<int64_t>(argument) + framework.num_args;
	return isPositive ? variable : -1 * variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_consistence_clause(SatSolver &solver, AF &framework, uint32_t argument)
{
	// models that an argument cannot be accepted and rejected at the same time
	solver.add_clause_short(
		Encoding::get_literal_rejected(framework, argument, false),
		Encoding::get_literal_accepted(argument, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_legal_in_clause(SatSolver &solver, AF &framework, uint32_t argument, uint32_t attacker)
{
	//ensures that all arguments are legally labeled IN
	solver.add_clause_short(
		Encoding::get_literal_accepted(argument, false),
		Encoding::get_literal_rejected(framework, attacker, true)
	);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


static vector<int64_t> add_legal_out_clause(SatSolver &solver, AF &framework, uint32_t argument)
{
	// Part I of ensuring that all arguments are labeled legally OUT
	vector<int64_t> legal_out_clause;
	legal_out_clause.push_back(Encoding::get_literal_rejected(framework, argument, false));
	return legal_out_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_legal_out_clause_per_attacker(SatSolver &solver, AF &framework, uint32_t argument, uint32_t attacker, vector<int64_t> &legal_out_clause)
{
	// Part II of ensuring that all arguments are labeled legally OUT
	legal_out_clause.push_back(Encoding::get_literal_accepted(attacker, true));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_legal_undec_in_clause(SatSolver &solver, AF &framework, uint32_t argument, uint32_t attacker)
{
	//ensures that all arguments are legally labeled UNDEC by ensuring that there is no attacker of an UNDEC labeled argument that is labeled IN
	solver.add_clause_short(
		Encoding::get_literal_rejected(framework, argument, true),
		Encoding::get_literal_accepted(attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_legal_undec_out_clause(SatSolver &solver, AF &framework, uint32_t argument)
{
	// Part I of ensuring that all arguments are labeled legally UNDEC by ensuring that not all attackers of an UNDEC argument are labeled OUT
	vector<int64_t> legal_undec_out_clause;
	legal_undec_out_clause.push_back(Encoding::get_literal_accepted(argument, true));
	return legal_undec_out_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_legal_undec_out_clause_per_attacker(SatSolver &solver, AF &framework, uint32_t argument, uint32_t attacker, vector<int64_t> &legal_undec_out_clause)
{
	// Part II of ensuring that all arguments are labeled legally UNDEC by ensuring that not all attackers of an UNDEC argument are labeled OUT
	legal_undec_out_clause.push_back(Encoding::get_literal_rejected(framework, attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible_encoding(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs, uint32_t argument)
{
	add_consistence_clause(solver, framework, argument);
	vector<int64_t> legal_out_clause = add_legal_out_clause(solver, framework, argument);
	vector<uint32_t> attackers = framework.attackers[argument];
	for (std::vector<unsigned int>::size_type i = 0; i < attackers.size(); i++)
	{
		if (activeArgs._bitset[attackers[i]])
		{
			add_legal_in_clause(solver, framework, argument, attackers[i]);
			add_legal_out_clause_per_attacker(solver, framework, argument, attackers[i], legal_out_clause);
		}
	}

	solver.add_clause(legal_out_clause);
	legal_out_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_complete_encoding(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs, uint32_t argument)
{
	add_consistence_clause(solver, framework, argument);
	vector<int64_t> legal_out_clause = add_legal_out_clause(solver, framework, argument);
	vector<int64_t> legal_undec_out_clause = add_legal_undec_out_clause(solver, framework, argument);
	vector<uint32_t> attackers = framework.attackers[argument];
	for (std::vector<unsigned int>::size_type i = 0; i < attackers.size(); i++)
	{
		if (activeArgs._bitset[attackers[i]])
		{
			add_legal_in_clause(solver, framework, argument, attackers[i]);
			add_legal_out_clause_per_attacker(solver, framework, argument, attackers[i], legal_out_clause);
			add_legal_undec_out_clause_per_attacker(solver, framework, argument, attackers[i], legal_undec_out_clause);
			add_legal_undec_in_clause(solver, framework, argument, attackers[i]);
		}
	}

	solver.add_clause(legal_out_clause);
	solver.add_clause(legal_undec_out_clause);
	legal_out_clause.clear();
	legal_undec_out_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_clauses_nonempty_admissible_set(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs)
{
	vector<int64_t> non_empty_clause;

	// iterate through all active arguments
	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		non_empty_clause.push_back(Encoding::get_literal_accepted(activeArgs._array[i], true));
		add_admissible_encoding(solver, framework, activeArgs, activeArgs._array[i]);
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_clauses_nonempty_complete_set(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs)
{
	vector<int64_t> non_empty_clause;

	// iterate through all active arguments
	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		non_empty_clause.push_back(Encoding::get_literal_accepted(activeArgs._array[i], true));
		add_complete_encoding(solver, framework, activeArgs, activeArgs._array[i]);
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_clauses_nonempty_stable_set(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs)
{
	vector<int64_t> non_empty_clause;

	// iterate through all active arguments
	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		non_empty_clause.push_back(Encoding::get_literal_accepted(activeArgs._array[i], true));
		add_complete_encoding(solver, framework, activeArgs, activeArgs._array[i]);

		solver.add_clause_short(
			Encoding::get_literal_accepted(activeArgs._array[i], true),
			Encoding::get_literal_rejected(framework, activeArgs._array[i], true));
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_complement_clause(SatSolver &solver, ArrayBitSet &activeArgs)
{
	vector<int64_t> clause;

	// iterate through all active arguments
	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		int64_t in_variable = get_literal_accepted(activeArgs._array[i], true);
		if (!solver.check_var_model(in_variable)) {
			// new solutions have to contain at least one input variable, that is not contained in the current solution
			clause.push_back(in_variable);
		}
	}

	solver.add_clause(clause);
	clause.clear();
}