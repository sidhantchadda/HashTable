#include"io.h"
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include "hashtable.c"
#include "linkedlist.c"
void write_accounts(FILE* fd, hashtable_t* accounts) {
	int i;
	for(i = 0; i< accounts -> table_len; i++) {
		hashtable_ent_t* ent = accounts -> table[i];
		while(ent != NULL) {
			fprintf(fd, "%s %lf\n", ent -> key, ent -> value);
			ent = ent -> next;
		}
	}
}
char* trimwhitespace(char* str) {
	char* end;
	//trim leading space
	while(isspace(*str)) str++;

	if(*str == 0) //if all spaces
		return str;
	//trim trailing spaces
	end = str + strlen(str) -1;
	while(end > str && isspace(*end)) end--;
	//make new null terminator
	*(end+1) = 0;
	return str;
}
//if at any point we return our function fails within the loop we need to be sure to return NULL and free the hashtbale
hashtable_t *read_accounts(FILE* fd) {
	if(fd == NULL)
		return NULL;
	hashtable_t* table = create_hashtable(1024);
	if(table == NULL)
		return NULL;
	char buff[255];
	while(fgets(buff, 255, (FILE*)fd)) {
		char* trim = (trimwhitespace(buff));
		char* tok1 = strtok(" ", trim);
		if(tok1 == NULL) {
			free_hashtable(table);	
			return NULL;
		}
		char* tok2 = strtok(NULL, " ");
		if(tok2 == NULL) {
			free_hashtable(table);	
			return NULL;
		}
		double amount = atof(tok2);
		int ret = set(table, tok1, amount);
		if(ret == -1) {
			free_hashtable(table);	
			return NULL;
		}
	}
	return table;
}
linkedlist_t* read_transactions(FILE* fd) {
	linkedlist_t* list = create_linkedlist();
	char buff[255];
	long int time =0 ;
	while(fgets(buff, 255, (FILE*)fd)) {
		char* trim = trimwhitespace(buff);
		//this array is located on the stack;
		char* arr[4];
		int i;
		char* tok1 = strtok(" ", trim);
		if(tok1 == NULL) {
			free_linkedlist(list);
			return NULL;
		}
		arr[0] = tok1;
		for(i =1; i<5; i++) {
			char*tok = strtok(NULL, " ");
			if(tok == NULL)
				arr[i] = NULL;
			arr[i] = tok;
		}
		//this must be NULL because there can only be 5 items in the paramters
		if(strtok(NULL, " ") != NULL) {
			free_linkedlist(list);
			return NULL;
		}
		long int timestamp = strtol(arr[0], NULL, 10);
		if(timestamp < time)
			return NULL;
		time = timestamp;
		short transaction = strtol(arr[1], NULL, 10);
		const char* company1 = arr[2];
		if(company1 == NULL) {
			free_linkedlist(list);
			return NULL;
		}
		if(transaction == TRANSFER) {
			// need all 5 parameters
			if(arr[4] == NULL) {
				free_linkedlist(list);
				return NULL;
			}
			const char* company2 = arr[3];
			double amount = atof(arr[4]);
			node_t* node = create_node(timestamp, transaction, company1, company2, amount);
			add_node(list, node);
		}
		else if(transaction == ADD) {
			//need 4 parameters
			if(arr[3] == NULL) {
				free_linkedlist(list);
				return NULL;
			}
			double amount = atof(arr[3]);
			node_t* node = create_node(timestamp, transaction, company1, NULL, amount);
			add_node(list, node);
		}
		else if(transaction == REMOVE) {
			//need 3 parameters
			node_t* node = find_node(list, timestamp);
			remove_node(list, node);
		}
		else {
			free_linkedlist(list);
			return NULL;
		}
	}
	return list;
}

int apply_transactions(linkedlist_t* transactions, hashtable_t* accounts) {
	node_t* node = transactions -> head;
	int count = 0;
	while(node != NULL) {
		count++;
		//in every transaction theres a company1
		const char* company1 = node -> company1;
		if(node -> transaction_type == TRANSFER) {
			const char* company2 = node -> company2;
			double amount = node -> value;
			//we dont wanna make company1 have negative money
			double a = 4;
			double b = 1;
			double* company1amount = &a;
			double* company2amount = &b;
			if(get(accounts, company1, company1amount) == -1)
				return -1;
			if(get(accounts, company2, company2amount) == -1)
				return -1;
			if(*company1amount - amount < 0)
				return -1;
			*company2amount = *company2amount +amount;
			*company1amount = *company1amount - amount;
			set(accounts, company2, *company2amount);
			set(accounts, company1, *company1amount);
		}
		if(node -> transaction_type == ADD) {
			double amount = node -> value;
			if(set(accounts, company1, amount) == -1)
				return -1;
		}
		if(node -> transaction_type == REMOVE) {
			if(remove_key(accounts, company1) == -1)
				return -1;
		}
		node = node -> next;
	}
	return count;
}
