	.thumb
	.global _start

_start:
	  mov r12, r5
	  mov r5, pc
	  add r5, r5, #8
	  ldr r5,[r5]
	  push {r5}
	  mov r5, r12
	  pop {pc}
target:	  .word 0xddccbbaa
	