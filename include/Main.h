#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>

extern "C" {
#include "../include/Actives.h"
#include "../include/AF.h"
#include "../include/Array.h"
#include "../include/Decodings.h"
#include "../include/Encodings.h"
#include "../include/LinkedSparseMatrix.h"
#include "../include/List.h"
#include "../include/Matrix.h"
#include "../include/Reduct.h"
#include "../include/SatProblem.h"
}

#include "../include/ExternalSolver.h"
#include "../include/InitialSetSolver.h"
#include "../include/ScepticalPRSequential.h"
#include "../include/ScepticalPRParallel.h"


#endif

static void AnalyseSolvingAlgorithms(argFramework_t *framework, activeArgs_t *actives);
