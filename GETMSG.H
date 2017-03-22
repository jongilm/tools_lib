
extern char *GetErrorMessage ( unsigned int ErcReqd, char *szMessage81, char *szFilename );
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
