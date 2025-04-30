#ifndef SAT_GLUCOSE_H
#define SAT_GLUCOSE_H

#ifdef SAT_GLUCOSE

#include "SatSolver.h"
#include <SimpSolver.h>

class SatSolverGlucose : public SatSolver {

private:
	Glucose::SimpSolver *solver;
	std::vector<Glucose::Lit> assumptions;
	std::vector<bool> model;
	int32_t num_vars;
public:
	SatSolverGlucose(uint32_t num_args);
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
#endif