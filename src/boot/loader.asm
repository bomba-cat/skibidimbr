	org 0x7C00
	bits 16
	; Assembler Stuff


	; Here comes the image binary
	; Following rules occur:
	; - 22x22
	; - Silly


; Pad for the image
times 484 db 0


	; Here we write the whole draw logic


; Pad for the MBR Signature
times 510 - ($-$$) db 0
dw 0xAA55
