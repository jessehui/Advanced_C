.global _start 		

_start:
	mov r0,#0x11
	mov r1,#0x32
	add r2,r1,r0

loop:
	b loop	

	.end