
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "getmsg.h"

//#define TESTING

#define TRUE  1
#define FALSE 0

char *GetErrorMessage ( unsigned int ErcReqd, char *szMessage81, char *szFilename )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ GetErrorMessage                         Jonnie, Fri 03-Jan-1997, 13:46:29
//³ ---------------
//³
//³ If szFilename is NULL, the last filename specified is used. 
//³
//³ If an error occurs one of the following are returned:
//³    "*** Unable to retrieve message. Insufficient memory ***"
//³    "*** Unable to retrieve message. Filename not specified ***"
//³    "*** Unable to retrieve message. Failed to open file ***"
//³
//³ If an error occurs one of the following are returned:
//³
//³ The message file must have the following format:
//³
//³ <Leader><Code><Delimiter><Message><Terminator>
//³ Where: <Leader>    = Zero or more non-numeric characters (ignored).
//³        <Code>      = Numeric message code (value 1..65535).
//³        <Delimiter> = Any non-numeric character (ignored).
//³        <Message>   = Any message string (first 80 chars are significant).
//³        <Terminator>= CR or LF or both
//³
//³ eg. If the file c:\doddle\ERRORS.MSG contained the line 
//³           "   00027 Sector not found\r\n" 
//³
//³     char TempBuffer81[81];
//³     GetErrorMessage(27,TempBuffer81,"c:\\doddle\\ERRORS.MSG");
//³     would return with "Sector not found\0" in TempBuffer81.
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   static char  ErrorFileName[_MAX_PATH] = {0};
   unsigned int ErcFound                 = 0;
   int          DecodeMsg                = FALSE;
   FILE        *pErrorFile               = NULL;
   char        *pBuffer                  = NULL;
   int          j;
    
   strcpy ( szMessage81, "*** Unable to retrieve message. " );
   pBuffer = malloc ( 400+1 ); // extra 1 incase we need to add a '\n' to the end of the last block
   if ( !pBuffer )
   {
      strcat ( szMessage81, "Insufficient memory ***" );
      return szMessage81;
   }

   if (szFilename) strcpy ( ErrorFileName, szFilename );
   if (ErrorFileName[0] == 0) 
   {
      free (pBuffer);
      strcat ( szMessage81, "Filename not specified ***" );
      return szMessage81;
   }

   pErrorFile = fopen ( ErrorFileName, "rb" );
   if ( !pErrorFile )
   {
      free (pBuffer);
      strcat ( szMessage81, "Failed to open file ***" );
      return szMessage81;
   }

   szMessage81[0] = 0;
   while ( !feof(pErrorFile) )
   {
      int cnt = fread ( pBuffer, 1, 400, pErrorFile );
      if (feof(pErrorFile) && pBuffer[cnt-1] != '\r' && pBuffer[cnt-1] != '\n')
      {
         pBuffer[cnt] = '\n';
         cnt++;
      }

      for ( j=0; j<cnt; j++ )
      {
         if ( DecodeMsg )
         {
            if ( pBuffer[j] == '\r' || pBuffer[j] == '\n' )
            {
               if ( ErcReqd == ErcFound )
               {
                  fclose ( pErrorFile );
                  free (pBuffer);
                  return szMessage81;
               }
               DecodeMsg      = FALSE;
               szMessage81[0] = 0;
               ErcFound       = 0;
               continue;
            }
            if ( strlen(szMessage81) < 80 )
            {
               int len = strlen(szMessage81);
               szMessage81[len] = pBuffer[j];
               szMessage81[len+1] = 0;
            }
            // If this is the last byte in the file
            //if (feof(pErrorFile) && j==cnt-1)
            //   break;
            continue;
         }

         if ( pBuffer[j] < '0' || pBuffer[j] > '9' )
         {
            if ( ErcFound > 0 ) DecodeMsg = TRUE;
            continue;
         }
         ErcFound = ErcFound*10 + pBuffer[j] - '0';
      }
   }
   fclose ( pErrorFile );
   free (pBuffer);
   return szMessage81;
}

#ifdef TESTING
int main ( int argc, char *argv[] )
{
   char TempBuffer81[81];
   printf("\"%s\"\n",GetErrorMessage(1,TempBuffer81,"c:\\doddle\\ERRORS.MSG") );
   printf("\"%s\"\n",GetErrorMessage(400,TempBuffer81,"c:\\doddle\\ERRORS.MSG") );
   printf("\"%s\"\n",GetErrorMessage(40098,TempBuffer81,NULL) );
   printf("\"%s\"\n",GetErrorMessage(30098,TempBuffer81,NULL) );
   printf("\"%s\"\n",GetErrorMessage(2,TempBuffer81,"ERRORS.MSG") );
   return 0;
}
#endif
