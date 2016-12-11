/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
This file is the Header for the String Conversion Tools Module. It contains
a function for converting simple C strings ( arrays of type char ) to 
win32 API strings of type LPTSTR.

This File was written as part of HW assignment 2 in the course "Introduction
To Systems Programming" at Tel-Aviv University's School of Electrical 
Engineering, Winter 2011, by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef ISP_STRING_CONVERSION_TOOLS_H
#define ISP_STRING_CONVERSION_TOOLS_H

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include <windows.h>

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

/**
 * ConvertCharStringToLPTSTR converts a simple C string ( An array of type 
 * char terminated with a zero ) to a Win32 API string of type LPTSTR.
 * The LPTSTR is dynamically allocated and must be freed by the caller. 
 *
 * Inputs:
 * ------
 * Source - a C-string. 
 *
 * Outputs:
 * -------
 * An LPTSTR string containing the same text as Source. Memory is allocated
 * for this string, so it must be freed by the user when it is no longer needed.
 *
 * Example Usage:
 * -------------
 *      char* CommandAsCString = "calc.exe";
 *      LPTSTR Command = ConvertCharStringToLPTSTR( CommandAsCString );
 *
 *      if ( Command != NULL )
 *      {
 *           DoSomethingWith( Command );
 *           free( Command );
 *      }
 *      else
 *      {
 *           printf("Failed to convert C-String to LPTSTR\n");
 *      }
 */ 
LPTSTR ConvertCharStringToLPTSTR( const char *Source );

/**
 * CreateSentenceFromWords accepts an array of strings (words), and creates
 * a new string containing a sentence built from these words.
 *
 * Accepts:
 * -------
 * WordsArr - an array of char pointers, each pointing to a string 
 *            ( string = an array of chars terminated by zero ).
 *            Each string is a word.
 * ArrSize - the number of words in WordsArr.
 *
 * Returns:
 * -------
 * On Failure: NULL.
 * On Success: A dynamically allocated string containing a sentence built by joining the
 * words in WordsArr, and placing a spearator (space) between each two words.
 * Notice: the caller must later free this memory.
 */
char* CreateSentenceFromWords( char const * const * WordsArr, int ArrSize );

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#endif /* ISP_STRING_CONVERSION_TOOLS_H */