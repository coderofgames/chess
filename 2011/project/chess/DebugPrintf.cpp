#include "DebugPrintf.h"

#define KILL_DEBUG_COMPLETELY
#ifdef KILL_DEBUG_COMPLETELY
void DebugPrintfInner( const char *fmt, ... )
{
}
#else
void DebugPrintfInner( const char *fmt, ... )
{
	static char buf[1024];
    #define PREPEND_TIME
    #ifdef  PREPEND_TIME
    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    const char *s = asctime(p);
    strcpy( buf, s );
    if( strchr(buf,'\n') )
        *strchr(buf,'\n') = '\0';
    strcat(buf,": ");
    #endif
	va_list stk;
	va_start( stk, fmt );
	vsprintf( strchr(buf,'\0'), fmt, stk );
//  #define DEBUG_TO_LOG_FILE
    #ifdef  DEBUG_TO_LOG_FILE
    {
        static FILE *log_file;
        if( log_file == NULL )
            log_file = fopen("log.txt","wt");
        if( log_file )
            fwrite( buf, 1, strlen(buf), log_file );
    }
    #else
    	OutputDebugString( buf );
    #endif
	va_end(stk);
}
#endif