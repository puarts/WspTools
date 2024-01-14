/**
 * @file fn-debug.cpp
 * 
 */

#include "fn-debug.h"


#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")
#endif


void WSP_PrintCallStack( void )
{
#ifdef _WIN32
     unsigned int   i;
     void         * stack[ 100 ];
     unsigned short frames;
     SYMBOL_INFO  * symbol;
     HANDLE         process;

     process = GetCurrentProcess();

     SymInitialize( process, NULL, TRUE );

     frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
     symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
     symbol->MaxNameLen   = 255;
     symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

     WSP_Printf ( "\nStack Trace:\n" );
     for( i = 1; i < frames; i++ )
     {
         SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );
         WSP_Printf( "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address );
     }

     free( symbol );
#endif
}
