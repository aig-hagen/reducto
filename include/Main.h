/*!
 * Copyright (c) <2024> <Lars Bengel & Julian Sander, University of Hagen>
 *
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <getopt.h>

extern "C" {
	#include "../include/util/MemoryWatchDog.h"
}

#include "../include/logic/AF.h"
#include "../include/logic/Parser_iccma.h"
#include "../include/logic/Solver_DC_CO.h"
#include "../include/logic/Solver_DC_ST.h"
#include "../include/logic/Solver_DS_PR.h"
#include "../include/logic/Solver_DS_ST.h"
#include "../include/logic/Solver_SE_PR.h"
#include "../include/logic/Solver_SE_ST.h"

#include "../include/logic/Enums.h"

constexpr auto PROGRAM_NAME = "ParallelSolver";
constexpr auto VERSIONNUMBER = "1.46";
/// <summary>
/// Number of cores requested to use during the computation of a solution.
/// Keep in mind that the actual number of cores used depends on the 
/// OS scheduler and might be lower. If value == 0, then the system 
/// is free to choose the /// number of cores automatically without any 
/// restrictions.
/// </summary>
constexpr auto NUM_CORES = 0;

/// <summary>
/// Flags used for internal processing.
/// </summary>
static int version_flag = 0;
static int usage_flag = 0;
static int formats_flag = 0;
static int problems_flag = 0;

/// <summary>
/// Different options that can be added to a execution call of this application.
/// </summary>
const struct option longopts[] =
{
	{"help", no_argument, &usage_flag, 1},
	{"version", no_argument, &version_flag, 1},
	{"formats", no_argument, &formats_flag, 1},
	{"problems", no_argument, &problems_flag, 1},
	{"p", required_argument, 0, 'p'},
	{"f", required_argument, 0, 'f'},
	{"fo", required_argument, 0, 'o'},
	{"a", required_argument, 0, 'a'},
	{0, 0, 0, 0}
};

/// <summary>
/// This method is used to start the program.
/// </summary>
/// <param name="argc">Number of arguments with which the program got started.</param>
/// <param name="argv">Array of strings, containing the different starting arguments of this progam.</param>
/// <returns>0 iff the program exited without error. 1 otherwise.</returns>
int main(int argc, char **argv);

int CheckQuery(std::string &query, char **argv, bool &retFlag);

void print_proof(std::__cxx11::list<uint32_t> &proof_extension);

#endif