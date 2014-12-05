.text
	.globl _start
_put:				@ sys_write(stdout, r11, 1)
	mov	r7,#4		@ sys_write
	mov	r0,#1		@ stdout
	mov	r1,r11
	swi	#0
	bx	lr
_get:				@ sys_read(stdin, r11, 1)
	mov	r7,#3		@ sys_read
	mov	r0,#0		@ stdin
	mov	r1,r11
	swi	#0
	bx	lr
_start:
	mov	r0,#1		@ Set stack size to 8192
	lsl	r0,#13
	sub	sp,r0		@ Create stack
	mov	r11,sp		@ Set ptr to start of stack
	mov	r2,#1		@ r2 is always 1
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#9
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_0
tag0_0:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#8
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_0
end0_0:	sub	r11,#1
	bl	_put
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#7
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_1
tag0_1:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#4
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_1
end0_1:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	add	r0,#7
	strb	r0,[r11]
	bl	_put
	bl	_put
	ldrb	r0,[r11]
	add	r0,#3
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_2
tag0_2:	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_2
end0_2:	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#8
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_3
tag0_3:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#4
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_3
end0_3:	sub	r11,#1
	bl	_put
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#11
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_4
tag0_4:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#5
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_4
end0_4:	sub	r11,#1
	bl	_put
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#8
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_5
tag0_5:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#3
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_5
end0_5:	sub	r11,#1
	bl	_put
	ldrb	r0,[r11]
	add	r0,#3
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	sub	r0,#6
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	sub	r0,#8
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_6
tag0_6:	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_6
end0_6:	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#8
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_7
tag0_7:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#4
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_7
end0_7:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	bl	_put
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_8
tag0_8:	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_8
end0_8:	ldrb	r0,[r11]
	add	r0,#10
	strb	r0,[r11]
	bl	_put
	mov	r7,#1		@ sys_exit(0)
	mov	r0,#0
	swi	#0
