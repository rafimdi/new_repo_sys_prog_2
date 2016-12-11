/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
This file contains the implementation of the String Conversion Tools Module. 
See StringConversionTools.h for details.

This File was written as part of HW assignment 2 in the course "Introduction
To Systems Programming" at Tel-Aviv University's School of Electrical 
Engineering, Winter 2011, by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#define _CRT_SECURE_NO_WARNINGS /* to suppress Visual Studio 2010 compiler warning */

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "StringConversionTools.h"
#include <tchar.h>

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#define STRINGS_ARE_IDENTICAL( str1, str2 ) ( strcmp( (str1), (str2) ) == 0 )
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

LPTSTR ConvertCharStringToLPTSTR( const char *Source )
{
    /* the win32 API LPTSTR string type is defined in one of two ways, */
    /* as a simple char string or as a wide-character (unicode) string.*/ 
    /* If the second case is true, the macro UNICODE should be defined. */

    #ifdef UNICODE     
        #define STR_COPY_FUNCTION mbstowcs /* converts a simple char string */
                                           /* to a wide char string */
        typedef size_t CopyFunctionOutput_t;
    #else
        #define STR_COPY_FUNCTION strncpy 
        typedef char *CopyFunctionOutput_t;
    #endif
    
    TCHAR *Dest = NULL;
    CopyFunctionOutput_t CopyFunctionOutput;
    BOOL CopyFunctionSucceeded;
    size_t NumOfLettersInSource;
    size_t LengthOfSourceIncludingTerminatingZero;

    if ( Source == NULL )
        return NULL;

    NumOfLettersInSource = strlen(Source);
    LengthOfSourceIncludingTerminatingZero =  NumOfLettersInSource + 1;
    
    Dest = (TCHAR*)malloc( sizeof(TCHAR) * LengthOfSourceIncludingTerminatingZero );
    
    CopyFunctionOutput = STR_COPY_FUNCTION( 
        Dest, 
        Source, 
        LengthOfSourceIncludingTerminatingZero );  

    /* Add terminating zero: */
    Dest[ LengthOfSourceIncludingTerminatingZero - 1 ] = _T( '\0' );

    #ifdef UNICODE     
        CopyFunctionSucceeded = ( CopyFunctionOutput == NumOfLettersInSource );
    #else        
        CopyFunctionSucceeded = STRINGS_ARE_IDENTICAL( Dest, Source );
    #endif

    if ( !CopyFunctionSucceeded )
    {
        free( Dest );
        return NULL;
    }
 
    return (LPTSTR)Dest;

    #undef STR_COPY_FUNCTION 
}

char* CreateSentenceFromWords( char const * const * WordsArr, int ArrSize )
{
    const char* WordSeparator = " "; /* Used to separate between words in */ 
                                                          /* the sentence */
    const int WordSeparatorLength = strlen( WordSeparator );

    int NumOfLettersInSentence;
    int TotalSentenceLength;
    int WordInd;
    int LetterInd;
    int IndexInSentence;
    char *Sentence = NULL;

    if ( WordsArr == NULL )
        return NULL;
   
    /* ------------------------------------------------- */
    /* 1. Count the total number of letters in sentence: */
    /* ------------------------------------------------- */
    NumOfLettersInSentence = 0;
    for ( WordInd = 0; WordInd < ArrSize; WordInd++ )
    {
        if ( WordInd > 0 )
        {
            NumOfLettersInSentence += WordSeparatorLength;
        }

        NumOfLettersInSentence += strlen( WordsArr[WordInd] );
    }
    TotalSentenceLength = NumOfLettersInSentence + 1; /* leave room for '\0' */

    /* ------------------------------------------------- */
    /*     2. Allocate memory to hold the sentence:      */
    /* ------------------------------------------------- */
    Sentence = (char*)malloc( sizeof(char) * TotalSentenceLength );

    if ( Sentence == NULL )
        return NULL;

    /* ------------------------------------------------- */
    /*          3. Copy words into sentence:             */
    /* ------------------------------------------------- */
    IndexInSentence = 0;
    for ( WordInd = 0; WordInd < ArrSize; WordInd++ )
    {        
        const char* CurWord = WordsArr[WordInd]; 
        const int LengthOfCurWord = strlen(CurWord);
        /* <ISP> the C language allows */ 
        /* us to declare variables at the beginning of a block, which is a piece */
        /* of the program that start with a { and ends with a }. This allows us */
        /* to declare variables closer to where they are used, which can prevent */
        /* mistakes. */

        if ( WordInd > 0 )
        {
            for ( LetterInd = 0 ; LetterInd < WordSeparatorLength ; LetterInd++ )
            {
                Sentence[IndexInSentence] = WordSeparator[LetterInd];
                IndexInSentence++;
            }
        }
        
        for ( LetterInd = 0 ; LetterInd < LengthOfCurWord; LetterInd++ )
        {
            Sentence[IndexInSentence] = CurWord[LetterInd];
            IndexInSentence++;
        }
    }
    Sentence[IndexInSentence] = '\0';

    return Sentence;
}

