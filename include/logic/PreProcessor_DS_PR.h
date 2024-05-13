#ifndef PREPROC_DS_PR_H
#define PREPROC_DS_PR_H

#include <iostream>
#include <cstdint>
#include <list>

#include "AF.h"
#include "Reduct.h"

#include "omp.h"

#include "../logic/Enums.h"

enum pre_proc_result { accepted, rejected, unknown };

class PreProc_DS_PR {
public:
	//returns 1 if argument was accepted, 2 if argument was  
	static pre_proc_result process(AF &framework, unordered_set<uint32_t> &active_args, uint32_t argument);
};
#endif