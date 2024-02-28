#include "../../include/util/List.h"

uint8_t appendNode(nodeUInt32_t *predecessor, uint32_t number)
{
    nodeUInt32_t *ptrNode = NULL;
    ptrNode = (nodeUInt32_t *)malloc(sizeof * ptrNode);
    if (ptrNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        predecessor->next = ptrNode;
        ptrNode->number = number;
        ptrNode->next = NULL;

        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeUInt32_t *copy_list_uint32(nodeUInt32_t *head)
{
    nodeUInt32_t *out_head = create_list_uint32(head->number);
    nodeUInt32_t *ptrOut = out_head;

    nodeUInt32_t *p = NULL;
    for (p = head->next; p; p = p->next)
    {
        appendNode(ptrOut, p->number);
        ptrOut = ptrOut->next;
    }

    return out_head;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint32_t count_list_uint32(nodeUInt32_t *head)
{
    nodeUInt32_t *current = head;
     uint32_t count = 1;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
        count++;
    }

    return count;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint32_t count_list_int64(nodeInt64_t *head)
{
    nodeInt64_t *current = head;
    uint32_t count = 1;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
        count++;
    }

    return count;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeUInt32_t *create_list_uint32(uint32_t number)
{
    nodeUInt32_t *head = NULL;
    head = (nodeUInt32_t*) malloc(sizeof *head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->next = NULL;
        head->number = number;
        return head;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

listUInt32_t *create_list_list_uint32(nodeUInt32_t *element)
{
    listUInt32_t *head = NULL;
    head = (listUInt32_t *)malloc(sizeof * head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->list = element;
        head->next = NULL;

        return head;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeInt64_t *create_list_int64(int64_t number)
{
    nodeInt64_t *head = NULL;
    head = (nodeInt64_t*)malloc(sizeof * head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->next = NULL;
        head->number = number;
        return head;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

listInt64_t* create_list_list_int64(nodeInt64_t *element)
{
    listInt64_t *head = NULL;
    head = (listInt64_t*) malloc(sizeof * head);
    if (head == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        head->list = element;
        head->next = NULL;

        return head;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t free_list_uint32(nodeUInt32_t *head)
{
    nodeUInt32_t *current = head;
    nodeUInt32_t *next = current->next;

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

uint8_t free_list_list_uint32(listUInt32_t *head)
{
    listUInt32_t *current = head;
    listUInt32_t *next = current->next;

    while (next != NULL)
    {
        free_list_uint32(current->list);
        free(current);
        current = next;
        next = current->next;
    }
    free_list_uint32(current->list);
    free(current);

    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t free_list_int64(nodeInt64_t *head)
{
    if (head == NULL)
    {
        return EXIT_SUCCESS;
    }

    nodeInt64_t *current = head;
    nodeInt64_t *next = current->next;

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

uint8_t free_list_list_int64(listInt64_t *head)
{
    listInt64_t *current = head;
    listInt64_t *next = current->next;

    while (next != NULL)
    {
        free_list_int64(current->list);
        free(current);
        current = next;
        next = current->next;
    }
    free_list_int64(current->list);
    free(current);

    return EXIT_SUCCESS;
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t push_uint32(nodeUInt32_t *head, uint32_t number)
{
    nodeUInt32_t *current = head;
    while (current -> next != NULL) { // iterate to end of list
        current = current -> next;
    }

    return appendNode(current, number);
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t push_list_uint32(listUInt32_t *head, nodeUInt32_t *list)
{
    listUInt32_t *current = head;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
    }

    listUInt32_t *ptrNode = NULL;
    ptrNode = (listUInt32_t *)malloc(sizeof * ptrNode);
    if (ptrNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        current->next = ptrNode;
        current = current->next;
        current->list = list;
        current->next = NULL;

        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t push_int64(nodeInt64_t *head, int64_t number)
{
    nodeInt64_t *current = head;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
    }

    nodeInt64_t *ptrNode = NULL;
    ptrNode = (nodeInt64_t*) malloc(sizeof *ptrNode );
    if (ptrNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        current->next = ptrNode;
        current = current->next;
        current->number = number;
        current->next = NULL;

        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t push_list_int64(listInt64_t *head, nodeInt64_t *list)
{
    listInt64_t *current = head;
    while (current->next != NULL) { // iterate to end of list
        current = current->next;
    }

    listInt64_t *ptrNode = NULL;
    ptrNode = (listInt64_t*) malloc(sizeof *ptrNode);
    if (ptrNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    else {
        current->next = ptrNode;
        current = current->next;
        current->list = list;
        current->next = NULL;

        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t print_list_uint32(nodeUInt32_t *head)
{
    if (head == NULL)
    {
        return EXIT_FAILURE;
    }
    else 
    {
        nodeUInt32_t *current = head;
        printf("{ ");
        printf("%d", current->number);
        while (current->next != NULL)
        {
            current = current->next;
            printf(", %d", current->number);
        }
        printf(" }");
        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t print_list_elements_uint32(nodeUInt32_t *head)
{

    if (head == NULL)
    {
        return EXIT_FAILURE;
    }
    else
    {
        nodeUInt32_t *current = head;
        printf(" %d", current->number);
        while (current->next != NULL)
        {
            current = current->next;
            printf(" %d", current->number);
        }
        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t print_list_list_uint32(listUInt32_t *head)
{
    {
        if (head == NULL)
        {
            return EXIT_FAILURE;
        }
        else
        {
            listUInt32_t *current = head;
            printf("{ ");
            print_list_uint32(current->list);
            while (current->next != NULL)
            {
                current = current->next;
                printf(", ");
                print_list_uint32(current->list);
            }
            printf(" }");
            return EXIT_SUCCESS;
        }
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t print_list_int64(nodeInt64_t *head)
{
    if (head == NULL)
    {
        return EXIT_FAILURE;
    }
    else
    {
        nodeInt64_t *current = head;
        printf("{ ");
        printf("%ld", current->number);
        while (current->next != NULL)
        {
            current = current->next;
            printf(", %ld", current->number);
        }
        printf(" }");
        return EXIT_SUCCESS;
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

uint8_t print_list_list_int64(listInt64_t *head)
{
    {
        if (head == NULL)
        {
            return EXIT_FAILURE;
        }
        else
        {
            listInt64_t *current = head;
            printf("{ ");
            print_list_int64(current->list);
            while (current->next != NULL)
            {
                current = current->next;
                printf(", ");
                print_list_int64(current->list);
            }
            printf(" }");
            return EXIT_SUCCESS;
        }
    }
}

/*===========================================================================================================================================================*/
/*===========================================================================================================================================================*/

nodeUInt32_t *remove_head_list_uint32(nodeUInt32_t *head)
{
    nodeUInt32_t *output = head->next;
    free(head);
    return output;
}