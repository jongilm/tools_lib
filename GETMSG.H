
extern char *GetErrorMessage ( unsigned int ErcReqd, char *szMessage81, char *szFilename );
//旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
//� GetErrorMessage                         Jonnie, Fri 03-Jan-1997, 13:46:29
//� ---------------
//�
//� If szFilename is NULL, the last filename specified is used. 
//�
//� If an error occurs one of the following are returned:
//�    "*** Unable to retrieve message. Insufficient memory ***"
//�    "*** Unable to retrieve message. Filename not specified ***"
//�    "*** Unable to retrieve message. Failed to open file ***"
//�
//� The message file must have the following format:
//�
//� <Leader><Code><Delimiter><Message><Terminator>
//� Where: <Leader>    = Zero or more non-numeric characters (ignored).
//�        <Code>      = Numeric message code (value 1..65535).
//�        <Delimiter> = Any non-numeric character (ignored).
//�        <Message>   = Any message string (first 80 chars are significant).
//�        <Terminator>= CR or LF or both
//�
//� eg. If the file c:\doddle\ERRORS.MSG contained the line 
//�           "   00027 Sector not found\r\n" 
//�
//�     char TempBuffer81[81];
//�     GetErrorMessage(27,TempBuffer81,"c:\\doddle\\ERRORS.MSG");
//�     would return with "Sector not found\0" in TempBuffer81.
//�
//읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
