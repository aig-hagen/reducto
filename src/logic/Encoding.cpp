#include "../../include/logic/Encoding.h"

using namespace std;
using std::vector;

int64_t Encoding::get_literal_accepted(uint32_t argument, bool isPositive)
{
	int64_t variable = static_cast<int64_t>(argument);
	return isPositive ? variable : -1 * variable;
}

int64_t Encoding::get_literal_rejected(uint32_t argsSize, uint32_t argument, bool isPositive)
{
	int64_t variable = static_cast<int64_t>(argument) + argsSize;
	return isPositive ? variable : -1 * variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_rejected_clauses(SatSolver &solver, uint32_t argsSize, uint32_t argument)
{
	// basic acceptance and rejection clause
	// Part I:  models that an argument cannot be accepted and rejected at the same time
	solver.add_clause_short(
		Encoding::get_literal_rejected(argsSize, argument, false),
		Encoding::get_literal_accepted(argument, false));

	// Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	vector<int64_t> rejection_reason_clause;
	rejection_reason_clause.push_back(Encoding::get_literal_rejected(argsSize, argument, false));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_completeness_clause(SatSolver& solver, uint32_t argument)
{
	//constitutes that if an argument is not labelled IN, at least one of it's attacker has to be NOT_OUT
	vector<int64_t> completeness_clause;
	completeness_clause.push_back(Encoding::get_literal_accepted(argument, true));
	return completeness_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SatSolver &solver, uint32_t argsSize, uint32_t argument, uint32_t attacker, vector<int64_t> &rejection_reason_clause)
{
	// Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	solver.add_clause_short(
		Encoding::get_literal_rejected(argsSize, argument, true),
		Encoding::get_literal_accepted(attacker, false));

	// Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	rejection_reason_clause.push_back(Encoding::get_literal_accepted(attacker, true));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SatSolver &solver, uint32_t argument, uint32_t attacker)
{
	if (argument != attacker)
	{
		solver.add_clause_short(
			Encoding::get_literal_accepted(argument, false),
			Encoding::get_literal_accepted(attacker, false));
	}
	else
	{
		solver.add_clause_short(Encoding::get_literal_accepted(argument, false), 0);
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
		Encoding::get_literal_accepted(argument, false),
		Encoding::get_literal_rejected(argsSize, attacker, true));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_completeness_clause_per_attacker(SatSolver& solver, uint32_t argsSize, uint32_t argument, uint32_t attacker, vector<int64_t>& completeness_clause) {
	//constitutes that if an argument is not labelled IN, at least one of it's attacker has to be NOT_OUT
	completeness_clause.push_back(Encoding::get_literal_rejected(argsSize, attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible_encoding(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs, uint32_t argument)
{
	vector<int64_t> rejection_reason_clause = add_rejected_clauses(solver, framework.num_args, argument);

	vector<uint32_t> attackers = framework.attackers[argument];

	for (std::vector<unsigned int>::size_type i = 0; i < attackers.size(); i++)
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

static void add_complete_encoding(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs, uint32_t argument)
{
	vector<int64_t> rejection_reason_clause = add_rejected_clauses(solver, framework.num_args, argument);
	vector<int64_t> completeness_clause = add_completeness_clause(solver, argument);
	
	vector<uint32_t> attackers = framework.attackers[argument];

	for (std::vector<unsigned int>::size_type i = 0; i < attackers.size(); i++)
	{
		if (activeArgs._bitset[attackers[i]])
		{
			add_rejected_clauses_per_attacker(solver, framework.num_args, argument, attackers[i], rejection_reason_clause);
			add_conflict_free_per_attacker(solver, argument, attackers[i]);
			add_defense_per_attacker(solver, framework.num_args, argument, attackers[i]);
			add_completeness_clause_per_attacker(solver, framework.num_args, argument, attackers[i], completeness_clause);
		}
	}

	solver.add_clause(rejection_reason_clause);
	solver.add_clause(completeness_clause);
	rejection_reason_clause.clear();
	completeness_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_clauses_nonempty_admissible_set(SatSolver &solver, AF &framework, ArrayBitSet &activeArgs)
{
	vector<int64_t> non_empty_clause;

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

	for (std::vector<unsigned int>::size_type i = 0; i < activeArgs._array.size(); i++) {
		non_empty_clause.push_back(Encoding::get_literal_accepted(activeArgs._array[i], true));
		add_complete_encoding(solver, framework, activeArgs, activeArgs._array[i]);

		solver.add_clause_short(
			Encoding::get_literal_accepted(activeArgs._array[i], true),
			Encoding::get_literal_rejected(framework.num_args, activeArgs._array[i], true));
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encoding::add_complement_clause(SatSolver &solver, ArrayBitSet &activeArgs)
{
	vector<int64_t> clause;

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