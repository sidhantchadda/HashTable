#ifndef IO_H
#define IO_H

#include <stdio.h>

#include "hashtable.h"
#include "linkedlist.h"

/**
 * Constants for different transaction types
 */
#define ADD 1
#define REMOVE 2
#define TRANSFER 4

/**
 * Various IO functions to:
 *  Read the intial account states
 *  Read the transaction list
 *  Apply the transaction list to the account hashtable
 *  Write the final account states back to the file
 */

/**
 * Read the initial account information from a file
 * by creating a new hashtable with one entry per line
 * in the file, containing the <CompanyName> as the key
 * and the <AccountBalance> as the value. If a given
 * <CompanyName> already exists in the hashtable,
 * overwrite the existing balance associated with
 * the <CompanyName>
 * Input Format:
 *  <CompanyName> <AccountBalance>
 *  <CompanyName> <AccountBalance>
 *  <CompanyName> <AccountBalance>
 *  ......
 *  <CompanyName> <AccountBalance>
 *  there may be leading or trailing spaces
 * Parameters:
 *      fd - File descriptor to read from
 * Return:
 *      Pointer to newly created hashtable, the caller is
 *      responsible for freeing this later. Return NULL
 *      if fd is NULL or if the account file is incorrectly
 *      formatted.
 */
hashtable_t *read_accounts(FILE *fd);

/**
 * Write the final account information to a file by iterating
 * through the hashtable and outputting valid entries to the file
 * Output Format:
 *      <CompanyName> <AccountBalance>
 *      <CompanyName> <AccountBalance>
 *      ....
 *      ....
 *      <CompanyName> <AccountBalance>
 * Sample Output Records:
 *      "Google 10000"
 *      "Microsoft 43082.34"
 * Parameters:
 *      fd - File descriptor to write the output to
 *      accounts - Pointer to hashtable to write to the file
 * Remarks:
 *      The ordering in which the <CompanyName> <AccountBalance> lines
 *      are written to file does not matter
 */
void write_accounts(FILE *fd, hashtable_t *accounts);

/**
 * Read a list of transactions from a file
 * Input Format:
 *      <TimeStamp> <TransactionType> <Company1> [<Company2> <Value>]
 * Sample Input Lines:
 *      "159742987 TRANSFER Google Microsoft 10000000"
 *      "7398079827 ADD Google 1000000"
 *      "907983988 REMOVE Microsoft"
 * Parameters:
 *      fd - File descriptor for the file to read input from
 * Returns:
 *      A pointer to a linkedlist_t structure containing the transactions in the order
 *      they were in the file or NULL if fd is invalid or the file contains a transaction
 *      that was invalid (Unknown transaction type, timestamps out of order, etc)
 */
linkedlist_t *read_transactions(FILE *fd);

/**
 * Iterate from the head to the tail of the transaction linked list
 * and apply the transactions in order to the accounts hashtable.
 * For each node in the list, perform the transaction the list
 * node describes, then move onto the next node.
 * Transaction:
 *      Each node in the linked list contains five fields describing the transaction:
 *      1. 'timestamp' - The time at which the transaction occured
 *      2. 'transaction_type' - Integer representing the transaction type (ADD, REMOVE, TRANSFER)
 *      3. 'company1' - A pointer to the string containing the first company name
 *      4. 'company2' - A pointer to the string containing the second company name (Only for TRANSFERs)
 *      5. 'value'    - The numeric value of the TRANSFER or inital account balance for an ADD
 * Transaction Type:
 *      ADD 'company1' 'value'
 *          Add 'company1' to the account hashtable with a value of 'value'
 *      REMOVE 'company1'
 *          Remove 'company1' from the hashtable if it exists, error if it doesn't
 *      TRANSFER 'company1' 'company2' 'value'
 *          Transfer 'value' from the account of 'company1' into the account of 'company2'
 * Parameters:
 *      transactions - Ordered transactions in a linked list
 *      accounts - Hashtable mapping company name to account balance
 * Return:
 *      -1 if any of the parameters are NULL or an illegal transaction
 *      occurs, otherwise return the number of processed transactions
 */
int apply_transactions(linkedlist_t *transactions, hashtable_t *accounts);

#endif
