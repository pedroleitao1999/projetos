; GLOBL
global	$_assert:function
; TEXT
segment	.text
; ALIGN
align	4
; LABEL
$_assert:
; ENTER
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; DATA
segment	.data
; LABEL
$.L1:
; CHAR
	db	0x61
; CHAR
	db	0x73
; CHAR
	db	0x73
; CHAR
	db	0x65
; CHAR
	db	0x72
; CHAR
	db	0x74
; CHAR
	db	0x20
; CHAR
	db	0x66
; CHAR
	db	0x61
; CHAR
	db	0x69
; CHAR
	db	0x6C
; CHAR
	db	0x65
; CHAR
	db	0x64
; CHAR
	db	0x3A
; CHAR
	db	0x20
; CHAR
	db	0x6C
; CHAR
	db	0x69
; CHAR
	db	0x6E
; CHAR
	db	0x65
; CHAR
	db	0x20
; CHAR
	db	0x00
; TEXT
segment	.text
; ADDR
	push	dword $.L1
; EXTRN
extern	$_prints
; CALL
	call	$_prints
; TRASH
	add	esp, 4
; COMM line 3
; LOCAL
	lea	eax, [ebp+8]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; EXTRN
extern	$_printi
; CALL
	call	$_printi
; TRASH
	add	esp, 4
; COMM line 3
; COMM line 4
; IMM
	push	dword 0
; POP
	pop	eax
; LEAVE
	leave
; RET
	ret
