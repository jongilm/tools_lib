// Jonnie, Sun 02-Mar-1997, 22:42:49

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <conio.h>
#include <stdarg.h>

#include "tools.h"

static char LastIniErrorMessage[121] = {""};

char *tIniFile::SkipWhiteSpaces ( char *p )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ remove leading white spaces
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   //while ((*p == ' ') || (*p == '\t') || (*p == '\0')) p++;
   while (p[0]==' ' || p[0]=='\t') p++;
   return p;
}

int tIniFile::IsComment ( char *p )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   p = SkipWhiteSpaces ( p );

   if ( strlen(p)==0 ||
        p[0] == ';' ||
        p[0] == '#' ||
        p[0] == '!' ||
        (p[0] == '/' && p[1] == '/') )
      return TRUE;
   else
      return FALSE;
}

void tIniFile::fputs_lf ( char *string, FILE *stream )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (string)
   {
      if (strlen(string) > 0)
         fputs ( string, stream );
      fputs ( "\n", stream );
   }
}

void tIniFile::fputs_section ( char *Section, FILE *stream  )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   char Str[256];
   if ( Section && strlen(Section)>0 )
   {
      strcat(strcat(strcpy(Str,"["),Section),"]");
      //fputs ( "\n", stream ); // Jonnie, Sun 02-Mar-1997, 20:54:59
      fputs_lf ( Str, stream );
   }
}

void tIniFile::fputs_label ( char *Label, char *pData, FILE *stream  )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   char Str[256];

   if (pData && Label) // if no pData then we're deleting the label.
   {
      strcpy(Str,Label);
      if ( strlen(pData)>0 )
         strcat(strcat(Str,"="),pData);

      fputs_lf ( Str, stream );
   }
}

int tIniFile::IsSection ( char *p, char *Section )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (!Section)
   {
      if ( IsComment ( p ) )
         return FALSE;
      p = SkipWhiteSpaces ( p );
      if (p[0] == '[')
         return TRUE; // any section

      return FALSE;
   }

   if (strlen(Section)==0)
      return FALSE;

   if ( IsComment ( p ) )
      return FALSE;

   p = SkipWhiteSpaces ( p );
   if (p[0] != '[')
      return FALSE;

   p++;
   p = SkipWhiteSpaces ( p );
   Section = SkipWhiteSpaces ( Section );
   if ( memicmp ( p, Section, strlen(Section) ) != 0)
      return FALSE;

   p += strlen(Section);
   p = SkipWhiteSpaces ( p );
   if (p[0] != ']')
      return FALSE;

   return TRUE;
}

char *tIniFile::IsLabel ( char *p, char *Label )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ return NULL if not correct label
//³ return pointer to value if correct label=value
//³ return pointer to 0     if correct label
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (!Label || strlen(Label)==0)
      return NULL;


   if ( IsComment ( p ) )
      return NULL;

   p = SkipWhiteSpaces ( p );
   if (p[0] == '[')
      return NULL;

   Label = SkipWhiteSpaces ( Label );
   if ( memicmp ( p, Label, strlen(Label) ) != 0)
      return NULL;

   p += strlen(Label);
   p = SkipWhiteSpaces ( p );

   if ( strlen(p)==0 )
      return p; // pointer to 0

   if (p[0] != '=' )
      return NULL;

   p++;
   p = SkipWhiteSpaces ( p );
   return p; // pointer to value
}

//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ PUBLIC METHODS
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

tIniFile::tIniFile ( char *pFilename )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   strcpy ( pIniFilename, pFilename );
   fIniFile = NULL;
   OpenForReading = 0;
}

tIniFile::~tIniFile ()
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   Close ();
}

int tIniFile::HasReadAccess ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (_access ( pIniFilename, 0x04 ) == 0)
      return TRUE;
   return FALSE;
}

int tIniFile::HasWriteAccess ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (_access ( pIniFilename, 0x02 ) == 0)
      return TRUE;
   return FALSE;
}

int tIniFile::Exists ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (_access ( pIniFilename, 0x00 ) == 0)
      return TRUE;
   return FALSE;
}

