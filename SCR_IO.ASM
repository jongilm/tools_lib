;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³        File : scr_io.asm                                                   ³
;³      Author : Jonathan Gilmore                                             ³
;³        Date : 29 March 1990                                                ³
;³ Description : Screen handling routines with snow suppression               ³                                                             ³
;ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
;³         Copyright (c) Computer Lab Jhb.  All rights reserved.              ³
;ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
;³  Assembling : MASM /Mx Scr_IO.ASM;                                         ³
;³ C Interface : #include <scr_io.h>.                                         ³
;³     Linking : No special instructions.                                     ³
;³                                                                            ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

_TEXT     SEGMENT    BYTE   PUBLIC   'CODE'
_TEXT     ENDS
_DATA     SEGMENT    WORD   PUBLIC   'DATA'
_DATA     ENDS
CONST     SEGMENT    WORD   PUBLIC   'CONST'
CONST     ENDS
_BSS      SEGMENT    WORD   PUBLIC   'BSS'
_BSS      ENDS

IFDEF LMODEL
FAR_BSS   SEGMENT    PARA   PUBLIC   'FAR_BSS'
FAR_BSS   ENDS
ENDIF

DGROUP    GROUP  CONST, _BSS, _DATA
          ASSUME     CS:_TEXT, DS:DGROUP, SS:DGROUP, ES:DGROUP

_DATA     SEGMENT
          PUBLIC     __scr_seg
          PUBLIC     __scr_cols
          PUBLIC     __scr_snow
_DATA     ENDS

_TEXT     SEGMENT
          PUBLIC     __put_char
          PUBLIC     __put_attr
          PUBLIC     __get_char
          PUBLIC     __get_attr
          PUBLIC     __put_chars
          PUBLIC     __put_attrs
          PUBLIC     __get_chars
          PUBLIC     __get_attrs
          PUBLIC     __put_string
          PUBLIC     __put_video
          PUBLIC     __get_video
          PUBLIC     __put_field
          PUBLIC     __put_text
          PUBLIC     __put_chars_xp
          PUBLIC     __put_attrs_xp

__scr_initialised dw  0
__scr_x           dw  ?
__scr_y           dw  ?
__scr_seg         dw  0B800h
__scr_cols        dw  80
__scr_snow        dw  0

;*****************************************************************************
; __SCR_INIT
;
; void _scr_init(void);
;
; At this point the stack contains the 4 byte far return address at bp+0
;*****************************************************************************
__scr_init PROC       FAR
   cmp     __scr_initialised,0
   jg      scr_init_end

   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   mov     ah,0Fh                ; Get current video mode / columns
   int     10h
   cmp     al,07h                ; if (al < 0x07)
   jge     not_cga
   mov     __scr_seg,0B800h      ;    __scr_seg  = 0B800h ; CGA
   jmp     carry_on
not_cga:
   cmp     al,0Bh                ; else if (al < 0x0B)
   jge     not_mono
   mov     __scr_seg,0B000h      ;    __scr_seg  = 0B000h ; Mono
   jmp     carry_on
not_mono:                        ; else
   mov     __scr_seg,0A000h      ;    __scr_seg  = 0A000h ; EGA/VGA graphics
carry_on:
   mov     al,ah                 ; move cols to al
   mov     ah,0
   mov     __scr_cols,ax         ; save character columns per line

   mov     ah,12h                ; Alternate select
   mov     bl,10h                ; sub-service 10h = EGA information
   int     10h
   mov     __scr_snow,0          ; Initialise to no snow
   cmp     bl,3                  ; If (bl<=3) ie If memory size is valid
   jle     finish_off            ; Then EGA or VGA
   cmp     __scr_seg,0B800h      ; If video seg = B800 (current video mode < 7)
   jne     finish_off            ; Then monochrome
   mov     __scr_snow,1          ; else CGA

; Alternative to checking current video mode is to check initial video mode
; int  11h                       ; Read equipment list
; if ((ax & 0x30) <= 0x20)       ; If initial video mode <> mono
;    __scr_snow = 1;             ; then CGA

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
finish_off:
   mov     __scr_initialised,1   ; So we don't do it all again
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
scr_init_end:
   ret
__scr_init ENDP






;*****************************************************************************
; __PUT_CHAR
;
; void _put_char(int x, int y, int qty, int ch);
;
; At this point the stack contains the 4 byte far return address at bp+0
;*****************************************************************************
__put_char PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; Parameter 5: quantity
   mov     ax,[bp+20]            ; Parameter 6: character
   mov     bl,al                 ; and then to bl

   cmp     cx,0                  ; if length = 0
   jz      a99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      a03                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
a01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      a01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
a02:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     a02                   ; Loop until either bit is on
   mov     al,bl                 ; Move char to al
   stosb                         ; and then to screen
   sti                           ; Interrupts back on
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     a99
   jmp     a01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
