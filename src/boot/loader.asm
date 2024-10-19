	org 0x7C00
	bits 16
	; Assembler Stuff

; Also add this label so we can find the image itself in the RAM
; Image lies within:
; 0x7C00 - 0x7DE4
image:

	; Here comes the image binary
	; Following rules occur:
	; - 22x22
	; - Silly


; Pad for the image
times 242 db 0


	; Here we write the whole draw logic

setupstack:
	xor		  ax, ax
  mov 	  ds, ax
  mov 	  es, ax

  mov 	  ss, ax
  mov 	  sp, 0x7DFE

setvideomode:
	mov ah, 00h
	mov al, 13h
	int 0x10

; AL, AH = COLOR
; AX = READ COLOR COMBINATION
; BX = COUNTER (WILL BE SET TO 0 TO NOT MESS WITH PAGE COUNTER)
; CX = X
; DX = Y
setup:
  mov bx, 0
  push bx
drawmode:
  mov ah, 0Ch
readpixel:
  pop bx
  mov ax, [image + bx]
  add bx, 4
  push cx
drawpixel:
  
  

; Pad for the MBR Signature
times 510 - ($-$$) db 0
dw 0xAA55
