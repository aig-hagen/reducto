#include "../../include/logic/SatSolver_cadical.h"

SatSolver_cadical::SatSolver_cadical(uint64_t number_arguments) {
    _num_arguments = number_arguments;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


void SatSolver_cadical::add_clause(std::vector<int64_t> &clause) {
    // Add the clause to the CMS solver
    for (auto literal : clause) {
        cadical_solver.add(literal);
    }
    cadical_solver.add(0); // Terminate clause with 0
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


void SatSolver_cadical::add_clause_short(int64_t lit1, int64_t lit2){
    cadical_solver.add(lit1);
    
    if (lit2 != 0)
    {
        cadical_solver.add(lit2);
    }
    
    cadical_solver.add(0); // Terminate clause with 0
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


std::vector<bool> SatSolver_cadical::get_model() {
    std::vector<bool> model;
    // Get the model from the CMS solver
    for (int i = 1; i < _num_arguments + 1; ++i) {
        model.push_back(cadical_solver.val(i) > 0);
    }
    return model;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

std::uint8_t SatSolver_cadical::check_var_model(int64_t variable) {
    return cadical_solver.val(variable) > 0;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


bool SatSolver_cadical::solve() {
    // Solve the SAT problem using the CMS solver
    return cadical_solver.solve() == 10; // Returns true if a solution is found, false otherwise
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolver_cadical::solve(int64_t assumption) {
    cadical_solver.assume(assumption);
    return solve();
}


bool SatSolver_cadical::solve(std::vector<int64_t> assumptions) {
    for (int i = 0; i < assumptions.size(); i++) {
        cadical_solver.assume(assumptions[i]);
    }
    return solve();
}