/* ial.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
 */

 #ifndef IAL_H
 #define IAL_H

 #include "ifj-inter.h"

 struct _symbol_table
 {
     void (*get_stg)();
     void (*set_stg)();
 };

 symbol_table *ial_symbol_table_new();

 #endif