void tIniFile::Close ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (fIniFile)
   {
      fclose ( fIniFile );
      fIniFile = NULL;
      OpenForReading = 0;
   }
}

int tIniFile::Read ( char *Section, char *Label, char *pDataRet, int *pcbDataRet )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   char  Str[256];
   char *p;
   int   SectionFound = 0;
   //LONG  FilePos = 0L;
   int rc;

   LastIniErrorMessage[0] = 0;

   if (!Exists())
   {
      sprintf ( LastIniErrorMessage,"File not found: \"%s\"", pIniFilename );
      rc = ERC_FILENOTFOUND;
#ifdef DEBUG
      printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }
   if (!HasReadAccess())
   {
      sprintf ( LastIniErrorMessage,"Access denied: \"%s\"", pIniFilename );
      rc = ERC_ACCESSDENIED;
#ifdef DEBUG
      printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }

   if (!OpenForReading)
   {
      fIniFile = fopen ( pIniFilename,"r+t");
      if ( fIniFile == NULL )
      {
         sprintf ( LastIniErrorMessage,"Open failed: \"%s\"", pIniFilename );
         rc = ERC_OPENFAILED;
#ifdef DEBUG
         printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
         return rc;
      }
      OpenForReading = 1;
   }

   if ( Section == NULL      ) SectionFound = 1;
   if ( strlen(Section) == 0 ) SectionFound = 1;

   fseek(fIniFile,0L,SEEK_SET);
   for (;;)
   {
      if (fgets(Str,255,fIniFile) == 0)
      {
         if (!SectionFound)
         {
            sprintf ( LastIniErrorMessage,"Section not found \"%s\" [%s]", pIniFilename, Section );
            rc = ERC_SECTIONNOTFOUND;
#ifdef DEBUG
            printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
            return rc;
         }
         else
         {
            sprintf ( LastIniErrorMessage,"Label not found \"%s\" [%s] \"%s\"", pIniFilename, Section, Label );
            rc = ERC_LABELNOTFOUND;
#ifdef DEBUG
            printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
            return rc;
         }
      }
      if (strlen(Str) == 0) continue;

      if (Str[strlen(Str)-1] == '\n') Str[strlen(Str)-1] = 0;
      p = Str;
      if ( IsComment ( p ) ) continue;

      if (!SectionFound)
      {
         if ( IsSection ( p, Section ) ) // specific section
         {
            SectionFound = 1;
            if (Label == NULL) break;
         }
      }
      else
      {
         if ( IsSection ( p, NULL ) ) // any section
            break;

         p = IsLabel ( p, Label );
         if ( p )
         {
            if (pcbDataRet) *pcbDataRet = (int)strlen(p);
            if (pDataRet) strcpy ( pDataRet, p );
            if (strlen(p) == 0)
            {
               sprintf ( LastIniErrorMessage,"Label found with no value \"%s\" [%s] \"%s\"", pIniFilename, Section, Label );
               rc = ERC_INILABELONLY;
#ifdef DEBUG
               printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
               return rc;
            }
            return 0;
         }
      }
   }

   if (!SectionFound)
   {
      sprintf ( LastIniErrorMessage,"Section not found \"%s\" [%s]", pIniFilename, Section );
      rc = ERC_SECTIONNOTFOUND;
#ifdef DEBUG
      printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }
   else
   {
      sprintf ( LastIniErrorMessage,"Label not found \"%s\" [%s] \"%s\"", pIniFilename, Section, Label );
      rc = ERC_LABELNOTFOUND;
#ifdef DEBUG
      printf("IniFileRead Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }
}

long tIniFile::ReadLong ( char *Section, char *Label, long DefaultValue )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   char pData[21];
   int cbData;
   long value;
   int rc;

   rc = Read ( Section, Label, pData, &cbData );
   if (rc == 0)
   {
      if (pData[0]=='0' && toupper(pData[1])=='X')
      {
         if (sscanf(pData,"%lx",&value) == 1)
            return value;
      }
      else
      {
         if (sscanf(pData,"%ld",&value) == 1)
            return value;
      }
   }
   return DefaultValue;
}