a03:
   mov     al,bl                 ; move char to al
   stosb                         ; and then to screen
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     a99
   jmp     a03

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
a99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_char ENDP






;*****************************************************************************
; __PUT_ATTR
;
; void _put_attr( int x, int y, int qty, int colour);
;
; At this point the stack contains the 4 byte far return address at bp+0
;*****************************************************************************
__put_attr PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   inc     ax                    ; point to colour byte
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; Parameter 5: quantity
   mov     ax,[bp+20]            ; Parameter 5: colour
   mov     bl,al                 ; and then to bl

   cmp     cx,0                  ; if length = 0
   jz      b99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      b03                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
b01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      b01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
b02:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     b02                   ; Loop until either bit is on
   mov     al,bl                 ; Move char to al
   stosb                         ; and then to screen
   sti                           ; Interrupts back on
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     b99
   jmp     b01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
b03:
   mov     al,bl                 ; move char to al
   stosb                         ; and then to screen
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     b99
   jmp     b03

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
b99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_attr ENDP






;*****************************************************************************
; __GET_CHAR
;
; int _get_char(int x, int y);
;
; At this point the stack contains the 4 byte far return address at bp+0
;*****************************************************************************
__get_char PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     ds,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     si,ax                 ; Screen offset in si

   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      c03                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
c01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      c01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
c02:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     c02                   ; Loop until either bit is on
   lodsb                         ; get byte from screen
   xor     ah,ah                 ; clear ah
   sti                           ; Interrupts back on
   jmp     c99

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
c03:
   lodsb                         ; get byte from screen
   xor     ah,ah                 ; clear ah

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
c99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__get_char ENDP






;*****************************************************************************
; __GET_ATTR
;
; int _get_attr( int x, int y);
;
; At this point the stack contains the 4 byte far return address at bp+0
;*****************************************************************************
__get_attr PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     ds,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   inc     ax                    ; point to colour byte
   mov     si,ax                 ; Screen offset in si

   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      d03                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
d01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      d01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
d02:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     d02                   ; Loop until either bit is on
   lodsb                         ; get byte from screen
   xor     ah,ah                 ; clear ah
   sti                           ; Interrupts back on
   jmp     d99

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
d03:
   lodsb                         ; get byte from screen
   xor     ah,ah                 ; clear ah

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
d99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__get_attr ENDP






