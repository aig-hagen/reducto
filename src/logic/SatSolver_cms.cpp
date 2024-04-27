#include "../../include/logic/SatSolver_cms.h"

using namespace std;
using std::vector;
using namespace CMSat;

SatSolver_cms::SatSolver_cms(uint64_t num_var) {
    // Initialize the cryptominisat solver with the number of variables
    cms_solver.set_num_threads(1); // Set the number of threads to 1 for simplicity
    cms_solver.new_vars((2 * num_var) + 1);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint32_t getAbsolut(int64_t variable) {
    int64_t abs_var64 = variable < 0 ? ((-1) * variable) : variable;
    return static_cast<uint32_t>(abs_var64);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver_cms::AddVariableToClause(int64_t variable, std::vector<CMSat::Lit> &lits)
{
    uint32_t abs_var = getAbsolut(variable);
    lits.push_back(Lit(abs_var - 1, variable < 0)); // Convert literals to cryptominisat format
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver_cms::add_clause(std::vector<int64_t> clause) {
    // Add the clause to the cryptominisat solver
    std::vector<Lit> lits;
    for (auto variable : clause) {
        AddVariableToClause(variable, lits);
    }
    cms_solver.add_clause(lits);
    lits.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolver_cms::add_clause_short(int64_t lit1, int64_t lit2) {
    
    std::vector<Lit> lits;
    
    AddVariableToClause(lit1, lits);

    if (lit2 != 0)
    {
        AddVariableToClause(lit2, lits);
    }

    cms_solver.add_clause(lits);
    lits.clear();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


std::vector<bool> SatSolver_cms::get_model() {
    std::vector<bool> model;
    // Get the model from the cryptominisat solver
    for (int i = 0; i < cms_solver.nVars(); ++i) {
        model.push_back(cms_solver.get_model()[i] == l_True);
    }
    return model;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolver_cms::solve() {
    // Solve the SAT problem using the cryptominisat solver
    return cms_solver.solve() == l_True; // Return true if a solution is found, false otherwise
}