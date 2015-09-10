	.globl _start
_start:
	.thumb
	mov	ip, r5
	adr	r5, jump_va
	ldr	r5, [r5]
	push	{r5}
	mov	r5, ip
	pop	{pc}

jump_va:.word  0xaabbccdd
