
#ifdef SAT_GLUCOSE

#include "../../include/logic/SatSolverGlucose.h"

using namespace Glucose;

SatSolverGlucose::SatSolverGlucose(uint32_t num_args)
{
    solver = new Glucose::SimpSolver;
    solver->setIncrementalMode();
    solver->initNbInitialVars(2 * num_args);
    num_vars = num_args;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolverGlucose::add_assumption(int64_t lit) {
    int32_t var = abs(lit) - 1;
    while (var >= solver->nVars())
        solver->newVar();
    assumptions.push_back(mkLit(var, (lit < 0)));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolverGlucose::add_clause(std::vector<int64_t> &clause) {
    vec<Lit> lits(clause.size());
    for (uint32_t i = 0; i < clause.size(); i++) {
        int32_t var = abs(clause[i]) - 1;
        while (var >= solver->nVars())
            solver->newVar();
        lits[i] = mkLit(var, (clause[i] < 0));
    }
    solver->addClause_(lits);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolverGlucose::add_clause_1(int32_t lit) {
    int32_t var = abs(lit) - 1;
    while (var >= solver->nVars())
        solver->newVar();
    solver->addClause(mkLit(var, (lit < 0)));
}

void SatSolverGlucose::add_clause_2(int32_t lit1, int32_t lit2) {
    int32_t var1 = abs(lit1) - 1;
    int32_t var2 = abs(lit2) - 1;
    while (var1 >= solver->nVars())
        solver->newVar();
    while (var2 >= solver->nVars())
        solver->newVar();
    solver->addClause(mkLit(var1, (lit1 < 0)), mkLit(var2, (lit2 < 0)));
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

void SatSolverGlucose::add_clause_short(int64_t lit1, int64_t lit2) {

    if (lit2 == 0)
    {
        add_clause_1(lit1);
    }
    else
    {
        add_clause_2(lit1, lit2);
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

std::uint8_t SatSolverGlucose::check_var_model(int64_t variable) {
    return model[variable - 1];
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/


bool SatSolverGlucose::solve() {
    vec<Lit> assume;
    for (std::vector<Glucose::Lit>::size_type i = 0; i < assumptions.size(); i++) {
        assume.push(assumptions[i]);
    }
    bool is_satisfiable = solver->solve(assume, false, false);
    if (is_satisfiable) {
        model.clear();
        for (int32_t i = 0; i < num_vars; i++) {
            model.push_back(solver->modelValue(i) == l_True ? true : false);
        }
    }
    assumptions.clear();
    return is_satisfiable; // Returns true if a solution is found, false otherwise
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolverGlucose::solve(int64_t assumption) {
    add_assumption(assumption);
    return solve();
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

bool SatSolverGlucose::solve(int64_t assumption1, int64_t assumption2) {
    add_assumption(assumption1);
    add_assumption(assumption2);
    return solve();
}

#endif