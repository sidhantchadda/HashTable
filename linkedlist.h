#ifndef LINKED_LIST_H
#define LINKED_LIST_H
/* Header guard, prevents accidental double inclusion */

/**
 * Structure to hold both the internal linkedlist nodes, and the
 * transaction data we want to hold within the node.
 * Note that based on the transaction_type field, some of the
 * following fields may be ununused
 */
typedef struct node {
    /* Internal linked list information */
    struct node *next; // Pointer to next node in the list
    struct node *prev; // Pointer to previous node in the list

    /*
     * Data we are storing in the node
     */

    /* Transaction timestamp */
    long int timestamp;
    /* The type of transaction (ADD, REMOVE, TRANSFER) */
    short transaction_type;
    /* First company for transaction (Used with all transaction types) */
    char *company1; 
    /* Second company for transaction (Only used for TRANSFER transaction) */
    char *company2;
    /* Value for transfer transaction (Only used for TRANSFER transaction) */
    double value;
} node_t;

/**
 * Structure to represent the linked list, containing pointers
 * to both the head and tail nodes
 *
 * Below is a diagram of how the nodes (numbered 1-6) are connected:
 *
 *        head                          tail
 *         |                             |
 *         \/                            \/
 * NULL <- 1 <-> 2 <-> 3 <-> 4 <-> 5 <-> 6 -> NULL
 *
 * Note that the arrows between nodes point in both directions.
 * For example, the next pointer of node 4 points to node 5, and
 * the prev pointer of node 5 points to node 4.
 * 
 * In an empty linked list, both head and tail would both point to NULL
 */
typedef struct linkedlist {
    node_t *head;
    node_t *tail;
} linkedlist_t;

/**
 * Create a newly allocated empty linkedlist_t with head and tail initialized to NULL
 * Parameters:
 *      None
 * Returns:
 *      A pointer to an allocated linked_list_t structure, 
 *      the caller is responsible for deallocating using
 *      free_linkedlist()
 */
linkedlist_t *create_linkedlist();

/**
 * Free the memory allocated for the linkedlist_t object. This includes
 * using delete_node() to delete all the nodes and the values in the nodes
 * Parameters:
 *      list - Pointer to the linkedlist object to
 * Returns:
 *      Nothing
 */
void free_linkedlist(linkedlist_t *list);

/**
 * Add a linkedlist node to the tail of the list. After this function
 * returns, list->tail should be the same as node.
 * Parameters:
 *      list - Pointer to the linkedlist we are adding the node to
 *      node - Pointer to the node we are appending to the tail of the list
 * Return:
 *      0 if the insertion is successful -1 otherwise or if either parameter is NULL
 */
int add_node(linkedlist_t *list, node_t *node);

/**
 * Given a pointer to a linkedlist node and the linkedlist, remove
 * the node from the list. This does not deallocate the node or it's
 * contents.
 * Parameters:
 *      list - Pointer to the linkedlist we are removing the node from
 *      node - Pointer to the node we are removing from the list
 * Return:
 *      0 if the removal is successful, or -1 if the removal failed or either parameter is NULL
 */
int remove_node(linkedlist_t *list, node_t *node);

/**
 * Create a new node containing the given values
 * Parameters:
 *      timestamp   - The timestamp of the transaction
 *      transaction_type - The value indicating the type of transaction
 *      company1    - Pointer to the string containing the name of the first company in the transaction
 *      company2    - Pointer to the string containing the name of the second company in the transaction
 *      value       - Numeric value for either the initial balance (ADD) or transfer amount (TRANSFER)
 * Return:
 *      Return a pointer to a newly allocated node_t initialized with the parameters.
 *      Return NULL if memory allocation failed, otherwise return a pointer to the node
 * Remarks:
 *      For both company1 and company2, ownership of the memory pointed to
 *      by company1 and company2 is not transfered to this function meaning
 *      that after this function call returns, it is possible that the memory
 *      is modified, so a copy of the underlying memory must be performed.
 *      Both company1 and company2 are allowed to be NULL.
 */
node_t * create_node(long int timestamp, short transaction_type, const char *company1, const char *company2, double value);

/**
 * Delete a node, including any strings or values it contains
 * Parameters:
 *      node - Pointer to the node that should be deleted
 * Remarks:
 */
void delete_node(node_t *node);

/**
 * Find a node based on the timestamp.
 * Parameters:
 *      list - The linkedlist to search for the given timestamp
 *      timestamp - The timestamp to search for in the linked list
 * Return:
 *      Return a pointer to the node if a node with the given timestamp
 *      can be found, otherwise return NULL if no node with that
 *      timestamp is found or the arguments are invalid.
 */
node_t* find_node(linkedlist_t *list, long int timestamp);

#endif
