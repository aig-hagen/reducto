#include "../../include/logic/SatSolver.h"

SatSolver::SatSolver() {
    _solver = ipasir_init();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver::add_assumption(int64_t assumption) {
    ipasir_assume(_solver, assumption);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver::add_clause(std::vector<int64_t> &clause) {
    // Add the clause to the CMS solver
    for (auto literal : clause) {
        ipasir_add(_solver, literal);
    }
    ipasir_add(_solver, 0); // Terminate clause with 0
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver::add_clause_short(int64_t lit1, int64_t lit2){
    ipasir_add(_solver, lit1);
    
    if (lit2 != 0)
    {
        ipasir_add(_solver, lit2);
    }
    
    ipasir_add(_solver, 0); // Terminate clause with 0
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

std::uint8_t SatSolver::check_var_model(int64_t variable) {
    return ipasir_val(_solver, variable) > 0;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


bool SatSolver::solve() {
    // Solve the SAT problem using the CMS solver
    return ipasir_solve(_solver) == 10; // Returns true if a solution is found, false otherwise
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolver::solve(int64_t assumption) {
    ipasir_assume(_solver, assumption);
    return solve();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolver::solve(int64_t assumption1, int64_t assumption2) {
    ipasir_assume(_solver, assumption1);
    ipasir_assume(_solver, assumption2);
    return solve();
}