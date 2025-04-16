#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include <iostream>
#include <cstdint>
#include <vector>

class SatSolver {
public:
	virtual ~SatSolver() {}
	virtual void add_assumption(int64_t assumption) = 0;
	virtual void add_clause(std::vector<int64_t> &clause) = 0;
	virtual void add_clause_short(int64_t lit1, int64_t lit2) = 0;
	virtual std::uint8_t check_var_model(int64_t variable) = 0;
	virtual bool solve() = 0;
	virtual bool solve(int64_t assumption) = 0;
	virtual bool solve(int64_t assumption1, int64_t assumption2) = 0;
};

#endif