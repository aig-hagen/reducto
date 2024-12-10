#ifndef ENUMS_H
#define ENUMS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <iostream>

/// <summary>
/// Abbreviations of the different type of tasks, which the solver can solve
/// </summary>
enum task { DC, DS, SE, UNKNOWN_TASK };
/// <summary>
/// Abbreviations of the different semantics of the solutions to calculate.
/// </summary>
enum semantics { PR, CO, ST, UNKNOWN_SEM };

/// <summary>
/// Return values of the preprocessor, indicating if the preprocessor solved the problem, and if so to what conclusion
/// </summary>
enum pre_proc_result { accepted, rejected, unknown };

using namespace std;

/// <summary>
/// This class is responsible for converting to and from Enum-Data-Types.
/// </summary>
class Enums {
public:

	/// <summary>
	/// This method converts the specified string into a task.
	/// </summary>
	/// <param name="task_abbreviation">String containing the abbreviation of the task as used in the enum.</param>
	/// <returns>Task corresponding to the specified string.</returns>
	static task string_to_task(string task_abbreviation);

	/// <summary>
	/// This method converts the specified string into a semantics.
	/// </summary>
	/// <param name="semantics_abbreviation">String containing the abbreviation of the semantics as used in the enum.</param>
	/// <returns>Semantics corresponding to the specified string.</returns>
	static semantics string_to_sem(string semantics_abbreviation);
};

#endif