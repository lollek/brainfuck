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
	add	byte[ebp],9
	cmp	byte[ebp],0
	jz	end0_0
tag0_0:	dec	ebp
	add	byte[ebp],8
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_0
end0_0:	dec	ebp
	call	_put
	inc	ebp
	add	byte[ebp],7
	cmp	byte[ebp],0
	jz	end0_1
tag0_1:	dec	ebp
	add	byte[ebp],4
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_1
end0_1:	dec	ebp
	inc	byte[ebp]
	call	_put
	add	byte[ebp],7
	call	_put
	call	_put
	add	byte[ebp],3
	call	_put
	cmp	byte[ebp],0
	jz	end0_2
tag0_2:	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_2
end0_2:	inc	ebp
	add	byte[ebp],8
	cmp	byte[ebp],0
	jz	end0_3
tag0_3:	dec	ebp
	add	byte[ebp],4
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_3
end0_3:	dec	ebp
	call	_put
	inc	ebp
	add	byte[ebp],11
	cmp	byte[ebp],0
	jz	end0_4
tag0_4:	dec	ebp
	add	byte[ebp],5
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_4
end0_4:	dec	ebp
	call	_put
	inc	ebp
	add	byte[ebp],8
	cmp	byte[ebp],0
	jz	end0_5
tag0_5:	dec	ebp
	add	byte[ebp],3
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_5
end0_5:	dec	ebp
	call	_put
	add	byte[ebp],3
	call	_put
	sub	byte[ebp],6
	call	_put
	sub	byte[ebp],8
	call	_put
	cmp	byte[ebp],0
	jz	end0_6
tag0_6:	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_6
end0_6:	inc	ebp
	add	byte[ebp],8
	cmp	byte[ebp],0
	jz	end0_7
tag0_7:	dec	ebp
	add	byte[ebp],4
	inc	ebp
	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_7
end0_7:	dec	ebp
	inc	byte[ebp]
	call	_put
	cmp	byte[ebp],0
	jz	end0_8
tag0_8:	dec	byte[ebp]
	cmp	byte[ebp],0
	jnz	tag0_8
end0_8:	add	byte[ebp],10
	call	_put
	mov	eax,1		; sys_exit(0)
	mov	ebx,0
	int	80h
