        title   ITOH - 16-bits to hex ASCII

; ITOH ---  converts 16-bit unsigned integer
;           into a "hexadecimal" ASCII string.
; (C) 1988 Ray Duncan
;
; Call with     AX    = value to convert
;               DS:BX = address to store 4-character string
;
; Returns       AX, BX destroyed, other registers preserved

_TEXT   segment word public 'CODE'

        assume  cs:_TEXT

        public  itoh
itoh    proc    near

        push    cx              ; save registers 
        push    dx

        mov     dx,4            ; initialize char. counter

itoh1:  mov     cx,4            ; isolate next four bits
        rol     ax,cl
        mov     cx,ax
        and     cx,0fh
        add     cx,'0'          ; convert to ASCII 
        cmp     cx,'9'          ; is it 0-9?
        jbe     itoh2           ; yes, jump
        add     cx,'A'-'9'-1    ; add correction for A-F

itoh2:                          ; store this character
        mov     [bx],cl
        inc     bx              ; bump string pointer

        dec     dx              ; count characters converted
        jnz     itoh1           ; loop, not four yet

        pop     dx              ; restore registers
        pop     cx
        ret                     ; back to caller

itoh    endp

_TEXT   ends

        end

