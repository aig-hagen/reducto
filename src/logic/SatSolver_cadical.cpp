#include "../../include/logic/SatSolver_cadical.h"

SatSolver_cadical::SatSolver_cadical(uint64_t number_variables) {
    _num_variables = number_variables;
}

void SatSolver_cadical::add_clause(std::vector<int64_t> clause) {
    // Add the clause to the CMS solver
    for (auto literal : clause) {
        cadical_solver.add(literal);
    }
    cadical_solver.add(0); // Terminate clause with 0
}

std::vector<bool> SatSolver_cadical::get_model() {
    std::vector<bool> model;
    // Get the model from the CMS solver
    for (int i = 1; i < _num_variables + 1; ++i) {
        model.push_back(cadical_solver.val(i) > 0);
    }
    return model;
}

bool SatSolver_cadical::solve() {
    // Solve the SAT problem using the CMS solver
    return cadical_solver.solve() == 10; // Return true if a solution is found, false otherwise
}