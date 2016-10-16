/* ifj-util.c
 *
 * Copyright (C) 2016 SsYoloSwag41 Inc.
 * Authors: Robert Kolcun <xcubae00@stud.fit.vutbr.cz>
 *          Jan Demcak < DOPISAT >
 *          Ondrej Kurak < DOPISAT >
 */

 #include "ifj-util.h"
 #include <stdlib.h>
 #include <string.h>

/**
 * Read integer from stdin
 * @returns integer number
*/
int ifj_read_int ()
{
	return 0;
}

/**
 * Read double from stdin
 * @returns floating point number
*/
double ifj_read_double ()
{
	return 0;
}

/**
 * Read string from stdin
 * @returns string
*/
const char * ifj_read_string ()
{
	return NULL;
}

/**
 * Print input at stdout
 * - int convert to string
 * - double convert to string and print by '%g'
 * @param input string
 * @returns void
*/
void ifj_print ( const char *input )
{

}

/**
 * Calculate length of inputString
 * @param inputString
 * @return number length of inputString
*/
int ifj_length ( const char *inputString )
{
	return strlen(inputString);
}

/**
 * Return substring of the inputString
 * - simulate substring function from Java
 * @param string inputString
 * @param index beginning of substring
 * @param length substring length
 * @returns
*/
char * ifj_substr (	const char *inputString,
					int i,
					int n )
{
	char *outputString = malloc(sizeof(*outputString) * (strlen(inputString) + 1));

	if (outputString == NULL)
	{
		return NULL;
	}

	for (int j = i; j < i + n; ++j)
	{
		if (inputString[j] == '\0')
		{
			break;
		}

		outputString[j - i] = inputString[j];
		outputString[j - i + 1] = '\0';
	}

	return outputString;
}

/**
 * Compare two input strings
 * - simulate function compareTo from Java
 * @param first input string
 * @param second input string
 * @returns 0 - s1 and s2 are same, 1 - s1 is greater than s2, -1 - another situation
*/
int ifj_compare (	const char *s1,
					const char *s2 )
{
	for (int i = 0; ; ++i)
	{
		if (s1[i] == s2[i])
		{
			if (s1[i] == '\0' || s2[i] == '\0')
			{
				break;
			}

			continue;
		}
		else
		{
			if (s1[i] == '\0' || s2[i] == '\0')
			{
				return ifj_length(s1) - ifj_length(s2) > 0 ? 1 : -1;
			}

			return s1[i] - s2[i] > 0 ? 1 : -1;
		}
	}

	return 0;
}
