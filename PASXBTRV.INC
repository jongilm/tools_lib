{								      }
{   Interface to the Btrieve Record Manager			      }
{                                                                     }

function BTRV	 (     OP	   : integer;
		  vars POS_BLOCK   : string;
		  vars DATA_BUFFER : string;
		  vars DATA_LENGTH : integer;
		  vars KEY_BUFFER  : string;
		       KEY_NUMBER  : integer)	 : integer; extern;

CONST
   ! btrieve operations
   B_OPEN       = 0;
   B_CLOSE      = 1;
   B_INSERT     = 2;
   B_UPDATE     = 3;
   B_DELETE     = 4;
   B_GETEQ      = 5;
   B_GETNEXT    = 6;
   B_GETPREV    = 7;
   B_GETGR      = 8;
   B_GETGREQ    = 9;
   B_GETLESS    = 10;
   B_GETLESSEQ  = 11;
   B_GETFIRST   = 12;
   B_GETLAST    = 13;
   B_CREATE     = 14;
   B_STAT       = 15;
   B_EXTEND     = 16;
   B_SETDIR     = 17;
   B_GETDIR     = 18;
   B_BTRANS     = 19;
   B_ETRANS     = 20;
   B_ATRANS     = 21;
   B_GETPOS     = 22;
   B_GDIRECT    = 23;
   B_SDIRECT    = 24;
   B_STOP       = 25;
   B_VERS       = 26;
   B_UNLOCK     = 27;
   B_RESET      = 28;
   B_SETOWN     = 29;
   B_CLROWN     = 30;
   B_CRSUPP     = 31;
   B_DRSUPP     = 32;

   ! btrieve extended key types
   B_STRING     = 0;
   B_INT        = 1;
   B_FLOAT      = 2;
   B_DATE       = 3;
   B_TIME       = 4;
   B_DECIMAL    = 5;
   B_MONEY      = 6;
   B_LOGICAL    = 7;
   B_NUMERIC    = 8;
   B_BFLOAT     = 9;
   B_LSTRING    = 10;
   B_ZSTRING    = 11;
   B_UNSIGN     = 14;

   ! btrieve file attributes
   B_VARIABLE   = 1;
   B_BLNKCOMP   = 2;
   B_PREALLOC   = 4;

   ! btrieve key attributes
   B_DUPL       = 1;
   B_MOD        = 2;
   B_BIN        = 4;
   B_NULL       = 8;
   B_SEG        = 16;
   B_ALT        = 32;
   B_DESC       = 64;
   B_SUPPL      = 128;
   B_EXT        = 256;

TYPE
   B_KEYSPEC = RECORD
      KEY_POS          [00] : INTEGER;
      KEY_LEN          [02] : INTEGER;
      KEY_FLAG         [04] : INTEGER;
      KEY_NOTUSED      [06] : STRING(4);
      KEY_TYPE         [10] : BYTE;
      KEY_RESERVED     [11] : STRING(5);
   END;

   B_FILESPEC = RECORD
      FILE_RECLEN      [00] : INTEGER;
      FILE_PAGESIZE    [02] : INTEGER;
      FILE_NDXCOUNT    [04] : INTEGER;
      FILE_NOTUSED     [06] : STRING(4);
      FILE_FLAG        [10] : INTEGER;
      FILE_RESERVED    [12] : STRING(2);
      FILE_PREALLOC    [14] : INTEGER;
   END;

! Use as follows
!
!   FileSpec = RECORD
!      Body [00] : B_FILESPEC;
!      Key  [16] : ARRAY[1..3] OF B_KEYSPEC;
!   END;
!
