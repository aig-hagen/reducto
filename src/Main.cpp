#include "../include/Main.h"
#include <cryptominisat.h>
#include <assert.h>
#include <vector>
using std::vector;
using namespace CMSat;

using namespace std;

int main()
{
	//cout << "Hello CMake." << endl;

	//argumentInitTemp_t *head = set_up_initialization(4);
	//add_attack(head, 1, 2);
	////add_attack(head, 1, 3);
	////add_attack(head, 2, 4);
	////add_attack(head, 3, 1);
	//add_attack(head, 4, 1);
	argumentInitTemp_t *head = set_up_initialization(6);
	add_attack(head, 1, 2);
	add_attack(head, 1, 6);
	add_attack(head, 2, 4);
	add_attack(head, 2, 5);
	add_attack(head, 3, 1);
	add_attack(head, 3, 2);
	add_attack(head, 4, 1);
	add_attack(head, 5, 4);
	argFramework_t *framework = initialize_framework(head);

	printf("Attackers\n");
	print_matrix(framework->attackers);

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(6);
	/*printf("\n Active Arguments: base\n");
	print_active_arguments(actives);
	printf("\n");
	printf("\n");
	printf("\n");*/

	/*activeArgs_t *reduct = get_reduct(actives, framework, 2);
	printf("\n Active Arguments: reducted by 2\n");
	print_active_arguments(reduct);
	printf("\n");

	activeArgs_t *reduct2 = get_reduct(reduct, framework, 3);
	printf("\n Active Arguments: reducted by 3\n");
	print_active_arguments(reduct2);
	printf("\n");

	nodeUInt32_t *initial_set23 = create_list_uint32((uint32_t)2);
	push_uint32(initial_set23, 3);
	activeArgs_t *reduct23 = get_reduct_set(actives, framework, initial_set23);
	printf("\n Active Arguments: reducted by {2,3}\n");
	print_active_arguments(reduct23);
	printf("\n");*/

	/*
	activeArgs_t *reduct3 = get_reduct(reduct2, framework, 1);
	printf("\n Active Arguments: reducted by 1\n");
	print_active_arguments(reduct3);

	activeArgs_t *reduct4 = get_reduct(reduct3, framework, 4);
	printf("\n Active Arguments: reducted by 4\n");
	print_active_arguments(reduct4);*/

	/*SATProblem_t *problem = create_sat_problem(actives->numberActiveArguments);*/

	/*add_clauses_nonempty_admissible_set(problem, framework, actives);
	printf("\nEncoding SAT for basis: \n");
	print_list_list_int64(problem->clauses);
	printf("\n");

	ExternalSatSolver::solve(problem, "cryptominisat5");
	printf("\n=====computed solution======\n");
	print_array(problem->solution);

	nodeUInt32_t *extension = get_set_from_solution(problem, actives);
	printf("\n=====extension found======\n");
	print_list_uint32(extension);

	add_complement_clause(problem);
	printf("\n=========================================== new set ===========================================\n");
	ExternalSatSolver::solve(problem, "cryptominisat5");
	nodeUInt32_t *extension2 = get_set_from_solution(problem, actives);
	printf("\n=====extension found======\n");
	print_list_uint32(extension2);

	add_complement_clause(problem);
	printf("\n=========================================== new set ===========================================\n");
	ExternalSatSolver::solve(problem, "cryptominisat5");*/

	/*nodeUInt32_t *initial_set1 = create_list_uint32((uint32_t)0);
	InitialSetSolver::calculate_next_initial_set(framework, actives, problem, initial_set1);
	printf("Initial set 1 = ");
	print_list_uint32(initial_set1);
	printf("\n");

	nodeUInt32_t *initial_set2 = create_list_uint32((uint32_t)0);
	InitialSetSolver::calculate_next_initial_set(framework, actives, problem, initial_set2);
	printf("Initial set 2 = ");
	print_list_uint32(initial_set2);
	printf("\n");*/

//#pragma omp parallel for
	for (uint32_t i = 1; i < 7; i++)
	{
		//printf("Thread number: %d \n", omp_get_thread_num());
		uint32_t argument = i;
		//bool isScepticAccepted = !ScepticalPRSequential::check_rejection_per_reduct_recursiv(argument, framework, actives);
		bool isScepticAccepted = !ScepticalPRSequential::check_rejection_iterative(argument, framework, actives);
		//bool isScepticAccepted = !ScepticalPRParallel::check_rejection_parallel(argument, framework, actives);
		std::cout << std::boolalpha << "\nsceptic acceptance of " << argument << " : " << isScepticAccepted << std::endl;
	}

	return 0;

//SATSolver solver;
//vector<Lit> clause;
//
////Let's use 4 threads
//solver.set_num_threads(4);
//
////We need 3 variables. They will be: 0,1,2
////Variable numbers are always trivially increasing
//solver.new_vars(3);
//
////add "1 0"
//clause.push_back(Lit(0, false));
//solver.add_clause(clause);
//
////add "-2 0"
//clause.clear();
//clause.push_back(Lit(1, true));
//solver.add_clause(clause);
//
////add "-1 2 3 0"
//clause.clear();
//clause.push_back(Lit(0, true));
//clause.push_back(Lit(1, false));
//clause.push_back(Lit(2, false));
//solver.add_clause(clause);
//
//lbool ret = solver.solve();
//assert(ret == l_True);
//std::cout
//<< "Solution is: "
//<< solver.get_model()[0]
//<< ", " << solver.get_model()[1]
//<< ", " << solver.get_model()[2]
//<< std::endl;
//
////assumes 3 = FALSE, no solutions left
//vector<Lit> assumptions;
//assumptions.push_back(Lit(2, true));
//ret = solver.solve(&assumptions);
//assert(ret == l_False);
//
////without assumptions we still have a solution
//ret = solver.solve();
//assert(ret == l_True);
//
////add "-3 0"
////No solutions left, UNSATISFIABLE returned
//clause.clear();
//clause.push_back(Lit(2, true));
//solver.add_clause(clause);
//ret = solver.solve();
//assert(ret == l_False);
//
//return 0;
}
