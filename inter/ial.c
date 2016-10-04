/* ial.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

 #include "ial.h"
 #include <stdlib.h>
 #include <stdio.h>

 /**
  * Symbol table constructor
  * @return new uninitialised symbol_table
  */
 static symbol_table* _ial_symbol_table_construct()
 {
     symbol_table *table = (symbol_table*) malloc ( sizeof( struct _symbol_table ));
     if(!table)
     {
         fprintf(stderr,"ERROR: allocating symbol table!\n");
         exit(99);
     }
     return table;
 }

/**
 * Symbol table initializer
 * @param self symbol_table
 */
 static void ial_symbol_table_init( symbol_table *self )
 {
     //TODO intialize object here, prepare functions and variables
 }

/**
 * Create new symbol table structure
 *  - contains default methods + hash structure
 * @return  new symbol table
 */
 symbol_table *ial_symbol_table_new()
 {
     symbol_table *table = _ial_symbol_table_construct();
     ial_symbol_table_init(table);
     return table;
 }
