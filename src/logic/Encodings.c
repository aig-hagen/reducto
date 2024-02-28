#include "../../include/logic/Encodings.h"

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int64_t get_accepted_variable(activeArgs_t *activeArgs, uint32_t argument)
{
	return (int64_t) 0 + activeArgs->matrix->content[argument][2];
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

int64_t get_rejected_variable(activeArgs_t *activeArgs, uint32_t argument)
{
	return get_accepted_variable(activeArgs, argument) + activeArgs->encodingToArgument->length - 1;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static nodeInt64_t *add_rejected_clauses(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	// basic acceptance and rejection clause
	//Part I:  models that an argument cannot be accepted and rejected at the same time
	//create disjunction
	nodeInt64_t *non_simultaneity_clause = create_list_int64(-1 * get_rejected_variable(activeArgs, argument));
	push_int64(non_simultaneity_clause, -1 * get_accepted_variable(activeArgs, argument));
	add_clause(problem, non_simultaneity_clause);

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	nodeInt64_t *rejection_reason_clause = create_list_int64(-1 * get_rejected_variable(activeArgs, argument));
	return rejection_reason_clause;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_rejected_clauses_per_attacker(SATProblem_t *problem, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker, nodeInt64_t *rejection_reason_clause)
{
	//Part II: ensures that if an attacker 'b' of an argument 'a' is accepted, then 'a' must be rejected
	//iterate through all active attackers
	nodeInt64_t *non_simultaneity_with_attacker_clause = create_list_int64(get_rejected_variable(activeArgs, argument));
	push_int64(non_simultaneity_with_attacker_clause, -1 * get_accepted_variable(activeArgs, attacker));
	add_clause(problem, non_simultaneity_with_attacker_clause);

	//Part III: constitutes that if an argument 'a' is rejected, one of its attackers must be accepted
	//create disjunction
	push_int64(rejection_reason_clause, get_accepted_variable(activeArgs, attacker));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_conflict_free_per_attacker(SATProblem_t *problem, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	//create disjunction
	nodeInt64_t *conflictfree_clause = create_list_int64(-1 * get_accepted_variable(activeArgs, argument));
	if (argument != attacker)
	{
		//is not a self-attack
		push_int64(conflictfree_clause, -1 * get_accepted_variable(activeArgs, attacker));
	}
	add_clause(problem, conflictfree_clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_defense_per_attacker(SATProblem_t *problem, activeArgs_t *activeArgs, uint32_t argument, uint32_t attacker)
{
	if (argument == attacker)
	{
		return;
	}
	//is not a self-attack

	//models the notion of defense in an abstract argumentation framework: 
	// if an argument is accepted to be in the admissible set, all its attackers must be rejected
	//create disjunction
	nodeInt64_t *defense_clause = create_list_int64(-1 * get_accepted_variable(activeArgs, argument));
	push_int64(defense_clause, get_rejected_variable(activeArgs, attacker));
	add_clause(problem, defense_clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_admissible(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	nodeInt64_t *rejection_reason_clause = add_rejected_clauses(problem, framework, activeArgs, argument);

	//iterate through all active attackers
	uint32_t current_attacker = 0;
	while (has_next_in_row(framework->attackers, argument, current_attacker))
	{
		current_attacker = get_next_in_row(framework->attackers, argument, current_attacker);
		if (is_active(activeArgs, current_attacker))
		{
			add_rejected_clauses_per_attacker(problem, activeArgs, argument, current_attacker, rejection_reason_clause);
			add_conflict_free_per_attacker(problem, activeArgs, argument, current_attacker);
			add_defense_per_attacker(problem, activeArgs, argument, current_attacker);
		}
	}

	add_clause(problem, rejection_reason_clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void extend_nonempty(activeArgs_t *activeArgs, uint32_t argument, nodeInt64_t *existingClause)
{
	//models the nonemptiness of initial sets, by extending a disjunction demanding that at least one argument has to be accepted
	//extend disjunction
	push_int64(existingClause, get_accepted_variable(activeArgs, argument));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_clauses_nonempty_admissible_set(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs)
{
	uint32_t currentArgument = get_first_active(activeArgs);
	if (currentArgument == 0)
	{
		return;
	}

	nodeInt64_t *non_empty_clause = create_list_int64(get_accepted_variable(activeArgs, currentArgument));
	add_admissible(problem, framework, activeArgs, currentArgument);
	//iterate through all active arguments
	while (has_next_active(activeArgs, currentArgument))
	{
		currentArgument = get_next_active(activeArgs, currentArgument);
		extend_nonempty(activeArgs, currentArgument, non_empty_clause);
		add_admissible(problem, framework, activeArgs, currentArgument);
	}
	add_clause(problem, non_empty_clause);

	//printf("Encoding SAT for basis: \n");												//DEBUG
	//print_list_list_int64(problem->clauses);											//DEBUG
	//printf("\n");																		//DEBUG
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void add_complete_per_argument(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs, uint32_t argument)
{
	nodeInt64_t *clause = create_list_int64(get_accepted_variable(activeArgs, argument));
	//iterate through all active attackers
	uint32_t current_attacker = 0;
	while (has_next_in_row(framework->attackers, argument, current_attacker))
	{
		current_attacker = get_next_in_row(framework->attackers, argument, current_attacker);
		if (is_active(activeArgs, current_attacker))
		{
			push_int64(clause, -1 * get_rejected_variable(activeArgs, current_attacker));
		}
	}
	add_clause(problem, clause);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_clauses_complete_extension(SATProblem_t *problem, argFramework_t *framework, activeArgs_t *activeArgs)
{
	uint32_t currentArgument = get_first_active(activeArgs);
	if (currentArgument == 0)
	{
		return;
	}

	add_admissible(problem, framework, activeArgs, currentArgument);
	add_complete_per_argument(problem, framework, activeArgs, currentArgument);
	//iterate through all active arguments
	while (has_next_active(activeArgs, currentArgument))
	{
		currentArgument = get_next_active(activeArgs, currentArgument);
		add_admissible(problem, framework, activeArgs, currentArgument);
		add_complete_per_argument(problem, framework, activeArgs, currentArgument);
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void add_complement_clause(SATProblem_t *problem)
{
	nodeInt64_t *clause = NULL; 

	for (int64_t i = 1; i < ((int64_t) problem->numberOfVariables) + 1; i++)
	{
		if (problem->solution->elements[i] == 1)
		{
			if (clause == NULL)
			{
				clause = create_list_int64(-1 * i);
			}
			else
			{
				push_int64(clause, -1 * i);
			}
		}
	}

	add_clause(problem, clause);
}
