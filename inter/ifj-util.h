/* ifj-util.h
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xcubae00@stud.fit.vutbr.cz>
 *          Jan Demcak < DOPISAT >
 *          Ondrej Kurak < DOPISAT >
 */

 #ifndef IFJ_UTIL_H
 #define IFJ_UTIL_H

int ifj_read_int ();
double ifj_read_double ();
const char * ifj_read_string ();

void ifj_print ( const char *input );

int ifj_length ( const char *inputString );

const char * ifj_substr (	const char *inputString,
							int i,
							int n );

int ifj_compare (	const char *s1,
					const char *s2 );

#endif