.global _start 		;定义全局变量

_start:
	mov r0,#0x11
	mov r1,#0x32
	add r2,r1,r0

loop:
	b loop	;死循环

	.end