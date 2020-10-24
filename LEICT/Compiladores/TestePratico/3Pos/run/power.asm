; TEXT
segment	.text
; ALIGN
align	4
; GLOBL
global	$_power:function
; LABEL
$_power:
; ENTER
	push	ebp
	mov	ebp, esp
	sub	esp, 4
; LOCAL
	lea	eax, [ebp+12]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; IMM
	push	dword 0
; JGE
	pop	eax
	pop	ecx
	cmp	ecx, eax
	jge	near $_i1
; IMM
	push	dword 0
; POP
	pop	eax
; LEAVE
	leave
; RET
	ret
; LABEL
$_i1:
; LOCAL
	lea	eax, [ebp+12]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; IMM
	push	dword 0
; JNE
	pop	eax
	pop	ecx
	cmp	ecx, eax
	jne	near $_i2
; IMM
	push	dword 1
; POP
	pop	eax
; LEAVE
	leave
; RET
	ret
; LABEL
$_i2:
; LOCAL
	lea	eax, [ebp+8]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; COPY
	push	dword [esp]
; LOCA
	pop	eax
	mov	[ebp+-4], eax
; TRASH
	add	esp, 4
; LABEL
$_i3:
; LOCAL
	lea	eax, [ebp+12]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; IMM
	push	dword 1
; JLE
	pop	eax
	pop	ecx
	cmp	ecx, eax
	jle	near $_i4
; LOCAL
	lea	eax, [ebp+-4]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; LOCAL
	lea	eax, [ebp+8]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; MUL
	pop	eax
	imul	dword eax, [esp]
	mov	[esp], eax
; COPY
	push	dword [esp]
; LOCA
	pop	eax
	mov	[ebp+-4], eax
; TRASH
	add	esp, 4
; LOCAL
	lea	eax, [ebp+12]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; IMM
	push	dword 1
; SUB
	pop	eax
	sub	dword [esp], eax
; COPY
	push	dword [esp]
; LOCA
	pop	eax
	mov	[ebp+12], eax
; TRASH
	add	esp, 4
; JMP
	jmp	dword $_i3
; LABEL
$_i4:
; LOCAL
	lea	eax, [ebp+-4]
	push	eax
; LOAD
	pop	eax
	push	dword [eax]
; POP
	pop	eax
; LEAVE
	leave
; RET
	ret
; LEAVE
	leave
; RET
	ret
