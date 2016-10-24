#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linkedlist.h"

linkedlist_t* create_linkedlist() { 
    linkedlist_t* list = malloc(sizeof(linkedlist_t));
    list -> head = NULL;
    list -> tail = NULL;
    return list;
}

node_t * create_node(long int timestamp, short transaction_type, const char     *company1, const char *company2, double value) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node -> timestamp = timestamp;
    node -> transaction_type = transaction_type;
    node -> company1 = strdup(company1);
    node -> company2 = strdup(company2);
    node -> value = value;
    return node;
}

int add_node(linkedlist_t *list, node_t *node) {
    if(node == NULL || list == NULL) 
        return -1;
    //if the node is empty
    if(list-> head == NULL) {
        list->head= node;
        list -> tail = node;
        return 0;
    }
    node_t *temp = list->tail;
    temp -> next = node;
    node-> prev = temp;
    node -> next = NULL;
    list -> tail = node;
    return 0;
}
// do not deallocate space for the node just delete it
int remove_node(linkedlist_t *list, node_t *node) {
    //we should not need to traverse list becausae we have a pointer to node 
	//but in this case we need to traverse the list because we need to make sure that the given node is actually in the list and not just a node not connected to the list.
    if(list == NULL || node == NULL)
        return -1;
	if(list -> head == NULL && list -> tail == NULL) {
		return -1;
	}
	node_t* temp = list -> head;
	while(temp != NULL)	{
		if(temp  == node) {
			//if found
			//one case
			if(list -> head == list -> tail) {
				list -> head = NULL;
				list -> tail = NULL;
				return 0;
			}
			//head case
			if(list -> head == node) {
				node_t* next = node -> next;
				next -> prev = NULL;
				list -> head = next;
				return 0;
			}
			//tail case
			if(list -> tail == node) {
				node_t* prev = node -> prev;
				prev -> next = NULL;
				list -> tail = prev;
				return 0;
			}
			//normal case
			node_t* prev = node -> prev;
			node_t* next = node -> next;
			prev -> next = next;
			next -> prev = prev;
			return 0;
		}
		temp = temp -> next;
	}
	//if not found
	return -1;
}
void delete_node(node_t *node) {
	if(node == NULL)
		return;
	free(node -> company1);
	free(node -> company2);
	free(node);
	node = NULL;
}
node_t* find_node(linkedlist_t* list, long int timestamp) {
	if(list == NULL)
		return NULL;
	node_t* temp = list -> head;
	while(temp != NULL) {
		if(temp -> timestamp == timestamp) {
			//we found it yay
			return temp;
		}
		temp = temp -> next;
	}
	return NULL;
}
void free_linkedlist(linkedlist_t* list) {
	if(list == NULL)
		return;
	node_t* node = list -> head;
	while(node != NULL) {
		node_t* next = node -> next;
		//remove the node
		remove_node(list, node);		
		delete_node(node);
		node = NULL;
		node = next;
	}
	list -> head = NULL;
	list -> tail = NULL;
}
void printlist(linkedlist_t* list) {
	node_t* node = list -> head;
	while(node != NULL) {
		if(node -> prev != NULL) 
			printf("^\n");
		printf("%lf\n", node -> value);
		if(node -> next != NULL) 
			printf("v");
		node = node -> next;
	}
}
