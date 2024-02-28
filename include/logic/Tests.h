#ifndef TESTS_H
#define TESTS_H

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>

extern "C" {
#include "Actives.h"
#include "AF.h"
#include "Decodings.h"
#include "Encodings.h"
#include "Reduct.h"
#include "SatProblem.h"

#include "../util/Array.h"
#include "../util/LinkedSparseMatrix.h"
#include "../util/List.h"
#include "../util/Matrix.h"
}

#include "ExtensionSolver.h"
#include "ExternalSolver.h"
#include "InitialSetSolver.h"
#include "Parser_iccma.h"
#include "ScepticalPRSequential.h"
#include "ScepticalPRParallel.h"

using namespace std;

class TestCases {
public:

	/// <summary>
	/// Runs different test cases on the solver
	/// </summary>
	static void run_Tests();
};

#endif