int tIniFile::Writef ( char *Section, char *Label, char *pFormat, ... )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   va_list marker;
   char    *buffer;
   int      rc;
   size_t  size;

   // It's probably safe to use a buffer length of 512 bytes or five times
   // the length of the format string.
   size = strlen( pFormat );
   size = (size > 512) ? 512 : size * 5;
   buffer = (char *)malloc( size );
   if ( buffer == NULL )
      return -1;
   va_start( marker, pFormat );
   rc = vsprintf( buffer, pFormat, marker );
   if (rc<0)
      return -1;
   va_end( marker );

   rc = Write ( Section, Label, buffer );

   free( buffer );
   return rc;
}

int tIniFile::Write ( char *Section, char *Label, char *pData )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ 1. To create or modify Filename:[Section] Label=Value
//³      IniFileWrite ( Filename, Section, Label, Value )
//³ 2. To create or modify Filename:Label=Value
//³      IniFileWrite ( Filename, "", Label, Value )
//³ 3. To create Filename:[Section] Label
//³      IniFileWrite ( Filename, Section, Label, "" )
//³ 4. To create Filename:Label
//³      IniFileWrite ( Filename, "", Label, "" )
//³
//³ 5. To remove Filename:[Section] Label=Value
//³      IniFileWrite ( Filename, Section, Label, NULL )
//³ 6. To remove Filename:[Section] Label
//³      IniFileWrite ( Filename, Section, Label, NULL )
//³ 7. To remove Filename: Label=Value
//³      IniFileWrite ( Filename, "", Label, NULL )
//³ 8. To remove Filename: Label
//³      IniFileWrite ( Filename, "", Label, NULL )
//³ 9. To remove Filename:[Section]
//³      IniFileWrite ( Filename, Section, NULL, any )
//³10. To remove Filename
//³      IniFileWrite ( Filename, NULL, any, any )
//³
//³ Sections and files are created as required
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   FILE *pTempFile;
   char *p;
   int   SectionFound         = 0;
   int   LabelDone            = FALSE;
   int   NextSectionFound     = FALSE;
   //LONG  FilePos              = 0L;
   int   DeletingSection      = 0;
   char  Str          [256];
   char  dir          [_MAX_DIR];
   char  drive        [_MAX_DRIVE];
   char  filename     [_MAX_FNAME];
   char  fileext      [_MAX_EXT];
   char  TempFilename [_MAX_PATH] = {0};
   int rc;

   LastIniErrorMessage[0] = 0;

#ifdef DEBUG
   //Debugf("\n\n\nPdata is<%s>\n\n\n", pData );
