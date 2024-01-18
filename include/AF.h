#ifndef AF_H
#define AF_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Actives.h"
#include "List.h"
#include "Matrix.h"
#include "LinkedSparseMatrix.h"

// This line is necessary to inform the compiler of the type, 
// which allows to recursively call the struct in it's own type definition
typedef struct argumentInitTemp argumentInitTemp_t;
/// <summary>
/// temporary data structure for initialization
/// <br></br> - <br></br>
/// This struct is used during the initialization to keep track of all attacks, 
/// which will be read one after the other from the input-file.
/// </summary>
typedef struct argumentInitTemp {
    uint32_t number;
    nodeUInt32_t *listAttackers;
    nodeUInt32_t *listVictims;
    argumentInitTemp_t *next;
} argumentInitTemp_t;

/// <summary>
/// data model of the framework
/// <br></br> - <br></br> 
/// This data structure is representing the initial abstract argumentation framework. It is only created once
/// and stays unchanged throughout the whole process of calculation. However, this structure gets
/// accessed by every parallel calculation several times.
/// <br></br> - <br></br> 
/// The attack relations are stored in two "linked sparse matrices", allowing fast access to attackers
/// and victims of an argument.
/// <br></br> - <br></br> 
///  ! Pay attention to the definition of a "linked sparse matrice" !
/// </summary>
typedef struct argFramework {
    //datamodel of the framework
    uint32_t number;
    matrix_t *attackers; // 2D-matrix containing the index of arguments
    matrix_t *victims; // 2D-matrix containing the index of arguments
} argFramework_t;

/// <summary>
/// Adds an attack to the specified initialization data structure. 
/// </summary>
/// <param name="head">List of arguments, used as initialization data structure.</param>
/// <param name="attacker">Number of the argument, which is the attacker.</param>
/// <param name="victim">Number of the argument, which is being attacked.</param>
/// <returns> EXIT_SUCCESS iff the operation was successful. EXIT_FAILURE if any of the specified arguments couldn't be found in the specified list of arguments.</returns>
uint8_t add_attack(argumentInitTemp_t *head,  uint32_t attacker,  uint32_t victim);
/// <summary>
/// Initializes the data model of the framework based on the specified data structure of the initialization phase.
/// </summary>
/// <param name="head">List of arguments, used during initialization.</param>
/// <returns>The data model of the framework.</returns>
argFramework_t* initialize_framework(argumentInitTemp_t *head);
/// <summary>
/// Initializes the data structure used during the initializing phase, during which the structure of the framework gets
/// sequentially build up.
/// </summary>
/// <param name="numberArguments">Number of arguments in the framework.</param>
/// <returns>List of arguments in the framework to build.</returns>
argumentInitTemp_t* set_up_initialization(uint32_t numberArguments);

#endif

