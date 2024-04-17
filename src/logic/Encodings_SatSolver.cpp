#include "../../include/logic/Encodings_SatSolver.h"

using namespace std;
using std::vector;

static int64_t get_literal_accepted(activeArgs_t *activeArgs, uint32_t argument, bool isInverted)
{
	int64_t variable = activeArgs->matrix->content[argument][2];
	return isInverted ? -1 * variable: variable;
}

static int64_t get_literal_rejected(activeArgs_t *activeArgs, uint32_t argument, bool isInverted)
{
	int64_t variable = static_cast<int64_t>(activeArgs->matrix->content[argument][2]) + activeArgs->encodingToArgument->length - 1;
	return isInverted ? -1 * variable : variable;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<int64_t> add_rejected_clauses(SatSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	// basic acceptance and rejection clause
	//Part I:  models that an argument cannot be accepted and rejected at the same time
	//create disjunction
	vector<int64_t> non_simultaneity_clause;
	non_simultaneity_clause.push_back(get_literal_rejected(activeArgs, argument, true));
	non_simultaneity_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	(*solver).add_clause(non_simultaneity_clause);
	non_simultaneity_clause.clear();

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	vector<int64_t> rejection_reason_clause;
	rejection_reason_clause.push_back(get_literal_rejected(activeArgs, argument, true));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SatSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker, vector<int64_t> *rejection_reason_clause)
{
	//Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	//create disjunction for active attacker
	vector<int64_t> non_simultaneity_with_attacker_clause;
	non_simultaneity_with_attacker_clause.push_back(get_literal_rejected(activeArgs, argument, false));
	non_simultaneity_with_attacker_clause.push_back(get_literal_accepted(activeArgs, attacker, true));
	(*solver).add_clause(non_simultaneity_with_attacker_clause);
	non_simultaneity_with_attacker_clause.clear();

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	(*rejection_reason_clause).push_back(get_literal_accepted(activeArgs, attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SatSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	//create disjunction
	vector<int64_t> conflictfree_clause;
	conflictfree_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	if (argument != attacker)
	{
		//is not a self-attack
		conflictfree_clause.push_back(get_literal_accepted(activeArgs, attacker, true));
	}
	(*solver).add_clause(conflictfree_clause);
	conflictfree_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_defense_per_attacker(SatSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	if (argument == attacker)
	{
		return;
	}
	//is not a self-attack

	//models the notion of defense in an abstract argumentation framework: 
	// if an argument is accepted to be in the admissible set, all its attackers must be rejected
	//create disjunction
	vector<int64_t> defense_clause;
	defense_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	defense_clause.push_back(get_literal_rejected(activeArgs, attacker, false));
	(*solver).add_clause(defense_clause);
	defense_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible(SatSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	vector<int64_t> rejection_reason_clause = add_rejected_clauses(solver, framework, activeArgs, argument);

	//iterate through all active attackers
	uint32_t current_attacker = 0;
	while (has_next_in_row(framework->attackers, argument, current_attacker))
	{
		current_attacker = get_next_in_row(framework->attackers, argument, current_attacker);
		if (is_active(activeArgs, current_attacker))
		{
			add_rejected_clauses_per_attacker(solver, activeArgs, argument, current_attacker, &rejection_reason_clause);
			add_conflict_free_per_attacker(solver, activeArgs, argument, current_attacker);
			add_defense_per_attacker(solver, activeArgs, argument, current_attacker);
		}
	}

	(*solver).add_clause(rejection_reason_clause);
	rejection_reason_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_SatSolver::add_clauses_nonempty_admissible_set(SatSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs)
{
	uint32_t currentArgument = get_first_active(activeArgs);
	if (currentArgument == 0)
	{
		return;
	}

	vector<int64_t> non_empty_clause;
	non_empty_clause.push_back(get_literal_accepted(activeArgs ,currentArgument, false));

	add_admissible(solver, framework, activeArgs, currentArgument);
	//iterate through all active arguments
	while (has_next_active(activeArgs, currentArgument))
	{
		currentArgument = get_next_active(activeArgs, currentArgument);

		//models the nonemptiness of initial sets, by extending a disjunction demanding that at least one argument has to be accepted
		//extend disjunction
		non_empty_clause.push_back(get_literal_accepted(activeArgs, currentArgument, false));

		add_admissible(solver, framework, activeArgs, currentArgument);
	}
	(*solver).add_clause(non_empty_clause);
	non_empty_clause.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_SatSolver::add_complement_clause(SatSolver *solver, activeArgs_t *activeArgs)
{
	vector<int64_t> complement_clause;
	vector<bool> model = (*solver).get_model();

	for (uint32_t i = 0; i < activeArgs->numberActiveArguments; i++)
	{
		if (model[i] == true)
		{
			complement_clause.push_back(-1 * (static_cast<int64_t>(i) + 1));
		}
	}

	(*solver).add_clause(complement_clause);
	complement_clause.clear();
}