#include "../../include/logic/Encodings.h"

using namespace std;
using std::vector;

static int64_t get_literal_accepted(vector<uint32_t> &activeArgs, uint32_t idxInActives, bool isInverted)
{
	int64_t variable = static_cast<int64_t>(idxInActives) + 1;
	return isInverted ? -1 * variable: variable;
}

static int64_t get_literal_rejected(vector<uint32_t> &activeArgs, uint32_t idxInActives, bool isInverted)
{
	int64_t variable = static_cast<int64_t>(idxInActives) + 1 + activeArgs.size();
	return isInverted ? -1 * variable : variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_rejected_clauses(SatSolver &solver, vector<uint32_t> &activeArgs, uint32_t idxInActives)
{
	// basic acceptance and rejection clause
	//Part I:  models that an argument cannot be accepted and rejected at the same time
	//create disjunction

	solver.add_clause_short(
		get_literal_rejected(activeArgs, idxInActives, true),
		get_literal_accepted(activeArgs, idxInActives, true));

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	vector<int64_t> rejection_reason_clause;
	rejection_reason_clause.push_back(get_literal_rejected(activeArgs, idxInActives, true));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SatSolver &solver, vector<uint32_t> &activeArgs, uint32_t idxInActives, uint32_t idxAtckr, vector<int64_t> &rejection_reason_clause)
{
	//Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	//create disjunction for active attacker

	solver.add_clause_short(
		get_literal_rejected(activeArgs, idxInActives, false),
		get_literal_accepted(activeArgs, idxAtckr, true));

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	rejection_reason_clause.push_back(get_literal_accepted(activeArgs, idxAtckr, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SatSolver &solver, vector<uint32_t> &activeArgs, uint32_t idxInActives, uint32_t idxAtckr)
{
	//create disjunction

	if (idxInActives != idxAtckr)
	{
		solver.add_clause_short(
			get_literal_accepted(activeArgs, idxInActives, true),
			get_literal_accepted(activeArgs, idxAtckr, true));
	}
	else
	{
		solver.add_clause_short(get_literal_accepted(activeArgs, idxInActives, true), 0);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_defense_per_attacker(SatSolver &solver, vector<uint32_t> &activeArgs, uint32_t idxInActives, uint32_t idxAtckr)
{
	if (idxInActives == idxAtckr)
	{
		return;
	}
	//is not a self-attack

	//models the notion of defense in an abstract argumentation framework: 
	// if an argument is accepted to be in the admissible set, all its attackers must be rejected
	//create disjunction

	solver.add_clause_short(
		get_literal_accepted(activeArgs, idxInActives, true),
		get_literal_rejected(activeArgs, idxAtckr, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible(SatSolver &solver, AF &framework, vector<uint32_t> &activeArgs, uint32_t idxInActives)
{
	vector<int64_t> rejection_reason_clause = add_rejected_clauses(solver, activeArgs, idxInActives);

	vector<uint32_t> attackers = framework.attackers[activeArgs[idxInActives]];

	for (int i = 0; i < attackers.size(); i++)
	{
		int64_t idxAtckr = Actives::isActive(activeArgs, attackers[i]);

		if(idxAtckr > -1)
		{
			uint32_t u_idxAtckr = static_cast<uint32_t>(idxAtckr);
			add_rejected_clauses_per_attacker(solver, activeArgs, idxInActives, u_idxAtckr, rejection_reason_clause);
			add_conflict_free_per_attacker(solver, activeArgs, idxInActives, u_idxAtckr);
			add_defense_per_attacker(solver, activeArgs, idxInActives, u_idxAtckr);
		}
	}

	solver.add_clause(rejection_reason_clause);
	rejection_reason_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_SatSolver::add_clauses_nonempty_admissible_set(SatSolver &solver, AF &framework, vector<uint32_t> &activeArgs)
{
	vector<int64_t> non_empty_clause;
	//iterate through all active arguments

	for (int i = 0; i < activeArgs.size(); i++) {
		non_empty_clause.push_back(get_literal_accepted(activeArgs, i, false));
		add_admissible(solver, framework, activeArgs, i);
	}

	solver.add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_SatSolver::add_complement_clause(SatSolver &solver, vector<uint32_t> &activeArgs)
{
	vector<int64_t> complement_clause;
	vector<bool> model = solver.get_model();

	for (uint32_t i = 0; i < activeArgs.size(); i++)
	{
		if (model[i] == true)
		{
			int64_t variable = -1 * (static_cast<int64_t>(i) + 1);
			complement_clause.push_back(variable);
		}
	}

	solver.add_clause(complement_clause);
	complement_clause.clear();
}