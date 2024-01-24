#ifndef EXTERNAL_SOLVER_H
#define EXTERNAL_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
    #include "SatProblem.h"
}

//#include "../lib/pstreams-1.0.3/pstream.h"
//#include "../lib/cryptominisat-5.11.15/src/cryptominisat.h"
#include "pstream.h"
#include <cryptominisat.h>
#include <assert.h>
#include <vector>
using std::vector;
using namespace CMSat;

/*
Class for all kinds of pre-compiled SAT solvers, e.g. cadical, cryptominisat5
SAT calls are answered by opening a pipe to an instance of the external solver with pstream
*/
class ExternalSatSolver {
public:    
    /// <summary>
    /// Computes the solution of the specified SAT-problem using the library pstreams.
    /// This method is not recommended fpr parallel execution, since psteams uses
    /// the stdout/stdin of the process for each thread.
    /// </summary>
    /// <param name="problem">The SAT-problem to solve.</param>
    /// <returns>Returns '10' if the operation was successful. Otherwise returns '20'.</returns>
    static uint8_t solve_pstreams(SATProblem_t *problem, std::string path_to_solver);
    
       
    /// <summary>
    /// Computes the solution of the specified SAT-problem combined with the specified assumptions,
    /// using the library pstreams.
    /// This method is not recommended fpr parallel execution, since psteams uses
    /// the stdout/stdin of the process for each thread.
    /// </summary>
    /// <param name="problem">The SAT-problem to solve.</param>
    /// <param name="assumptions">The assumptions to take into account.</param>
    /// <returns>
    /// Returns '10' if the operation was successful. Otherwise returns '20'.
    /// </returns>
    static uint8_t solve_pstreams_with_assumptions(SATProblem_t *problem, nodeInt64_t *assumptions, std::string path_to_solver);

    /// <summary>
    /// Computes the solution of the specified SAT-problem using the library cryptominisat.
    /// This method is the preferred option for parallel execution.
    /// </summary>
    /// <param name="problem">The SAT-problem to solve.</param>
    /// <returns>
    /// Returns '10' if the operation was successful. Otherwise returns '20'.
    /// </returns>
    static uint8_t solve_cms(SATProblem_t *problem);
};

#endif
