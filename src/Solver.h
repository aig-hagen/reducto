#ifndef SOLVER_H
#define SOLVER_H

#include "ipafair.h"

#include "./logic/AF.h"
#include "./logic/Enums.h"


class SatSolver {
public:
	const char *ipafair_signature();

private:
	AF _framework;
	semantics _semantics;
};

#endif
