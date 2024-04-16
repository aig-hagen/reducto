#include "../../include/logic/SatSolver_cms.h"

using namespace std;
using std::vector;
using namespace CMSat;

SatSolver_cms::SatSolver_cms(uint64_t num_var) {
    // Initialize the cryptominisat solver with the number of variables
    cms_solver.set_num_threads(1); // Set the number of threads to 1 for simplicity
    cms_solver.new_vars((2 * num_var) + 1);
}

void SatSolver_cms::add_clause(std::vector<int64_t> clause) {
    // Add the clause to the cryptominisat solver
    std::vector<Lit> lits;
    for (auto lit : clause) {
        lits.push_back(Lit(abs(lit) - 1, lit > 0)); // Convert literals to cryptominisat format
    }
    cms_solver.add_clause(lits);
    lits.clear();
}

std::vector<bool> SatSolver_cms::get_model() {
    std::vector<bool> model;
    // Get the model from the cryptominisat solver
    for (int i = 0; i < cms_solver.nVars(); ++i) {
        model.push_back(cms_solver.get_model()[i] == l_True);
    }
    return model;
}

bool SatSolver_cms::solve() {
    // Solve the SAT problem using the cryptominisat solver
    return cms_solver.solve() == l_True; // Return true if a solution is found, false otherwise
}