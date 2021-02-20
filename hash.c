/*
me:
 * GNumber:
 *  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

/* (IMPLEMENT THIS FUNCTION)
 *  * In this function, you will create a new Symtab struct.
 *   * Follow the Directions in the Project Documentation for this Function
 *    * Return the pointer to the new symtab.
 *     * On any memory errors, return NULL
 *      */
Symtab *hash_initialize() {
  /* Implement this function */
  Symtab *new=malloc(sizeof(Symtab));
  if(new == NULL)
  {
    return NULL;
  }
  new->table=malloc(sizeof(Symbol *) * HASH_TABLE_INITIAL);
  
  if(new->table == NULL)
  {
    return NULL;
  }

  int i=0;
  for(i=0;i<HASH_TABLE_INITIAL;i++)
  {
    new->table[i] = NULL;
  }
  new->capacity=HASH_TABLE_INITIAL;
  new->size=0;
  /*
 *   new->table[HASH_TABLE_INITIAL] = {0};
 *     */
  return new;
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Destroy your Symbol Table.
 *   * Follow the Directions in the Project Documentation for this Function
 *    * Return on any memory errors.
 *     */
void hash_destroy(Symtab *symtab) {
  /* Implement this function */
  Symbol *new_sym=NULL;
  Symbol *temp=NULL;
  int i=0;
  for(i=0;i<symtab->capacity; i++)
  {
    new_sym=symtab->table[i];
    temp=new_sym;
    while(temp!=NULL)
    {
      new_sym=temp;
      temp=temp->next;
      symbol_free(new_sym);
    }
  }
  free(symtab->table);
  free(symtab);
  return ;
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Return the capacity of the table inside of symtab.
 *   * If symtab is NULL, return -1;
 *    */
int hash_get_capacity(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL)
  {
    return -1;
  }
  return symtab->capacity;
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Return the number of used indexes in the table (size) inside of symtab.
 *   * If symtab is NULL, return -1;
 *    */
int hash_get_size(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL)
  {
    return -1;
  }
  return symtab->size;
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Adds a new Symbol to the symtab via Hashing.
 *   * Follow the Directions in the Project Documentation for this Function
 *    * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 *     * Otherwise, return 0;
 *      */
int hash_put(Symtab *symtab, char *var, int val) {
  /* Implement this function */
  if (symtab == NULL)
  {
    return -1;
  }
  int n=hash_code(var)%(symtab->capacity);
  Symbol *sym_temp=symtab->table[n];
  Symbol *prev=NULL;
  while(sym_temp != NULL)
  {
    if(strcmp(sym_temp->variable,var) == 0)
    {
      sym_temp->val=val;
      return 0;
      break;
    }
    prev=sym_temp;
    sym_temp=sym_temp->next;
  }
  if(sym_temp == NULL)
  {
	
      Symbol *create=symbol_create(var, val);
      if( create == NULL )
      {
		return -1;
      }
      create->next=NULL;

      if(prev == NULL || sym_tab->table[n] == NULL)
      {
        symtab->table[n]=create;
        symtab->size=(symtab->size)+1;;
        
      }
      else
      {
        prev->next=create;
        symtab->size=(symtab->size)+1;
        
      }
      double load= (double) (symtab->size)/(symtab->capacity);
      if(load >= 2.0)
      {
         hash_rehash(symtab, (symtab->capacity)*2 );
      }
      return 0;
    
  }
  else
  {
	return -1;
  }
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Gets the Symbol for a variable in the Hash Table.
 *   * Follow the Directions in the Project Documentation for this Function
 *    * On any NULL symtab or memory errors, return NULL
 *     */
Symbol *hash_get(Symtab *symtab, char *var) {
  /* Implement this function */
  if (symtab == NULL)
  {
    return NULL;
  }
  int n=hash_code(var)%(symtab->capacity);
  Symbol *sym_temp=symtab->table[n];
  while(sym_temp != NULL)
  {
    if(strcmp(sym_temp->variable,var) == 0)
    {
      break;
    }
    sym_temp=sym_temp->next;
  }
  if(sym_temp == NULL)
  {
    return NULL;
  }
  else
  {
    Symbol *sym_ret=symbol_copy(sym_temp);
    symbol_free(sym_temp);
    return sym_ret;
  }
}

/* (IMPLEMENT THIS FUNCTION)
 *  * Doubles the size of the Array in symtab and rehashes.
 *   * Follow the Directions in the Project Documentation for this Function
 *    * If there were any memory errors, set symtab->array to NULL
 *     * If symtab is NULL, return immediately.
 *      */
void hash_rehash(Symtab *symtab, int new_capacity) {
  /* Implement this function */ 
  if(symtab == NULL)
  {
    return;
  }
  Symbol **new=malloc(sizeof(Symbol *) * new_capacity);
  Symbol **old=symtab->table;
  int size_old=symtab->size;
  
  if(new== NULL) 
  {
    symtab->table=NULL;
    return;
  }

  int i=0;
  for(i=0;i<new_capacity;i++)
  {
    new[i] = NULL;
  }
  symtab->capacity=new_capacity;
  symtab->size=0;
  symtab->table=new;
  Symbol *sym_temp=NULL;
  Symbol *prev=NULL;

  for (int i = 0; i < size_old; i++)
  {
    sym_temp=old[i];
    prev=NULL;
    while(sym_temp != NULL)
    {
      hash_put(symtab, sym_temp->variable, sym_temp->val);
      //prev=sym_temp;
      sym_temp=sym_temp->next;
      //symbol_free(prev);
    }
  }
  free(old);
  return;
}

/* Implemented for you.
 *  * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 *  * This computes the hash function for a String
 *   */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }

  return code;
}

