#include<stdio.h>
#include<string.h>
#include"hashtable.h"

hashtable_t* create_hashtable(int max_size) {
	if(max_size <= 0)
		return NULL;
	hashtable_t* hashtable =(hashtable_t*) malloc(sizeof(hashtable_t));
	if(hashtable == NULL)
		return NULL;
	hashtable -> table_len = max_size;
	hashtable_ent_t** table = (hashtable_ent_t**) malloc(sizeof(hashtable_ent_t*)*max_size);
	if(table == NULL)
		return NULL;
	int i;
	for(i = 0; i<max_size; i++) {
		table[i] = NULL;
	}
	hashtable -> table  = table;
	return hashtable;
}
int key_exists(hashtable_t* table, const char* key) {
	if(table == NULL || key == NULL)
		return -1;
	int num = hash(key);
	num = num % table -> table_len;
	hashtable_ent_t* ent = table -> table[num];
	//iterate through the linked list to find the key
	while(ent != NULL) {
		//if found
		if(strcmp(ent -> key, key) == 0) {
			return 1;
		}
		ent = ent -> next;
	}
	return 0;
}
int get(hashtable_t* table, const char * key, double *value) {
	if(table == NULL || key == NULL || value == NULL)
		return -1;
	int num = hash(key);
	num = num % table -> table_len;
	hashtable_ent_t* ent = table -> table[num];
	while(ent != NULL) {
		if(strcmp(ent -> key, key) == 0) {
			//key found
			//replace the key's value with the new value
			*value = ent -> value;
			return 0;
		}
		ent = ent -> next;
	}
	return -1;
}
//if the key already exists update the node with the existing value
int set(hashtable_t* table, const char* key, double value) {
	if(table == NULL || key == NULL)
		return -1;
	int num = hash(key);
	num = num % table -> table_len;
	hashtable_ent_t* ent = table -> table[num];
	if(ent == NULL) {
		hashtable_ent_t* new = (hashtable_ent_t*) malloc(sizeof(hashtable_ent_t));
		new -> next = NULL;
		new -> key = strdup(key);
		new -> value = value;
		table -> table[num] = new;
		return 0;
	}
	while(ent != NULL) {
		if(strcmp(ent -> key, key) == 0) {
			//key has already been found update key with existing value
			ent -> value = value;
			return 0;
		}
		if(ent -> next == NULL)
			break;
		ent = ent -> next;
	}
	hashtable_ent_t* new = (hashtable_ent_t*) malloc(sizeof(hashtable_ent_t));
	if(new == NULL)
		return -1;
	ent -> next = new;
	new -> next = NULL;
	new -> value = value;
	new -> key = strdup(key);
	return 0;
}
int remove_key(hashtable_t* table, const char* key) {
	//check for head tail and middle cases
	if(table == NULL || key == NULL)
		return -1;
	int num = hash(key);
	num = num % table -> table_len;
	hashtable_ent_t* ent = table -> table[num];
	hashtable_ent_t* prev = NULL;
	int firstit = 1;
	while(ent != NULL) {
		//if we need to remove the node
		if(strcmp(key, ent -> key) == 0) {
			//if its the head node
			if(ent == table -> table[num]) {
				//if there is only one element in the list
				hashtable_ent_t* next = ent -> next;
				if(next == NULL) {
					free(ent -> key);
					free(ent);
					ent = NULL;
					table -> table[num] = NULL;
					return 0;
				}
				//case where its head and more than one element in list
				else {
					free(ent -> key);
					free(ent);
					ent = NULL;
					table -> table[num] = next;
					return 0;
				}
			}
			//if its the tail node and theres more tahn one element in the list
			if(ent -> next == NULL) {
				free(ent -> key);
				free(ent);
				ent = NULL;;
				prev -> next = NULL;
				return 0;
			}
			//if its a middle node must be three elements in the list at this point
			hashtable_ent_t* next = ent -> next;
			free(ent -> key);
			free(ent);
			ent = NULL;
			prev -> next = next;
			return 0;
		}
		if(firstit ==1) {
			prev = ent;
			firstit = 0;
		}
		else
			prev = prev -> next;
		ent = ent -> next;
	}
	return -1;
}
//free 4 things
void free_hashtable(hashtable_t* table) {
	//lets first free all the nodes and their keysa
	int i;
	for(i = 0; i<table ->table_len; i++) {
		hashtable_ent_t* ent = table -> table[i];	
		while(ent != NULL) {
			hashtable_ent_t* next = ent -> next;
			remove_key(table, ent -> key);
			ent = next;
		}
	}
	free(table -> table);
	free(table);
	table = NULL;
}
