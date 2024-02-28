#include "../../include/logic/ExternalSolver.h"

using namespace std;

uint8_t ExternalSatSolver::solve_pstreams(SATProblem_t *problem, string path_to_solver) {
	redi::pstream process(path_to_solver, redi::pstreams::pstdout | redi::pstreams::pstdin | redi::pstreams::pstderr);
	process << "p cnf " << problem->numberOfVariables << " " << (problem->numberOfClauses + problem->numberOfAssumptions) << "\n";
	//std::cout << "p cnf " << problem->numberOfVariables << " " << (problem->numberOfClauses + problem->numberOfAssumptions) << "\n";		//DEBUG

	// giving the clauses to the solver
	listInt64_t *currentClause = problem->clauses;
	while (currentClause != NULL)
	{
		nodeInt64_t *currentLiteral = currentClause->list;
		while (currentLiteral != NULL)
		{
			process << currentLiteral->number << " ";
			//std::cout << currentLiteral->number << " ";																					//DEBUG
			currentLiteral = currentLiteral->next;
		}

		process << "0" << "\n";
		//std::cout << "\n";																												//DEBUG
		currentClause = currentClause->next;
	}
	
	//giving the assumptions to the solver
	nodeInt64_t *currentAssumption = problem->assumptions;
	while (currentAssumption != NULL)
	{
		process << currentAssumption->number << " 0\n";
		//std::cout << currentAssumption->number << " 0\n";																					//DEBUG
		currentAssumption = currentAssumption->next;
	}

	//closing the call of the solver
	//std::cout << "-----------------------------------------------" << std::endl;															//DEBUG
	process << redi::peof;
	
	//receiving the answer from the solver
	std::string line;
	while (process.peek() != EOF && std::getline(process, line)) {
		//std::cout << line << std::endl;																										//DEBUG
		if (line.rfind("c ", 0) == 0) {
            continue;
        }
        if (line.rfind("s ", 0) == 0) {
            if (line.rfind("UNSATISFIABLE") != std::string::npos) {
                return 20;
            }
        }
        if (line.rfind("v ", 0) == 0) {
            line.erase(0, 2);
            size_t pos = 0;
            while (line.length() > 0) {
                pos = line.find(" ");
                if (pos == std::string::npos) {
                    pos = line.length();
                }
                int var = stoi(line.substr(0, pos));
                if (var > 0) {
                    problem->solution->elements[var] = 1;
                }
                else if (var < 0) {
					problem->solution->elements[-var] = 0;
                }
                else {
                    break;
                }
                line.erase(0, pos + 1);
            }
        }
	}

	problem->isSolved = true;


	//printf("Calculated solution: ");																									//DEBUG
	//print_array(problem->solution);																										//DEBUG
	//printf("\n");																														//DEBUG
	return 10;
}


/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t ExternalSatSolver::solve_pstreams_with_assumptions(SATProblem_t *problem, nodeInt64_t *assumptions, string path_to_solver)
{
	nodeInt64_t *current = assumptions;
	while (current != NULL)
	{
		add_assumption(problem, current->number);
		current = current->next;
	}

	return solve_pstreams(problem, path_to_solver);
}



