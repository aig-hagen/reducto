#ifndef SAT_CRYPTO_H
#define SAT_CRYPTO_H

#ifdef SAT_CMSAT

#include "SatSolver.h"
#include <cryptominisat.h>

class SatSolverCrypto : public SatSolver {

private:
	CMSat::SATSolver solver;
	std::vector<CMSat::Lit> assumptions;
	int32_t num_vars;
public:
	SatSolverCrypto(uint32_t n_vars);
	~SatSolverCrypto() { };
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