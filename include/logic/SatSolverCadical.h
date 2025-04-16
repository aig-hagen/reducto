#ifndef SAT_CADICAL
#define SAT_CADICAL


#include "SatSolver.h"
#include "../../sat/cadical/src/cadical.hpp"

class SatSolverCadical : public SatSolver {

private:
	CaDiCaL::Solver *solver;
public:
	SatSolverCadical();
	~SatSolverCadical() { delete solver; };
	void add_assumption(int64_t assumption);
	void add_clause(std::vector<int64_t> &clause);
	void add_clause_short(int64_t lit1, int64_t lit2);
	std::uint8_t check_var_model(int64_t variable);
	bool solve();
	bool solve(int64_t assumption);
	bool solve(int64_t assumption1, int64_t assumption2);
};
#endif
