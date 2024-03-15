#ifndef MEMORY_WATCHDOG_H
#define MEMORY_WATCHDOG_H

#include <stdlib.h>
#include <sys/resource.h>

/// <summary>
/// This method returns the currently used total memory of the program itself.
/// </summary>
/// <returns>Memory usage in kB</returns>
long get_mem_usage();

#endif