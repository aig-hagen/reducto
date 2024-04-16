#ifndef EXTENSION_SOLVER_H
#define EXTENSION_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
	#include "AF.h"
	#include "Reduct.h"
	#include "../util/List.h"
}

#include "Encodings_SatSolver.h"
#include "InitialSetSolver.h"
#include "Decodings_SatSolver.h"
#include "SatSolver.h"
#include "SatSolver_cms.h"
#include "SatSolver_cadical.h"

#include "../util/Enums.h"


#include <vector>

using namespace CMSat;

class EXTENSIONSOLVER_CMS {
public:

	/// <summary>
	/// Extends a specified extension to complete it.
	/// </summary>
	/// <param name="framework"></param>
	/// <param name="activeArgs"></param>
	/// <param name="extension"></param>
	static void BuildExtension(argFramework_t *framework, activeArgs_t *activeArgs, nodeUInt32_t **extension, SOLVERS solver_type);
};

#endif