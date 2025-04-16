#ifndef SAT_GLUCOSE
#define SAT_GLUCOSE


#include "SatSolver.h"
#include "../../sat/glucose/core/Solver.h"

class SatSolverGlucose : public SatSolver {

private:
	Glucose::Solver *solver;
	Glucose::vec<Glucose::Lit> assumptions;
	std::vector<bool> model;
	int32_t num_vars;
public:
	SatSolverGlucose(int32_t n_vars);
	~SatSolverGlucose() { delete solver; };
	void add_assumption(int64_t assumption);
	void add_clause(std::vector<int64_t> &clause);
	void add_clause_short(int64_t lit1, int64_t lit2);
	std::uint8_t check_var_model(int64_t variable);
	bool solve();
	bool solve(int64_t assumption);
	bool solve(int64_t assumption1, int64_t assumption2);

private:
	void add_clause_1(int32_t lit);
	void add_clause_2(int32_t lit1, int32_t lit2);
};
#endif
