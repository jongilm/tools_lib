
CONST


!зддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
!Ё                                                         Ё
!Ё            BTRIEVE KEY FLAG VALUES                      Ё
!Ё                                                         Ё
!юддддддддддддддддддддддддддддддддддддддддддддддддддддддддды




       BTRV_KF_Duplicates   = 2#0000000001   ;
       BTRV_KF_Modifiable   = 2#0000000010   ;
       BTRV_KF_Binary       = 2#0000000100   ;
       BTRV_KF_Null         = 2#0000001000   ;
       BTRV_KF_Segmented    = 2#0000010000   ;
       BTRV_KF_AltColSeq    = 2#0000100000   ;
       BTRV_KF_Descending   = 2#0001000000   ;
       BTRV_KF_Supplemental = 2#0010000000   ;
       BTRV_KF_Extended     = 2#0100000000   ;
       BTRV_KF_Manual       = 2#1000000000   ;



!зддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
!Ё                                                         Ё
!Ё                    FLAG VALUES                          Ё
!Ё                                                         Ё
!юддддддддддддддддддддддддддддддддддддддддддддддддддддддддды




       BTRV_F_VariableLen   = 2#00000001   ;
       BTRV_F_BlankTrunc    = 2#00000010   ;
       BTRV_F_PreAllocation = 2#00000100   ;
       BTRV_F_DataComp      = 2#00001000   ;
       BTRV_F_KeyOnly       = 2#00010000   ;
       BTRV_F_10Free        = 2#01000000   ;
       BTRV_F_20Free        = 2#10000000   ;
       BTRV_F_30Free        = 2#11000000   ;



!зддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
!Ё                                                         Ё
!Ё                EXTENDED KEY TYPES                       Ё
!Ё                                                         Ё
!юддддддддддддддддддддддддддддддддддддддддддддддддддддддддды




       BTRV_KT_String        = 0  ;
       BTRV_KT_Integer       = 1  ;
       BTRV_KT_Float         = 2  ;
       BTRV_KT_Date          = 3  ;
       BTRV_KT_Time          = 4  ;
       BTRV_KT_Decimal       = 5  ;
       BTRV_KT_Money         = 6  ;
       BTRV_KT_Logical       = 7  ;
       BTRV_KT_Numeric       = 8  ;
       BTRV_KT_BFloat        = 9  ;
       BTRV_KT_LString       = 10 ;
       BTRV_KT_ZString       = 11 ;
       BTRV_KT_UnsBin        = 14 ;
       BTRV_KT_AutoIncr      = 15 ;






!зддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
!Ё                                                         Ё
!Ё                OPERATION CODES                          Ё
!Ё                                                         Ё
!юддддддддддддддддддддддддддддддддддддддддддддддддддддддддды




       BTRV_OP_Open          = 0  ;
       BTRV_OP_Close         = 1  ;
       BTRV_OP_Insert        = 2  ;
       BTRV_OP_Update        = 3  ;
       BTRV_OP_Delete        = 4  ;
       BTRV_OP_GetEqual      = 5  ;
       BTRV_OP_GetNext       = 6  ;
       BTRV_OP_GetPrevious   = 7  ;
       BTRV_OP_GetGreater    = 8  ;
       BTRV_OP_GetGtrOrEql   = 9  ;
       BTRV_OP_GetLess       = 10 ;
       BTRV_OP_GetLessOrEql  = 11 ;
       BTRV_OP_GetFirst      = 12 ;
       BTRV_OP_GetLast       = 13 ;
       BTRV_OP_Create        = 14 ;
       BTRV_OP_Stat          = 15 ;
       BTRV_OP_Extend        = 16 ;
       BTRV_OP_SetDir        = 17 ;
       BTRV_OP_GetDir        = 18 ;
       BTRV_OP_BeginTran     = 19 ;
       BTRV_OP_EndTran       = 20 ;
       BTRV_OP_AbortTran     = 21 ;
       BTRV_OP_GetPosition   = 22 ;
       BTRV_OP_GetDirect     = 23 ;
       BTRV_OP_StepNext      = 24 ;
       BTRV_OP_Stop          = 25 ;
       BTRV_OP_Version       = 26 ;
       BTRV_OP_Unlock        = 27 ;
       BTRV_OP_Reset         = 28 ;
       BTRV_OP_SetOwner      = 39 ;
       BTRV_OP_ClearOwner    = 30 ;
       BTRV_OP_CreateSupInd  = 31 ;
       BTRV_OP_DropSupInd    = 32 ;
       BTRV_OP_StepFirst     = 33 ;
       BTRV_OP_StepLast      = 34 ;
       BTRV_OP_StepPrevious  = 35 ;
       BTRV_OP_GetNextExt    = 36 ;
       BTRV_OP_GetPrevExt    = 37 ;
       BTRV_OP_StepNextExt   = 38 ;
       BTRV_OP_StepPrevExt   = 39 ;
       BTRV_OP_InsertExt     = 40 ;
       BTRV_OP_GetKey        = 50 ;


TYPE



!зддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
!Ё                                                         Ё
!Ё              BTRIEVE FILE SPECIFICATION                 Ё
!Ё                                                         Ё
!юддддддддддддддддддддддддддддддддддддддддддддддддддддддддды


        BTRV_KeySpec = RECORD
                         KeyPos       : INTEGER   ;
                         KeyLen       : INTEGER   ;
                         KeyFlags     : INTEGER   ;
                         NotUsed      : STRING(4);
                         ExtendedType : BYTE;
                         KeyRsvd1     : BYTE ;
                         KeyRsvd      : ARRAY [1..4] OF Byte ;
                       END ;




        BTRV_FileSpec = RECORD
             CASE INTEGER OF
                      1: (RecLen   : INTEGER   ;
                          PageSize : INTEGER   ;
                          IndxCnt  : INTEGER   ;
                          NotUsed  : STRING(4) ;
                          FileFlags: INTEGER   ;
                          Reserved : STRING(2) ;
                          PreAlloc : INTEGER   ;
                          KeyBuf   : ARRAY [0..24] OF BTRV_KeySpec) ;
                      2: (SpecBuf  : STRING(416)    ) ;
                        END ;



        BTRV_Filter   = RECORD
                          FldType  : BYTE    ;
                          FldLen   : INTEGER ;
                          FldOff   : INTEGER ;
                          CompCode : BYTE    ;
                          LogicExp : BYTE    ;
                          CompVal  : ARRAY [1..2] OF CHAR ;
                        END ;




        BTRV_Extract  = RECORD
                          FldLen   : INTEGER ;
                          FldOff   : INTEGER ;
                        END ;

{								      }
{								      }
{   Interface to the Btrieve Record Manager			      }
{								      }
function BTRV	 (     OP	   : integer;
		  vars POS_BLOCK   : string;
                  vars DATA_BUFFER : string;
		  vars DATA_LENGTH : integer;
		  vars KEY_BUFFER  : string;
		       KEY_NUMBER  : integer)	 : integer; extern;