;*****************************************************************************
; __PUT_CHARS
;
; void _put_chars( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_chars PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      e99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      e04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
e02:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      e02                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
e03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     e03                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     di                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     e99
   jmp     e02

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
e04:
   movsb                         ; es:di <- ds:si
   inc     di                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     e99
   jmp     e04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
e99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_chars ENDP






;*****************************************************************************
; __PUT_ATTRS
;
; void _put_attrs( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_attrs PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   inc     ax                    ; point to colour byte
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      f99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      f04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
f01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      f01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
f03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     f03                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     di                    ; skip over character byte
   dec     cx
   cmp     cx,0
   jbe     f99
   jmp     f01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
f04:
   movsb                         ; es:di <- ds:si
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     f99
   jmp     f04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
f99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_attrs ENDP






;*****************************************************************************
; __GET_CHARS
;
; void _get_chars( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__get_chars PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     ds,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     si,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   les     di,[bp+20]            ; zstring offset (di=low word,es=high word)

   cmp     cx,0                  ; if length = 0
   jz      g99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      g04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
g01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      g01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
g03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     g03                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     si                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     g99
   jmp     g01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
g04:
   movsb                         ; es:di <- ds:si
   inc     si                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     g99
   jmp     g04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
g99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__get_chars ENDP






;*****************************************************************************
; __GET_ATTRS
;
; void _get_attrs( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__get_attrs PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     ds,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   inc     ax                    ; point to colour byte
   mov     si,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   les     di,[bp+20]            ; zstring offset (di=low word,es=high word)

   cmp     cx,0                  ; if length = 0
   jz      h99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      h04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
h01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      h01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
h03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     h03                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     si                    ; skip over character byte
   dec     cx
   cmp     cx,0
   jbe     h99
   jmp     h01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
h04:
   mov     al,bl                 ; move char to al
   movsb                         ; es:di <- ds:si
   inc     si                    ; skip over character byte
   dec     cx
   cmp     cx,0
   jbe     h99
   jmp     h04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
h99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__get_attrs ENDP






;*****************************************************************************
; __PUT_STRING
;
; void _put_string( int x, int y, char *zstr);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_string PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   lds     si,[bp+18]            ; zstring offset (si=low word,ds=high word)

   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      i04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
i01:
   mov     al,ds:[si]            ;
   cmp     al,0                  ; if end of zstring
   jbe     i99                   ; goto end
i02:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      i02                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
i03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     i03                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     di                    ; skip over attribute byte
   jmp     i01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
i04:
   mov     al,ds:[si]            ;
   cmp     al,0                  ; if end of zstring
   jbe     i99
   movsb                         ; es:di <- ds:si
   inc     di                    ; skip over attribute byte
   jmp     i04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
i99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_string ENDP






;*****************************************************************************
; __PUT_VIDEO
;
; void _put_video( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_video PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      j99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      j04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
j02:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      j02                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
j03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     j03                   ; Loop until either bit is on
   movsw                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   dec     cx
   cmp     cx,0
   jbe     j99
   jmp     j02

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
j04:
   movsw                         ; es:di <- ds:si
   dec     cx
   cmp     cx,0
   jbe     j99
   jmp     j04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
j99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_video ENDP






;*****************************************************************************
; __GET_VIDEO
;
; void _get_video( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__get_video PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     ds,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     si,ax                 ; Screen offset in si

   mov     cx,[bp+18]            ; length in cx
   les     di,[bp+20]            ; zstring offset (di=low word,es=high word)

   cmp     cx,0                  ; if length = 0
   jz      k99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      k04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
k01:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      k01                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
k03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     k03                   ; Loop until either bit is on
   movsw                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   dec     cx
   cmp     cx,0
   jbe     k99
   jmp     k01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
k04:
   movsw                         ; es:di <- ds:si
   dec     cx
   cmp     cx,0
   jbe     k99
   jmp     k04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
k99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__get_video ENDP






;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³   _put_field                                                               ³
;³                                                                            ³
;³   This function puts a string on the screen, filling to length with colour ³
;³                                                                            ³
;³   void _put_field( int x, int y,                                           ³
;³                    int colour,int fillcolour,                              ³
;³                    int length, char *str);                                 ³
;³                                                                            ³
;³   Argument list:    int x          The column at which to place the string ³
;³                     int y          The row at which to place the string    ³
;³                     int colour     The colour attribute of 'line'          ³
;³                     int fillcolour The colour of the spaces after 'line'   ³
;³                     int length     The number of chars to put              ³
;³                     char *line     A pointer to the string                 ³
;³                                                                            ³
;³   Return value:     void                                                   ³
;³                                                                            ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
__put_field PROC       FAR
                                 ; At this point the stack contains the
                                 ; return address at bp+0
   push    bp                    ; bp must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     ax,[bp+18]            ; colour
   mov     bh,al
   mov     cx,[bp+22]            ; length in cx
   lds     si,[bp+24]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      l99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      l04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
l01:
   mov     bl,ds:[si]            ; move char to bl
   cmp     bl,0
   jbe     l05
l02:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      l02                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
l03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     l03                   ; Loop until either bit is on
   mov     ax,bx                 ; move char to ax
   stosw                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     si                    ; point to next char
   dec     cx
   cmp     cx,0
   jbe     l99
   jmp     l01
l05:
   mov     bl,' '                ; put spaces from now on
   mov     bh,byte ptr [bp+20]   ; read color to use for NULL
   cli                           ; no interrupts here
   in      al,dx                 ; read the port registers
   rcr     al,1                  ; test for bit 1 on
   jb      l05                   ; loop until bit 1 is off
l06:
   in      al,dx                 ; read port registers again
   rcr     al,1                  ; test for bit 1 or 3 off
   jnb     l06                   ; loop until either bit is on
   mov     ax,bx                 ; move char to ax
   stosw                         ; and then to screen
   sti
   dec     cx
   cmp     cx,0
   ja      l06
   jmp     l99


;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
l04:
   mov     bl,ds:[si]            ; move char to bl
   cmp     bl,0
   jbe     l07
   mov     ax,bx                 ; move char to ax
   stosw                         ; and then to screen
   inc     si                    ; point to next char
   dec     cx
   cmp     cx,0
   jbe     l99
   jmp     l04
l07:
   mov     al,' '                ; put spaces from now on
   mov     ah,byte ptr [bp+20]   ; read color to use for NULL
   stosw
   dec     cx
   cmp     cx,0
   ja      l07

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
l99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_field ENDP






;*****************************************************************************
; __PUT_TEXT
;
; void _put_text( int x, int y, int colour, char *zstr);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_text PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     ax,[bp+18]            ; colour
   mov     bh,al                 ; in bh
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      m04                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
m01:
   mov     bl,ds:[si]            ;
   cmp     bl,0                  ; if end of zstring
   jbe     m99                   ; goto end
m02:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      m02                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
m03:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     m03                   ; Loop until either bit is on
   mov     ax,bx                 ; move char to ax
   stosw                         ; es:di <- ax
   sti                           ; Interrupts back on
   inc     si                    ; next byte to display
   jmp     m01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
m04:
   mov     bl,ds:[si]            ;
   cmp     bl,0                  ; if end of zstring
   jbe     m99
   mov     ax,bx                 ; move char to ax
   stosw                         ; es:di <- ds:si
   inc     si                    ; next byte to display
   jmp     m04

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
m99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_text ENDP


;*****************************************************************************
; __PUT_CHARS_XP
;
; void _put_chars_xp( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_chars_xp PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      n99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      n06                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
n02:
   mov     bl,ds:[si]            ;
   cmp     bl,20h                ; if space
   je      n03                   ; skip it
   jmp     n04
n03:
   inc     si
   inc     di
   inc     di
   dec     cx
   cmp     cx,0
   jbe     n99
   jmp     n02
n04:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      n04                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
n05:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     n05                   ; Loop until either bit is on
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on

   inc     di                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     n99
   jmp     n02

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
n06:
   mov     bl,ds:[si]            ;
   cmp     bl,20h                ; if space
   je      n07                   ; skip it
   movsb                         ; es:di <- ds:si
   jmp     n08
n07:
   inc     si
   inc     di
n08:
   inc     di                    ; skip over attribute byte
   dec     cx
   cmp     cx,0
   jbe     n99
   jmp     n06

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
n99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_chars_xp ENDP






;*****************************************************************************
; __PUT_ATTRS_XP
;
; void _put_attrs_xp( int x, int y, int length, char *str);
;
; At this point the stack contains the return address at bp+0
;*****************************************************************************
__put_attrs_xp PROC       FAR
   push    bp                    ; Must be saved on the stack bp+4
   push    es                    ; bp+6
   push    di                    ; pb+8
   push    ds                    ; pb+10
   push    si                    ; pb+12
   mov     bp,sp                 ; Used to access stack

   call    __scr_init            ; Initialise screen parameters

   mov     es,__scr_seg          ; screen segment

   mov     ax,[bp+14]            ; Parameter 3: __scr_x
   dec     ax                    ; Change to origin 0
   mov     __scr_x,ax            ; Save for later use
   mov     ax,[bp+16]            ; Parameter 4: __scr_y
   dec     ax                    ; Change to origin 0
   mov     __scr_y,ax            ; Save for later use

   mov     ax,__scr_cols         ; Calculate x,y as an absolute offset
   imul    word ptr [__scr_y]    ; ax = total bytes in complete rows
   add     ax,word ptr [__scr_x] ; Plus incomplete row
   shl     ax,1                  ; Multiplied by 2
   inc     ax                    ; point to colour byte
   mov     di,ax                 ; Screen offset in di

   mov     cx,[bp+18]            ; length in cx
   lds     si,[bp+20]            ; zstring offset (si=low word,ds=high word)

   cmp     cx,0                  ; if length = 0
   jz      o99
   cld
   cmp     word ptr __scr_snow,0 ; If not suceptable to snow
   jz      o06                   ; jump to routine with no waits

;*****************************************************************************
; Print with snow suppression
;*****************************************************************************
   mov     dx,03DAh              ; Point DX to CGA status port
   mov     ah,9                  ; Move horiz. + vertical retrace mask to ah
o01:
   mov     bl,ds:[si]            ;
   cmp     bl,20h                ; if space
   je      o03                   ; skip it
   jmp     o04
o03:
   inc     si
   inc     di
   inc     di
   dec     cx
   cmp     cx,0
   jbe     o99
   jmp     o01

o04:
   in      al,dx                 ; Get 6845 status
   rcr     al,1                  ; Test for bit 1 on ie horizontal retrace
   jb      o04                   ; Loop if in horizontal retrace: this prevents
                                 ; starting in mid-retrace, since there is
                                 ; exactly enough time for 1 and only 1 STOSW
                                 ; during horizontal retrace (until bit 1 is off)

   cli                           ; No ints during critical section
o05:
   in      al,dx                 ; Read port registers again
   rcr     al,1                  ; Test for bit 1 or 3 off
   jnb     o05                   ; Loop until either bit is on

                                 ; Do it
   movsb                         ; es:di <- ds:si
   sti                           ; Interrupts back on
   inc     di                    ; skip over character byte
   dec     cx
   cmp     cx,0
   jbe     o99
   jmp     o01

;*****************************************************************************
; Print without snow suppression
;*****************************************************************************
o06:
   mov     bl,ds:[si]            ;
   cmp     bl,20h                ; if space
   je      o07                   ; skip it
   movsb                         ; es:di <- ds:si
   jmp     o08
o07:
   inc     si
   inc     di
o08:
   inc     di                    ; point to next screen char
   dec     cx
   cmp     cx,0
   jbe     o99
   jmp     o06

;*****************************************************************************
; Clean up and exit
;*****************************************************************************
o99:
   pop     si                    ; restore everything
   pop     ds
   pop     di
   pop     es
   pop     bp
   ret
__put_attrs_xp ENDP





_TEXT      ENDS
   END
