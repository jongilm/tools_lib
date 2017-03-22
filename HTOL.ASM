	name	HTOL
	page	55,132
	title	HTOL - Hex ASCII to long integer

;
; HTOL.ASM - convert hexadecimal ASCII string 
;            to long (32-bit) integer.
;
; (C) 1987 Ziff Davis, by Ray Duncan
; 
; Call with:	DS:SI = address of string
;
; Returns:	DX:AX = result (high word in DX)
;		DS:SI = address+1 of terminator 
;
;               other registers preserved
;
; Like the C library 'atol', this routine gives no 
; warning in the event of overflow, and terminates 
; on the first unconvertable character.
;

_TEXT	segment	word public 'CODE'

	assume	cs:_TEXT

	public	htol			; make HTOL available
					; to the Linker

htol	proc	near             	; Convert hex ASCII string
					; to 32-bit binary integer.

	push	cx			; save register
        xor     cx,cx           	; set forming answer to zero
        xor     dx,dx

htol1:	lodsb                   	; get next char., make sure
					; it is '0'-'9' or 'A'-'F'

        cmp     al,'0'
        jb      htol3        		; exit if char < '0'

	cmp	al,'9'
	jbe	htol2			; proceed if char '0'-'9'

	or	al,20h			; fold char. to lower case.

	cmp	al,'f'
	ja	htol3			; exit if > 'F' or 'f'

	cmp	al,'a'
	jb	htol3			; exit if < 'A' or 'a'

	add	al,9			; else add fudge factor
					; for digits 'A'-'F'

htol2:					; add this digit to
					; forming answer...

	shl     cx,1			; first shift current answer 
        rcl     dx,1            	; left by 4 bits...
        shl     cx,1
        rcl     dx,1            
        shl     cx,1
        rcl     dx,1            
        shl     cx,1
        rcl     dx,1            
        and     ax,0fh          	; isolate binary value 0-A
                                	; from ASCII character code,
        or 	cx,ax           	; add to the forming answer.

        jmp     htol1        		; get next character

htol3:	mov	ax,cx			; return DX:AX = value
	pop	cx			; restore register
        ret                     	; back to caller

htol	endp

_TEXT	ends

	end

