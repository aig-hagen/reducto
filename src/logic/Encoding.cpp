#include "../../include/logic/Encoding.h"

using namespace std;
using std::vector;

int64_t Encoding::get_literal_accepted(uint32_t argument, bool isInverted)
{
	int64_t variable = static_cast<int64_t>(argument);
	return isInverted ? -1 * variable: variable;
}

int64_t Encoding::get_literal_rejected(uint32_t argsSize, uint32_t argument, bool isInverted)
{
	int64_t variable = static_cast<int64_t>(argument) + argsSize;
	return isInverted ? -1 * variable : variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_rejected_clauses(SatSolver &solver, uint32_t argsSize, uint32_t argument)
{
	// basic acceptance and rejection clause
	// Part I:  models that an argument cannot be accepted and rejected at the same time
	solver.add_clause_short(
		Encoding::get_literal_rejected(argsSize, argument, true),
		Encoding::get_literal_accepted(argument, true));

	// Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	vector<int64_t> rejection_reason_clause;
	rejection_reason_clause.push_back(Encoding::get_literal_rejected(argsSize, argument, true));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SatSolver &solver, uint32_t argsSize, uint32_t argument, uint32_t attacker, vector<int64_t> &rejection_reason_clause)
{
	// Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	solver.add_clause_short(
		Encoding::get_literal_rejected(argsSize, argument, false),
		Encoding::get_literal_accepted(attacker, true));

	// Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	rejection_reason_clause.push_back(Encoding::get_literal_accepted(attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SatSolver &solver, uint32_t argument, uint32_t attacker)
{
	if (argument != attacker)
	{
		solver.add_clause_short(
			Encoding::get_literal_accepted(argument, true),
			Encoding::get_literal_accepted(attacker, true));
	}
	else
	{
		solver.add_clause_short(Encoding::get_literal_accepted(argument, true), 0);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_defense_per_attacker(SatSolver &solver, uint32_t argsSize, uint32_t argument, uint32_t attacker)
{
	if (argument == attacker)
	{
		return;
	}
	//is not a self-attack

	//models the notion of defense in an abstract argumentation framework: 
	// if an argument is accepted to be in the admissible set, all its attackers must be rejected
	solver.add_clause_short(
		Encoding::get_literal_accepted(argument, true),
		Encoding::get_literal_rejected(argsSize, attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs, uint32_t argument)
{
	vector<int64_t> rejection_reason_clause = add_rejected_clauses(solver, framework.num_args, argument);
	
	vector<uint32_t> attackers = framework.attackers[argument];

	for (int i = 0; i < attackers.size(); i++)
	{
		if (activeArgs._bitset[attackers[i]])
		{
			add_rejected_clauses_per_attacker(solver, framework.num_args, argument, attackers[i], rejection_reason_clause);
			add_conflict_free_per_attacker(solver, argument, attackers[i]);
			add_defense_per_attacker(solver, framework.num_args, argument, attackers[i]);
		}
	}

	solver.add_clause(rejection_reason_clause);
	rejection_reason_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_clauses_nonempty_admissible_set(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs)
{
	vector<int64_t> non_empty_clause;

	for (int i = 0; i < activeArgs._array.size(); i++) {
		non_empty_clause.push_back(get_literal_accepted(activeArgs._array[i], false));
		add_admissible(solver, framework, activeArgs, activeArgs._array[i]);
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_complement_clause(SatSolver &solver, ArrayBitSet &activeArgs)
{
	vector<uint32_t> args_not_in_model;
	vector<uint32_t> args_in_model;

	for (int i = 0; i < activeArgs._array.size(); i++) {
		int64_t arg_64 = get_literal_accepted(activeArgs._array[i], false);
		if (solver.check_var_model(arg_64)) {
			args_in_model.push_back(activeArgs._array[i]);
		}
		else {
			args_not_in_model.push_back(activeArgs._array[i]);
		}
	}

	for (int i = 0; i < args_in_model.size(); i++) {
		vector<int64_t> clause;
		for (int i = 0; i < args_not_in_model.size(); i++) {
			clause.push_back(get_literal_accepted(args_not_in_model[i], false));
		}
		clause.push_back(get_literal_accepted(args_in_model[i], true));
		solver.add_clause(clause);
		clause.clear();
	}
}