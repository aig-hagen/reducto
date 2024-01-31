#ifndef EXTERNAL_SOLVER_H
#define EXTERNAL_SOLVER_H

#include <iostream>
#include <stdio.h>
#include <stdint.h>

extern "C" {
    #include "SatProblem.h"
}

#include "pstream.h"

/*
Class to solve a SAT-Problem with different kinds of techniques, such as using pstreams
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
};

#endif
