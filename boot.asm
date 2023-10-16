BITS 16

global _start

_start:
	mov ax, 0x7c0
	mov ds, ax
	mov ax, 0xb800
	mov es, ax

	mov ax, 3
	int 0x10

	mov cx, 8
	mov si, key

.read:
	mov ah, 0
	int 0x16
	mov [si], al
	inc si
	loop .read

	xor cx, cx
	mov byte cl, [msgsize]
	mov ax, cx
	mov bx, 16
	idiv bx
	xor ah, ah
	mov bx, 0xb87c
	sub bx, ax
	mov es, bx

	xor ax, ax
	xor bx, bx

	mov di, msg
decrypt:
	mov si, key
d:
	cmp cx, 0
	jz dend

	mov byte al, [si]
	xor byte [di], al
	inc di
	inc si
	dec cx

	cmp si, key + 8
	jz decrypt
	jmp d

dend:
	xor di, di
	mov si, msg
	mov ah, 0x5f

next:
	lodsb

	test al, al
	jz end

	stosw
	jmp next

end:
    jmp $
key:
	times 8 db 0
msgsize:
	db 0xFF
msg:
	times 3 db 0xFF
    times 510-($-$$) db 0
    dw 0xaa55
        
