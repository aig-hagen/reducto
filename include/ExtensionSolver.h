#ifndef EXTENSION_SOLVER_H
#define EXTENSION_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
	#include "AF.h"
	#include "List.h"
	#include "Reduct.h"
}

#include "Encodings_cms.h"
#include "InitialSetSolver.h"
#include "Decodings_cms.h"

#include <cryptominisat.h>
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
	static void BuildExtension(argFramework_t *framework, activeArgs_t *activeArgs, nodeUInt32_t **extension);
};

#endif