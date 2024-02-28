#include "../../include/logic/ListActives.h"

nodeActives_t *create_empty_lst_active_args()
{
    nodeActives_t *head = NULL;
    head = (nodeActives_t *)malloc(sizeof * head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->element = NULL;
        head->next = NULL;
        head->before = NULL;
        return head;
    }
}


/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeActives_t * create_lst_active_args(activeArgs_t *element)
{
    nodeActives_t *head = NULL;
    head = (nodeActives_t *)malloc(sizeof * head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->element = element;
        head->next = NULL;
        head->before = NULL;
        return head;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t free_lst_active_args(nodeActives_t *list)
{
    nodeActives_t *current = list;
    nodeActives_t *next = current->next;

    while (next != NULL)
    {
        free(current);
        current = next;
        next = current->next;
    }
    free(current);

    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeActives_t* add_top_lst_active_args(nodeActives_t *head, activeArgs_t *element)
{
    nodeActives_t *out_head = create_lst_active_args(element);
    out_head->next = head;
    head->before = out_head;
    return out_head;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t add_end_lst_active_args(nodeActives_t *head, activeArgs_t *element)
{
    nodeActives_t *current = head;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
    }
    nodeActives_t *newNode = create_lst_active_args(element);
    current->next = newNode;
    newNode->before = current;
    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeActives_t *remove_top_lst_active_args(nodeActives_t *list)
{
    nodeActives_t *out_head = list->next;
    if (out_head != NULL)
    {
        out_head->before = NULL;
        free(list);
        return out_head;
    }
    else
    {
        free(list);
        return create_empty_lst_active_args();
    }
    
    
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t remove_end_lst_active_args(nodeActives_t *list)
{
    nodeActives_t *current = list;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
    }

    if (current->before != NULL)
    {
        current->before->next = NULL;
    }
    
    free(current);
    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t remove_element_lst_active_args(nodeActives_t *list, nodeActives_t *element)
{
    nodeActives_t *current = list;
    while (true) {
        if (current == NULL || current == element)
        {   // iterate to end of list or to the element
            break;
        }

        current = current->next;
    }

    if (current == NULL)
    {
        // element not found in list
        return EXIT_FAILURE;
    }
    if (current->before != NULL)
    {
        current->before->next = current->next;
    }
    
    if (current->next != NULL)
    {
        current->next->before = current->before;
    }

    free(current);
    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t search_lst_active_args(nodeActives_t *list, activeArgs_t *element, nodeActives_t *out_node)
{
    nodeActives_t *current = list;
    while (current->next != NULL && current->element != element) { // iterate to end of list
        current = current->next;
    }

    if (current == NULL)
    {
        // element not found in list
        return EXIT_FAILURE;
    }

    *out_node = *current;
    return EXIT_SUCCESS;
}
