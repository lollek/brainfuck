section .text
	global _start
_put:				; sys_write(stdout, ebp, 1)
	mov	eax,4		; sys_write
	mov	ebx,1		; stdout
	mov	ecx,ebp
	int	80h
	ret
_get:				; sys_read(stdin, ebp, 1)
	mov	eax,3		; sys_read
	mov	ebx,0		; stdin
	mov	ecx,ebp
	int	80h
	ret
_start:
	sub	esp,8000	; Init stack
	mov	ebp,esp		; Set ptr to start of stack
	mov	edx,1		; edx (for _put and _get) is always 1
	inc	ebp
	add	byte[ebp],8
	cmp	byte[ebp],0
	jz	end0_0
tag0_0:	dec	ebp
	add	byte[ebp],9
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_0
end0_0:	dec	ebp
	call	_put
	add	ebp,2
	inc	byte[ebp]
	inc	ebp
	inc	byte[ebp]
	inc	ebp
	add	byte[ebp],2
	inc	ebp
	cmp	byte[ebp],0
	jz	end0_1
tag0_1:	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_1
end0_1:	inc	byte[ebp]
	dec	ebp
	cmp	byte[ebp],0
	jz	end0_2
tag0_2:	inc	ebp
	cmp	byte[ebp],0
	jz	end1_0
tag1_0:	dec	byte[ebp]
	inc	ebp
	inc	byte[ebp]
	sub	ebp,2
	add	byte[ebp],4
	inc	ebp
	cmp	byte[ebp],0
	jnz	tag1_0
end1_0:	sub	ebp,2
	cmp	byte[ebp],0
	jnz	tag0_2
end0_2:	inc	ebp
	call	_put
	add	byte[ebp],7
	call	_put
	call	_put
	add	byte[ebp],3
	call	_put
	inc	ebp
	dec	byte[ebp]
	inc	ebp
	add	byte[ebp],7
	call	_put
	dec	ebp
	inc	byte[ebp]
	cmp	byte[ebp],0
	jz	end0_3
tag0_3:	inc	ebp
	cmp	byte[ebp],0
	jz	end1_1
tag1_1:	inc	byte[ebp]
	inc	ebp
	cmp	byte[ebp],0
	jnz	tag1_1
end1_1:	inc	ebp
	cmp	byte[ebp],0
	jnz	tag0_3
end0_3:	sub	ebp,3
	add	byte[ebp],15
	call	_put
	add	ebp,2
	call	_put
	add	byte[ebp],3
	call	_put
	sub	byte[ebp],6
	call	_put
	sub	byte[ebp],8
	call	_put
	add	ebp,2
	inc	byte[ebp]
	call	_put
	inc	ebp
	add	byte[ebp],4
	call	_put
	mov	eax,1		; sys_exit(0)
	mov	ebx,0
	int	80h