#endif
   Close ();

   if (Exists())
   {
      if (!HasWriteAccess())
      {
         sprintf ( LastIniErrorMessage,"Access denied: \"%s\"", pIniFilename );
         rc = ERC_ACCESSDENIED;
#ifdef DEBUG
         printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
         return rc;
      }
   }

   if ( Section == NULL )   // delete the file
   {
       if (remove ( pIniFilename ) != 0)
       {
          sprintf ( LastIniErrorMessage,"Deletion of \"%s\" failed.", pIniFilename );
          rc = ERC_DELETEFAILED;
#ifdef DEBUG
          printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
          return rc;
       }
       return 0;
   }

   if ( strlen(Section) == 0 ) SectionFound = 1;

   _splitpath ( pIniFilename, drive, dir, filename, fileext );
   strcpy ( TempFilename, "inXXXXXX" );
   _makepath ( TempFilename, drive, dir, "inXXXXXX", "" );
   if ( _mktemp(TempFilename) == 0 )
   {
      sprintf ( LastIniErrorMessage,"Cannot create unique temporary filename" );
      rc = ERC_CREATETEMPFILEFAILED;
#ifdef DEBUG
      printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }

   pTempFile = fopen ( TempFilename, "wt" );
   if ( pTempFile == 0)
   {
      sprintf ( LastIniErrorMessage,"Open failed: \"%s\"", TempFilename );
      rc = ERC_OPENFAILED;
#ifdef DEBUG
      printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }

   fIniFile = fopen ( pIniFilename,"r+t");
   if ( fIniFile == NULL )
   {
       // Brand new .INI file
       if ( Label!=NULL )  /* if label==NULL delete section */
       {
          fputs_section ( Section, pTempFile );
          fputs_label ( Label, pData, pTempFile );
       }
       fclose ( pTempFile );
       // Jonnie, Thu 14-Nov-1996, 12:44:29
       // Check that file exists before trying to delete it.
       // If you try to delete a file that doesn't exist, an error 3 is returned incorrectly.
       if (Exists ())
       {
          if (remove ( pIniFilename ) != 0)
          {
             sprintf ( LastIniErrorMessage,"Deletion of \"%s\" failed. \"%s\" contains the changes", pIniFilename, TempFilename );
             rc = ERC_DELETEFAILED;
#ifdef DEBUG
             printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
             return rc;
          }
       }
       if (rename ( TempFilename, pIniFilename ) != 0)
       {
          sprintf ( LastIniErrorMessage,"Rename of file from \"%s\" to \"%s\" failed", TempFilename, pIniFilename );
          rc = ERC_RENAMEFAILED;
#ifdef DEBUG
          printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
          return rc;
       }
       return 0;
   }

   for (;;)
   {
      Str[0] = 0;
      if (fgets(Str,255,fIniFile) == 0)
         break;

      //if (feof(fIniFile)) break;
      //if (strlen(Str)==0) continue; // basically end of file

      if (Str[strlen(Str)-1] == '\n') Str[strlen(Str)-1] = 0;
      p = Str;
      if ( IsComment ( p ) )
      {
         fputs_lf ( Str, pTempFile );
         continue;
      }
      p = SkipWhiteSpaces ( p );

      if (!SectionFound)
      {
         if ( IsSection ( p, Section ) ) // specific section
         {
            SectionFound = 1;
            if ( Label==NULL )  /* if label==NULL delete section */
            {
               DeletingSection = 1;
               continue;
            }
         }
         fputs_lf ( Str, pTempFile );
      }
      else
      {
         if ( IsSection ( p, NULL ) ) // any section
         {
            if ( DeletingSection )
            {
               fputs_lf ( Str, pTempFile ); // write next "[section]"
               LabelDone = TRUE;
            }
            NextSectionFound = TRUE;
            break;
         }

         if ( IsLabel ( p, Label ) )
         {
            fputs_label ( Label, pData, pTempFile );
            LabelDone = TRUE;
            break;
         }
         else
         {
            if ( !DeletingSection )
               fputs_lf ( Str, pTempFile );
         } // IsLabel
      } // SectionFound
   } // for

   if (!SectionFound)
   {
       if ( Label != NULL )
       {
          fputs_section ( Section, pTempFile );
          fputs_label ( Label, pData, pTempFile );
       }
   }
   else
   {
       if ( !LabelDone )
       {
          if ( pData == NULL )
          {
             if ( Label != NULL )
                fputs_lf ( Str, pTempFile );
          }
          else
          {
             fputs_label ( Label, pData, pTempFile );
             if ( NextSectionFound )
                fputs_lf ( Str, pTempFile );
          }
       }
   } // SectionFound

   // Copy remainder of file
   for (;;)
   {
      Str[0] = 0;
      if ( fgets(Str,255,fIniFile) == 0 )
         break;
      //if (feof(fIniFile)) break;
      //if (strlen(Str)==0) continue; // basically end of file
      fputs ( Str, pTempFile );
   }

   Close ();
   fclose ( pTempFile );
   if (remove ( pIniFilename ) != 0)
   {
      sprintf ( LastIniErrorMessage,"Deletion of \"%s\" failed. \"%s\" contains the changes", pIniFilename, TempFilename );
      rc = ERC_DELETEFAILED;
#ifdef DEBUG
      printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }
   if (rename ( TempFilename, pIniFilename ) != 0)
   {
      sprintf ( LastIniErrorMessage,"Rename of file from \"%s\" to \"%s\" failed", TempFilename, pIniFilename );
      rc = ERC_RENAMEFAILED;
#ifdef DEBUG
      printf("IniFileWrite Error %d : %s (LINE %d)\n",rc,LastIniErrorMessage,__LINE__); getch();
#endif
      return rc;
   }
   return 0;
}

char *IniFileLastErrorMessage ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   return LastIniErrorMessage;
}
