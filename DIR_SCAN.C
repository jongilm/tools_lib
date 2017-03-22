
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

void scan( char *name, void (*dirfunction)(void))
{
   struct find_t dir;

   chdir(name);
   if (dirfunction != NULL) (dirfunction)();

   if( !_dos_findfirst( "*.*", _A_SUBDIR, &dir ) )
   {
      if ((dir.name[0] != '.') && (dir.attrib & 0x10) )
         scan(dir.name,dirfunction);

      while( !_dos_findnext( &dir ) )
      {
         if ((dir.name[0] != '.') && (dir.attrib & 0x10) )
            scan(dir.name,dirfunction);
      }
   }

   chdir("..");
}

void dir_scan(void (*diskfunction)(void),void (*dirfunction)(void))
{
   int        num_drives;
   int        drive;
   int        test_drive;

   _dos_setdrive(3,&num_drives);

   for (drive=3; drive<=num_drives; drive++)
   {

      _dos_setdrive(drive,&num_drives);
      _dos_getdrive(&test_drive);

      if ( drive == test_drive )
      {
         if (diskfunction != NULL) (diskfunction)();
         scan("\\",dirfunction);
      }
   }

}
