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
	add	r0,#8
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_0
tag0_0:	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#9
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
	add	r11,#2
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#2
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_1
tag0_1:	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_1
end0_1:	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	sub	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_2
tag0_2:	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end1_0
tag1_0:	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	sub	r11,#2
	ldrb	r0,[r11]
	add	r0,#4
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag1_0
end1_0:	sub	r11,#2
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_2
end0_2:	add	r11,#1
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
	add	r11,#1
	ldrb	r0,[r11]
	sub	r0,#1
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#7
	strb	r0,[r11]
	bl	_put
	sub	r11,#1
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end0_3
tag0_3:	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	beq	end1_1
tag1_1:	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag1_1
end1_1:	add	r11,#1
	ldrb	r0,[r11]
	cmp	r0,#0
	bne	tag0_3
end0_3:	sub	r11,#3
	ldrb	r0,[r11]
	add	r0,#15
	strb	r0,[r11]
	bl	_put
	add	r11,#2
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
	add	r11,#2
	ldrb	r0,[r11]
	add	r0,#1
	strb	r0,[r11]
	bl	_put
	add	r11,#1
	ldrb	r0,[r11]
	add	r0,#4
	strb	r0,[r11]
	bl	_put
	mov	r7,#1		@ sys_exit(0)
	mov	r0,#0
	swi	#0
