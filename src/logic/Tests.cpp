#include "../../include/logic/Tests.h"
#include <cryptominisat.h>
#include <assert.h>
#include <vector>

#include <limits.h>

using std::vector;
using namespace CMSat;

using namespace std;

void static runParallel(int numArgsStart, int numArgsEnd, argFramework_t *framework, activeArgs_t *actives, double &run_time_para_rec)
{
	double start, end;
	start = omp_get_wtime();
	for (uint32_t i = numArgsStart; i < numArgsEnd + 1; i++)
	{
		uint32_t argument = i;
		nodeUInt32_t **proof_extension = NULL;
		proof_extension = (nodeUInt32_t **)malloc(sizeof * proof_extension);
		if (proof_extension == NULL) {
			printf("Memory allocation failed\n");
			exit(1);
		}
		//printf("%d: ------- extension allocated --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());												//DEBUG

		printf("////////////////////////// ARGUMENT %d //////////////////////////////\n", i);
		bool isScepticAccepted = !ScepticalPRParallel::check_rejection_parallel(argument, framework, actives, proof_extension, NUM_CORES_TESTS);
		std::cout << std::boolalpha << "sceptic acceptance of " << argument << " : " << isScepticAccepted << std::endl;
		if (!isScepticAccepted)
		{
			if (*proof_extension == NULL)
			{
				if (framework->attackers->content[argument][argument] != 0)
				{
					printf("Argument attacks itself.\n");
				}
				else
				{
					printf("No Extension found\n");
				}
			}
			else
			{
				EXTENSIONSOLVER_CMS::BuildExtension(framework, actives, proof_extension);
				printf("Extension that proves rejection: ");
				print_list_uint32((*proof_extension));
				printf("\n");
			}
		}
	}
	end = omp_get_wtime();
	run_time_para_rec = end - start;
	printf("Compute Time: %f seconds\n", run_time_para_rec);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void AnalyseSolvingAlgorithms(argFramework_t *framework, activeArgs_t *actives, int numArgsStart, int numArgsEnd)
{
	double start, end, run_time_seq_rec, run_time_seq_it, run_time_para_rec, diff_para_rec_seq_rec, diff_para_rec_seq_it;

	printf("==================== sequential recursive ==========================\n");
	start = omp_get_wtime();
	for (uint32_t i = numArgsStart; i < numArgsEnd + 1; i++)
	{
		uint32_t argument = i;
		bool isScepticAccepted = !ScepticalPRSequential::check_rejection_per_reduct_recursiv(argument, framework, actives);
		std::cout << std::boolalpha << "sceptic acceptance of " << argument << " : " << isScepticAccepted << std::endl;
	}
	end = omp_get_wtime();
	run_time_seq_rec = end - start;
	printf("Compute Time: %f seconds\n", run_time_seq_rec);

	printf("==================== sequential iterative ==========================\n");
	start = omp_get_wtime();
	for (uint32_t i = numArgsStart; i < numArgsEnd + 1; i++)
	{
		uint32_t argument = i;
		bool isScepticAccepted = !ScepticalPRSequential::check_rejection_iterative(argument, framework, actives);
		std::cout << std::boolalpha << "sceptic acceptance of " << argument << " : " << isScepticAccepted << std::endl;
	}
	end = omp_get_wtime();
	run_time_seq_it = end - start;
	printf("Compute Time: %f seconds\n", run_time_seq_it);

	printf("==================== parallel recursive ==========================\n");
	runParallel(numArgsStart, numArgsEnd, framework, actives, run_time_para_rec);
	diff_para_rec_seq_rec = run_time_para_rec - run_time_seq_rec;
	printf("Difference to sequential recursive: %f seconds, meaning %.2f%% \n", diff_para_rec_seq_rec, diff_para_rec_seq_rec / run_time_seq_rec * 100);
	diff_para_rec_seq_it = run_time_para_rec - run_time_seq_it;
	printf("Difference to sequential iterative: %f seconds, meaning %.2f%% \n", diff_para_rec_seq_it, diff_para_rec_seq_it / run_time_seq_it * 100);
}


/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


static void test0()
{
	argumentInitTemp_t *head = set_up_initialization(4);
	add_attack(head, 1, 2);
	//add_attack(head, 1, 3);
	//add_attack(head, 2, 4);
	//add_attack(head, 3, 1);
	add_attack(head, 4, 1);
	
	argFramework_t *framework = initialize_framework(head);

	printf("Attackers\n");
	print_matrix(framework->attackers);

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(4);
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

	SATProblem_t *problem = create_sat_problem(actives->numberActiveArguments);

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

	InitialSetSolver::calculate_next_solution(framework, actives, problem);
	nodeUInt32_t *initial_set1 = get_set_from_solution(problem->solution, actives);
	printf("Initial set 1 = ");
	print_list_uint32(initial_set1);
	printf("\n");

	InitialSetSolver::calculate_next_solution(framework, actives, problem);
	nodeUInt32_t *initial_set2 = get_set_from_solution(problem->solution, actives);
	printf("Initial set 2 = ");
	print_list_uint32(initial_set2);
	printf("\n");
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void testSelfAttack()
{
	argumentInitTemp_t *head = set_up_initialization(3);
	add_attack(head, 1, 1);
	add_attack(head, 2, 1);
	add_attack(head, 3, 2);
	argFramework_t *framework = initialize_framework(head);

	printf("Attackers\n");
	print_matrix(framework->attackers);

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(3);
	AnalyseSolvingAlgorithms(framework, actives, 1, 1);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void test4Args()
{
	argumentInitTemp_t *head = set_up_initialization(4);
	add_attack(head, 1, 2);
	add_attack(head, 4, 1);
	argFramework_t *framework = initialize_framework(head);

	printf("Attackers\n");
	print_matrix(framework->attackers);

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(4);
	AnalyseSolvingAlgorithms(framework, actives, 1 ,4);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void test6Args() 
{
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
	printf("\n");

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(framework->number);
	AnalyseSolvingAlgorithms(framework, actives, 1 ,6);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void test6ArgsFile()
{
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	}
	else {
		perror("getcwd() error");
		exit(1);
	}
	argFramework_t *framework = ParserICCMA::parse_af("./../../../rsc/examples/example2.i23");

	activeArgs_t *actives = initialize_actives(framework->number);
	AnalyseSolvingAlgorithms(framework, actives, 1, 6);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void testArgsALot()
{
	int numArgs = 50;
	argumentInitTemp_t *head = set_up_initialization(numArgs);
	for (int i = 1; i < numArgs + 1; i++)
	{
		if (i % 3)
		{
			add_attack(head, i, ( i * numArgs / 2) %numArgs);
			add_attack(head, i, (i * numArgs / 3) % numArgs);
		}
		else
		{
			add_attack(head, i, (i + numArgs / 2) % numArgs);
			add_attack(head, i, (i + numArgs / 3) % numArgs);
		}
	}
	argFramework_t *framework = initialize_framework(head);

	/*printf("Attackers\n");
	print_matrix(framework->attackers);
	printf("\n");*/

	printf("Victims\n");
	print_matrix(framework->victims);
	printf("\n");

	activeArgs_t *actives = initialize_actives(numArgs);
	AnalyseSolvingAlgorithms(framework, actives, 1, 1);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void static add_clauses(SATSolver *solver, listInt64_t *clauses)
{
	listInt64_t *currentClause = clauses;

	while (currentClause != NULL)
	{
		nodeInt64_t *currentLiteral = currentClause->list;
		vector<Lit> clause;

		while (currentLiteral != NULL)
		{
			uint32_t var = abs(currentLiteral->number) - 1;
			clause.push_back(Lit(var, (currentLiteral->number > 0) ? false : true));

			currentLiteral = currentLiteral->next;
		}

		(*solver).add_clause(clause);
		currentClause = currentClause->next;
	}
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void testCMS()
{
	argumentInitTemp_t *head = set_up_initialization(4);
	add_attack(head, 1, 2);
	add_attack(head, 4, 1);
	argFramework_t *framework = initialize_framework(head);

	activeArgs_t *actives = initialize_actives(4);

	SATProblem_t *problem = create_sat_problem(actives->numberActiveArguments);

	add_clauses_nonempty_admissible_set(problem, framework, actives);

	SATSolver solver;
	vector<Lit> clause;

	//Let's use 4 threads
	solver.set_num_threads(1);
	solver.new_vars(8);

	add_clauses(&solver, problem->clauses);

	lbool ret = solver.solve();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void testExampleCMS()
{
	SATSolver solver;
	vector<Lit> clause;

	//Let's use 4 threads
	solver.set_num_threads(4);

	//We need 3 variables. They will be: 0,1,2
	//Variable numbers are always trivially increasing
	solver.new_vars(3);
	solver.new_vars(3);

	//add "1 0"
	clause.push_back(Lit(0, false));
	solver.add_clause(clause);
	
	//add "-2 0"
	clause.clear();
	clause.push_back(Lit(1, true));
	solver.add_clause(clause);
	
	//add "-1 2 3 0"
	clause.clear();
	clause.push_back(Lit(0, true));
	clause.push_back(Lit(1, false));
	clause.push_back(Lit(2, false));
	solver.add_clause(clause);

	lbool ret = solver.solve();
	assert(ret == l_True);
	std::cout
	<< "Solution is: "
	<< solver.get_model()[0]
	<< ", " << solver.get_model()[1]
	<< ", " << solver.get_model()[2]
	<< std::endl;
	
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
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

static void testFile()
{
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	}
	else {
		perror("getcwd() error");
		exit(1);
	}

	//printf("%d: ------- program started --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());															//DEBUG

	argFramework_t *framework = ParserICCMA::parse_af("/home/jsander/solvers/ICCMA23/benchmarks/main/WS_400_8_30_30.i23");

	//printf("%d: ------- framework initialized --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());													//DEBUG

	activeArgs_t *actives = initialize_actives(framework->number);

	//printf("%d: ------- actives initialized --- memory usage: %ld\n", omp_get_thread_num(), get_mem_usage());														//DEBUG

	double runtime = 0;
	runParallel(18, 18, framework, actives, runtime);
}

void TestCases::run_Tests()
{
	//test0();
	testSelfAttack();
	//test4Args();
	//test6Args();
	//test6ArgsFile();
	//testArgsALot();
	//testCMS();
	//testExampleCMS();
	//testFile();
}
