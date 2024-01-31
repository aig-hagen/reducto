#include "../include/Encodings_cms.h"

using namespace std;
using std::vector;
using namespace CMSat;

static Lit get_literal_accepted(activeArgs_t *activeArgs, uint32_t argument, bool isInverted)
{
	return Lit(activeArgs->matrix->content[argument][2] - 1, isInverted);
}

static Lit get_literal_rejected(activeArgs_t *activeArgs, uint32_t argument, bool isInverted)
{
	return Lit((activeArgs->matrix->content[argument][2] + activeArgs->encodingToArgument->length - 1) - 1, isInverted);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static vector<Lit> add_rejected_clauses(SATSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	// basic acceptance and rejection clause
	//Part I:  models that an argument cannot be accepted and rejected at the same time
	//create disjunction
	vector<Lit> non_simultaneity_clause;
	non_simultaneity_clause.push_back(get_literal_rejected(activeArgs, argument, true));
	non_simultaneity_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	(*solver).add_clause(non_simultaneity_clause);

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	vector<Lit> rejection_reason_clause;
	rejection_reason_clause.push_back(get_literal_rejected(activeArgs, argument, true));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SATSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker, vector<Lit> *rejection_reason_clause)
{
	//Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	//create disjunction for active attacker
	vector<Lit> non_simultaneity_with_attacker_clause;
	non_simultaneity_with_attacker_clause.push_back(get_literal_rejected(activeArgs, argument, false));
	non_simultaneity_with_attacker_clause.push_back(get_literal_accepted(activeArgs, attacker, true));
	(*solver).add_clause(non_simultaneity_with_attacker_clause);

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	(*rejection_reason_clause).push_back(get_literal_accepted(activeArgs, attacker, false));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SATSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	//create disjunction
	vector<Lit> conflictfree_clause;
	conflictfree_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	if (argument != attacker)
	{
		//is not a self-attack
		conflictfree_clause.push_back(get_literal_accepted(activeArgs, attacker, true));
	}
	(*solver).add_clause(conflictfree_clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_defense_per_attacker(SATSolver *solver, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	if (argument == attacker)
	{
		return;
	}
	//is not a self-attack

	//models the notion of defense in an abstract argumentation framework: 
	// if an argument is accepted to be in the admissible set, all its attackers must be rejected
	//create disjunction
	vector<Lit> defense_clause;
	defense_clause.push_back(get_literal_accepted(activeArgs, argument, true));
	defense_clause.push_back(get_literal_rejected(activeArgs, attacker, false));
	(*solver).add_clause(defense_clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible(SATSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	vector<Lit> rejection_reason_clause = add_rejected_clauses(solver, framework, activeArgs, argument);

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
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_CMS::add_clauses_nonempty_admissible_set(SATSolver *solver, argFramework_t *framework, activeArgs_t *activeArgs)
{
	uint32_t currentArgument = get_first_active(activeArgs);
	if (currentArgument == 0)
	{
		return;
	}

	vector<Lit> non_empty_clause;
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
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void Encodings_CMS::add_complement_clause(SATSolver *solver, activeArgs_t *activeArgs)
{
	vector<Lit> complement_clause;

	for (uint32_t i = 0; i < activeArgs->numberActiveArguments; i++)
	{
		if ((*solver).get_model()[i] == l_True)
		{
			complement_clause.push_back(Lit(i, true));
		}
	}

	(*solver).add_clause(complement_clause);
}