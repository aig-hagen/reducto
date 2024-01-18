#ifndef LIST_ACTIVE_ARG_SET_H
#define LIST_ACTIVE_ARG_SET_H

#include "Actives.h"
#include "List.h"

// This line is necessary to inform the compiler of the type, 
// which allows to recursively call the struct in it's own type definition
typedef struct nodeActives nodeActives_t;
/// <summary>
/// Data structure of a list of tupels {active arguments, set of arguments}
/// </summary>
typedef struct nodeActives {
    activeArgs_t *element;
    nodeActives_t *next;
    nodeActives_t *before;
} nodeActives_t;

/// <returns>A list of active arguments, which contains only one node without any element.</returns>
nodeActives_t *create_empty_lst_active_args();
/// <param name="element">The active arguments of a framework.</param>
/// <returns>Newly created list of structs for the tupel {active arguments, set of arguments}</returns>
nodeActives_t* create_lst_active_args(activeArgs_t *element);
/// <summary>
/// Deallocates the memory space used for a specified list.
/// </summary>
/// <param name="list">The list, which memory space is to be freed. </param>
/// <returns>EXIT_SUCCESS if the operation was successfull. EXIT_FAILURE otherwise.</returns>
uint8_t free_lst_active_args(nodeActives_t *list);
/// <summary>
/// Adds a specified element at the top a a specified list,
/// </summary>
/// <param name="list">The list, at which top an element will be added.</param>
/// <param name="element">The element to add.</param>
/// <returns>The new head of the list.</returns>
nodeActives_t* add_top_lst_active_args(nodeActives_t *head, activeArgs_t *element);
/// <summary>
/// Adds a specified element at the end of a specified list.
/// </summary>
/// <param name="list">The list, at which top an element will be added.</param>
/// <param name="element">The element to add.</param>
/// <returns>EXIT_SUCCESS if the operation was successfull. EXIT_FAILURE otherwise.</returns>
uint8_t add_end_lst_active_args(nodeActives_t *list, activeArgs_t *element);
/// <summary>
/// Removes the first element from a specified list.
/// </summary>
/// <param name="list">List from which the first element is to be removed.</param>
/// <returns>The new head of the list.</returns>
nodeActives_t* remove_top_lst_active_args(nodeActives_t *list);
/// <summary>
/// Removes the last element from a specified list.
/// </summary>
/// <param name="list">List from which the last element is to be removed.</param>
/// <returns>EXIT_SUCCESS if the operation was successfull. EXIT_FAILURE otherwise.</returns>
uint8_t remove_end_lst_active_args(nodeActives_t *list);
/// <summary>
/// Removes the specified element from the specified list.
/// </summary>
/// <param name="list">The list from which the element is removed.</param>
/// <param name="element">The element to remove from the list.</param>
/// <returns>EXIT_SUCCESS if the operation was successfull. EXIT_FAILURE if the specified element was not found in the list.</returns>
uint8_t remove_element_lst_active_args(nodeActives_t *list, nodeActives_t *element);
/// <summary>
/// Searches the node in the list, which is containing the specified element.
/// </summary>
/// <param name="element">The element to find in the list.</param>
/// <param name="out_node">The node in the list, which is containing the specified element.<\param>
/// <returns>EXIT_SUCCESS if the element was found in the list. EXIT_FAILURE if the element was not found in the list.</returns>
uint8_t search_lst_active_args(nodeActives_t *list, activeArgs_t *element, nodeActives_t *out_node);

#endif