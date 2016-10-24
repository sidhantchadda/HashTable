#include"io.c"

int main(int argc, char**argv) {
	if(argc != 4) {
		printf("Usage: <transaction_file><account_int_file><account_out_file>\n");
		return 1;
	}
	FILE* f = fopen(argv[1], "r");
	linkedlist_t* list = read_transactions(f);
	if(list == NULL)
		return 1;
	fclose(f);

	FILE* f1 = fopen(argv[2], "r");
	hashtable_t* table = read_accounts(f1);
	if(table == NULL)
		return 1;
	fclose(f1);

	int check = apply_transactions(list, table);
	if(check == -1)
		return 1;
	FILE* f2 = fopen(argv[3], "w");
	write_accounts(f2, table);
	fclose(f2);
	free_linkedlist(list);
	free_hashtable(table);
	list = NULL;
	table = NULL;
	return 0;
}
