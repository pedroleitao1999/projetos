;Constantes
pixelscreen EQU 8000H
DISPLAYS   EQU 0A000H  ; endereço dos displays de 7 segmentos (periférico POUT-1)
TEC_LIN    EQU 0C000H  ; endereço das linhas do teclado (periférico POUT-2)
TEC_COL    EQU 0E000H  ; endereço das colunas do teclado (periférico PIN)
LINHA      EQU 8       ; primeira linha a testar (4ª linha, 1000b)

;Pilha
PLACE 1000H
pilha: TABLE 100H

SP_inicial:

PLACE 2000H

; Tabela de vectores de interrupção
tabint:        WORD    rot0
			   WORD    rot1

;dados

;Variaveis
random: WORD 1234h
int_ninja: WORD 0
int_arma:   WORD 0
score: WORD 0

;Objetos no Mundo
mundo:
numero_objetos: WORD 6
	WORD ninja1
	WORD ninja2
	WORD ninja3
	WORD ninja4
	WORD arma1
	WORD arma2
	
ninja1:
	WORD 0 ; x
	WORD 16 ; y
	WORD objeto_ninja
	WORD 1 ; visivel
	
ninja2:
	WORD 4 ; x
	WORD 16 ; y
	WORD objeto_ninja
	WORD 1 ; visivel

ninja3:
	WORD 8 ; x
	WORD 16 ; y
	WORD objeto_ninja
	WORD 1 ; visivel

ninja4:
	WORD 12 ; x
	WORD 16 ; y
	WORD objeto_ninja
	WORD 1 ; visivel

arma1:
	WORD 27 ; x
	WORD 8 ; y
	WORD objeto_prenda
	WORD 1 ; visivel

arma2:
	WORD 27 ; x
	WORD 20 ; y
	WORD objeto_estrela
	WORD 1 ; visivel
	
;------Imagens dos objetos--------------------------	
ninja_img:
	STRING 01000000B
	STRING 11100000B
	STRING 01000000B
	STRING 10100000B
	
estrela_img:
	STRING 10100000B
	STRING 01000000B
	STRING 10100000B
	STRING 00000000B ; para aceder a word
	
prenda_img:
	STRING 01000000B
	STRING 11100000B
	STRING 01000000B
	STRING 00000000B ; para aceder a word
;----------------------------------------------------

objeto_ninja:
	WORD 3   ;largura em pixeis
	WORD 4	 ;altura em pixeis
    WORD ninja_img	 ;pixeis da imagem
	
	
objeto_estrela:
	WORD 3   ;largura em pixeis
	WORD 3	 ;altura em pixeis
    WORD estrela_img	 ;pixeis da imagem
	
objeto_prenda:
	WORD 3   ;largura em pixeis
	WORD 3	 ;altura em pixeis
    WORD prenda_img	 ;pixeis da imagem


	
;codigo	
inicio:
PLACE 0
MOV SP, SP_inicial	; incializa o stack pointer

JMP cod_PP

;* -- Rotina de Serviço de Interrupção 0 -------------------------------------------
;* 
;* Descrição: Trata interrupções clock ligado à INT0. 
;*
rot0:
	PUSH R0
	PUSH R1
	MOV R0,int_arma
	MOV R1,[R0]
	INC R1
	MOV [R0],R1
	POP R1
	POP R0
	RFE

;*-----------------------------------------------------------------------------------

;* -- Rotina de Serviço de Interrupção 1 -------------------------------------------
;* 
;* Descrição: Trata interrupções clock ligado à INT1. 
;*		
rot1:
	PUSH R0
	PUSH R1
	MOV R0,int_ninja
	MOV R1,[R0]
	INC R1
	MOV [R0],R1
	POP R1
	POP R0
	RFE
;*-------------------------------------------------------------------------------

;* -- Rotina de limpeza do ecrã -------------------------------------------
;* 
;* Descrição: Trata interrupções clock ligado à INT0. 
;*
CLEAR_SCREEN:	 ;limpa o ecrã
	PUSH R0
	PUSH R1
	PUSH R2
	MOV R2,0	 ;valor a meter no ecrã
	MOV R0,pixelscreen	 ;ecrã em R0
	MOV R1,128	 ;numero de bytes
loop_cs:
	MOVB [R0],R2	; mete um byte a 0
	DEC R1	 ; menos uma word a apagar
	JZ FIM_CS
	INC R0	;proxima word
	JMP loop_cs; repete para o próximo byte
FIM_CS:
	POP R2
	POP R1
	POP R0
	RET
;*-----------------------------------------------------------------------

;* -- Rotina de SHR-------
;* 
;* Descrição: 	Faz o SHR do objeto
;*
;* Parâmetros:  R7 - linha da imagem do objeto, R1 - numero de bits a shiftar
SHIFTR_N:
	PUSH R1
	OR  R1,R1
AGAIN_SHRN:
	JZ	DONE_SHRN	 ;quando o x é 0 não faz shift
	SHR R7,1	 ;shift 1 vez para a direira
	DEC	R1	 ;menos uma vez a shiftar
	JNZ AGAIN_SHRN	 ;shifta a proxima vez
DONE_SHRN:	 ;já shiftou todas as vezes a shiftar
	POP R1
	RET
;*------------------------------------------------	

;* -- Rotina de SHL--------------
;* 
;* Descrição: 	Faz o SHL do objeto
;*
;* Parâmetros:  R7 - linha da imagem do objeto, R8 - numero de bits a shiftar	
SHIFTL_N:	 ;trata do objeto quando este ocupa mais de um byte
	PUSH R8
	OR  R8,R8
AGAIN_SHLN:
	JZ	DONE_SHLN	 ; se o objeto nao ocupa 2 bytes nao da shift para a esquerda 
	SHL R7,1	 ;s hift 1 vez para a esquerda
	DEC	R8	 ; menos uma vez a shiftar
	JNZ AGAIN_SHLN	 ; shifta a próxima vez 
DONE_SHLN:	 ; ja shiftou todas as vezes
	POP R8
	RET	
;*--------------------------------------------------
	

;* -- Rotina de colisao--------------
;* 
;* Descrição: 	Gere as colisoes quando é o ninja a colidir
;*
;* Parâmetros:  R6 - Objeto
colisao_ninja_arma:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	
	MOV R1,[R6+4]
	MOV R2,objeto_prenda
	CMP R1,R2
	JNZ cna_colidiu_com_estrela
cna_colidiu_com_prenda:
	MOV R0,0
	PUSH R4
	MOV R4,R6
    CALL draw_var_objeto
	MOV R2,29
	MOV [R4],R2
	; aleatorio prenda/estrela
	POP R4
	MOV R0,2 ; desenhar sem testar colisao
	CALL draw_var_objeto
	; ADD SCORE
	CALL add_score
	JMP cna_fim
cna_colidiu_com_estrela:	
	MOV R0,0 ; desenhar sem testar colisao
	CALL draw_var_objeto
	XOR R1,R1
	MOV [R4+6],R1
	MOV R4,R6
	MOV R0,2
	CALL draw_var_objeto
	;TESTAR todos os ninjas mortos
cna_fim:
	POP  R4
	POP  R3
	POP  R2
	POP  R1
	POP  R0
	RET


;* -- Rotina de score--------------
;* 
;* Descrição: 	Adiciona ao score
;*
add_score:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	MOV R0, score
	MOV R1, [R0]
	MOV R2, 63H
	CMP R1, R2
	JGE fim_adiciona_score
	ADD R1, 3
	MOV [R0], R1
	decimal:
	MOV    R1,   10
	MOV    R2,   [R0]      
	DIV    R2,   R1      ; obtem o valor das dezenas
	MOV    R3, R2        
	SHL    R3,   4         ; coloca esse valor no nibble high do R9
	MOV    R2,   [R0]
	MOD    R2,   R1      ; obtem o valor das unidades
	OR     R3,   R2        ; coloca esse valor no nibble low
	
	MOV   R1, DISPLAYS
	MOVB  [R1], R3
fim_adiciona_score:	
	POP R3
	POP R2
	POP R1
	POP R0
	RET
	
	
;* -- Rotina de colisao--------------
;* 
;* Descrição: gere as colisoes
;* Parametros: R4 - Objeto
;*
colisao:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	
	MOV R3,[R4+4]
	MOV R1,pixelscreen
	SUB R2,R1
	MOV R5,R2
	SHR R2,2 ; linha da colisao

	MOV R1,objeto_ninja
	CMP R3,R1
	JNZ teste_colisao_arma
	
	MOV R6,arma1
	MOV R7,[R6+2]
	SHR R2,4
	SHR R7,4
	CMP R2,R7
	JNZ colisao_com_arma2
	
	CALL colisao_ninja_arma
	JMP fim_de_colisao
colisao_com_arma2:
	MOV R6,arma2
	CALL colisao_ninja_arma
	JMP fim_de_colisao
	

teste_colisao_arma:
; descobrir qual o ninja
; chamar colisao_ninja_arma
	JMP fim_de_colisao
fim_de_colisao:
	POP R8
    POP R7
    POP R6
    POP R5
	POP R4
	POP R2
	POP R1
	POP R0
	RET
;R0 - 0 - ERASE 1 - DRAW
;R4 - VAR objeto	
draw_var_objeto:
	PUSH R3
	PUSH R2
	PUSH R1
	PUSH R0
	
	MOV R1, [R4]
	MOV R2, [R4+2]
	MOV R3, [R4+4]
	CALL draw_objeto
	
	POP R0
	POP R1
	POP R2
	POP R3
	RET
	
;* -- Rotina de desenho -----------------------------------------------------------------
;* 
;* Descrição: Desenha ou apaga objetos 
;*
;* Parâmetros:  R0 - Desenha ou apaga, R1 - coluna de referencia, R2 - linha de referencia, R4 - deteta colisoes ou nao
;*----------------------------------------------------------------------------------------------------------------------
draw_objeto:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	PUSH R7
	PUSH R8
	PUSH R9
	PUSH R10
	MOV R10,pixelscreen	 ; pixelscreen em R10
	SHL R2,2	 ; multiplica linha por 4
	ADD R10,R2	 ; adiciona ao endereço inicial do pixelscreen a linha*4   
	MOV R2,R1	 ; guarda em R2 a coluna
	SHR R2,3	 ; divide a coluna por 8
	ADD R10,R2   ; endereço do byte da 1 linha
;	MOV R9, 7
;	AND R1, R9	 ; para quando o objeto vai estar no segundo byte da linha
	SHL R1,13
	SHR R1,13	 ; offset do pixel
	
	PUSH R4
	MOV R4,[R3]   ; numero de colunas
	MOV R5,[R3+2] ; numero de linhas
	MOV R6,[R3+4] ; imagem (pixeis)
	POP R3 ; R3 contém variavel do objeto
	
	MOV R2,0
	
	MOV R9,R1	 ; mete em R9 o offset do pixel
	ADD R9,R4	 ; soma o numero de colunas
	DEC R9
	SHR R9,3 	; Se R9 <> 0 entao atravessa dois bytes
	
LOOP_LINHA:
	MOVB R7,[R6]	 ; mete em R7 uma linha do objeto
	
	CALL SHIFTR_N	; mete o bit de referencia no offset
	
	MOVB R8,[R10]	 ; mete em R8 o conteudo do byte do ecrã
	OR R0,R0	 ; quando o R0 é 0 vai apagar o Objeto
	JNZ DRAW_1	 ;quando nao é 0 vai desenhar

	NOT R7	 ; nega a linha do objeto
	AND R8,R7	 ; apaga a linha que vai aparecer no ecrã
	JMP DONE_D1	 
DRAW_1:
	MOV R4,R8
	XOR R4,R7
	
	OR  R8,R7
	
	OR R2,R2 ; se ja colidiu, nao vale a pena testar de novo
	JNZ DONE_D1

	BIT R0,0
	JZ DONE_D1
	CMP R8,R4
	JZ DONE_D1 ; nao hive colisao
	MOV R2,R10
DONE_D1:
	MOVB [R10],R8	 ; mete no byte do ecrã a linha
	
	OR R9,R9	 ; se nao atravessa dois bytes acabou de desenhar a linha
	JZ FIM_LINHA
	
	INC R10	 ; próximo byte
	MOVB R7,[R6]	 ; linha do objeto em R7	 
	MOV R8,8
	SUB R8,R1	 ; numero de bits a shiftar

	CALL SHIFTL_N
	
	MOVB R8,[R10]	 ; mete em R8 os bits do ecrã da linha a alterar

	OR R0,R0	 ; R0 = 0: vai apagar
	JNZ DRAW_2	; R0 = 1: vai desenhar
	NOT R7	 ; nega a linha
	AND R8,R7	 ; apaga a linha
	JMP DONE_D2
	
DRAW_2:
	MOV R4,R8
	XOR R4,R7
	
	OR  R8,R7
	
	OR R2,R2 ; se ja colidiu, nao vale a pena testar de novo
	JNZ DONE_D2

	BIT R0,0
	JZ DONE_D2
	CMP R8,R4
	JZ DONE_D2 ; nao houve colisao
	MOV R2,R10
	
DONE_D2:
	MOVB [R10],R8	 ; mete a linha no ecra
	
	DEC R10	 ; volta ao byte inicial
	
FIM_LINHA:
	DEC R5	 ;  menos uma linha a fazer
	JZ FIM_DRAW	 ; se ja estao todas sai da rotina
	INC R6	 ; proxima linha da imagem
	ADD R10,4 	; endereço da proxima linha do ecrã
	
	JMP LOOP_LINHA	 ; repete para a proxima linha

FIM_DRAW:

	OR  R2,R2
	JZ  sem_colisao
	MOV R4,R3
	CALL colisao
	
sem_colisao:
	POP R10
	POP R9
	POP R8
	POP R7
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET
;*-------------------------------------------------------------------------
	
	
	; R4 APONTA PARA NINJA
MOVER_NINJA_CIMA:
    PUSH   R0
	PUSH   R1
	PUSH   R2
	PUSH   R3
	PUSH   R4
	PUSH   R5
	
	MOV	   R0, [R4+6]
	OR	   R0, R0
	JZ	   DONE_MN	 ; se o ninja já desapareceu nao acontece nada
	MOV	   R2,[R4+2]   ; R2 contem a linha
	OR	   R2,R2
	JZ     DONE_MN	 ; se a linha é 0 nao sobe mais
	MOV    R1,[R4]	 ; R1 = quantidade de colunas
	MOV    R3,[R4+4]	 ; R3 = imagem do objeto
    MOV    R0,0
	CALL   draw_objeto	 ; apaga o objeto
	DEC    R2
	MOV    [R4+2],R2	 ; sobe de linha
	INC    R0	 
	CALL   draw_objeto	 ; desenha objeto

DONE_MN:
	POP    R5
	POP    R4
	POP    R3
	POP    R2
	POP    R1
	POP    R0
	RET
	
MOVER_NINJA_BAIXO:
    PUSH   R0
	PUSH   R1
	PUSH   R2
	PUSH   R3
	PUSH   R4
	PUSH   R5
	
	MOV	   R0,[R4+6]
	OR	   R0, R0
	JZ	   DONE_MD
	MOV	   R2,[R4+2]   ; R2 contem a linha
	MOV    R1,[R4]	 ; R1 = contem a coluna
	MOV    R3,[R4+4]	 ; R3 = imagem do objeto
    MOV    R0,0
	CALL   draw_objeto	 ; apaga o objeto
	MOV    R5,	27
	CMP    R2, R5	 
	JGT	   mata_ninja	 ; se o ninja tentar descer mais do que o chao morre
	INC    R2
	MOV    [R4+2],R2	 ; desce de linha
	INC    R0	 
	CALL   draw_objeto	 ; desenha objeto
	JMP DONE_MD

mata_ninja:
	CALL morte_ninja

DONE_MD:
	POP    R5
	POP    R4
	POP    R3
	POP    R2
	POP    R1
	POP    R0
	RET
	

morte_ninja:
	PUSH R0	 
	MOV R0, 0
	CALL draw_objeto	 ; apaga o ninja
	MOV [R4 + 6], R0	 ; faz o ninja desaparecer
	POP R0
	RET

ACCAO_TECLA:          ; tecla em R6
    PUSH   R1
	PUSH   R2
	PUSH   R3
	PUSH   R4
	PUSH   R5
	PUSH   R6
	PUSH   R7
	
	MOV    R5,   0
	CMP    R6,   R5	 ; testa se a tecla é 0
	JNZ    TESTA1	 ; quando nao é 0 vai para o proximo teste      
	MOV    R4,ninja1	 
	CALL   MOVER_NINJA_CIMA	 ; sobe o ninja1

	
TESTA1:	
	MOV    R5,   1	 
	CMP    R6,   R5	 ; testa se a tecla é 1
	JNZ    TESTA2           
	MOV    R4,ninja2
	CALL   MOVER_NINJA_CIMA	 ; sobe o ninja2
	
TESTA2:	
	MOV    R5,   2
	CMP    R6,   R5	 ; testa se a tecla é 2        
	JNZ    TESTA3           
	MOV    R4,ninja3
	CALL   MOVER_NINJA_CIMA	 ; sobe o ninja3
	
TESTA3:	
	MOV    R5,   3	 
	CMP    R6,   R5	 ; testa se a tecla é 3         
	JNZ    TESTA4           
	MOV    R4,ninja4
	CALL   MOVER_NINJA_CIMA	 ; sobe o ninja4
	
TESTA4:	
	MOV    R5,   4	
	CMP    R6,   R5	 ; testa se a tecla é 4
	JNZ    TESTA5           
	MOV    R4,ninja1
	CALL   MOVER_NINJA_BAIXO  ; desce o ninja1
	
TESTA5:	
	MOV    R5,   5	
	CMP    R6,   R5  ; testa se a tecla é 5
	JNZ    TESTA6           
	MOV    R4,ninja2
	CALL   MOVER_NINJA_BAIXO  ; desce o ninja2
	
TESTA6:	
	MOV    R5,   6
	CMP    R6,   R5	 ; testa se a tecla é 6         
	JNZ    TESTA7           
	MOV    R4,ninja3
	CALL   MOVER_NINJA_BAIXO  ; desce o ninja3
	
TESTA7:	
	MOV    R5,   7
	CMP    R6,   R5	 ; testa se a tecla é 7         
	JNZ    TESTA8
	MOV    R4,ninja4
	CALL   MOVER_NINJA_BAIXO  ; desce o ninja4
	
TESTA8:
	JMP DONE_AT

	
DONE_AT:
    POP    R7
    POP    R6
	POP    R5
	POP    R4
	POP    R3
    POP    R2
	POP    R1
	RET
	
;* -- Rotina que testa as flags das interrupções -------------------------------------------
;* 
;* Descrição: Deteta se as flags das interrupções estão ativas e quando estão faz a açao respetiva
;*
;*	
testa_interrupts:
	PUSH R0
	PUSH R1
	PUSH R2
	PUSH R3
	PUSH R4
	PUSH R5
	PUSH R6
	
	MOV R6,random  ; INCREMENTAR RANDOM
	MOV R5,[R6]
	INC R5
	MOV [R6],R5
	
	MOV    R6,int_arma
	MOV    R5,[R6]
	OR     R5,R5
	JZ     testa_move_ninja
	XOR    R5,R5
	MOV    [R6],R5

	MOV    R6,mundo
	MOV    R5,[R6]  ; NUMERO DE OBJETOS
move_prox_arma:
	ADD	   R6,2     ; R3 APONTA PRIMEIRa variavel de estado

	MOV    R4,[R6]  ; TEM variavel de estado
	MOV    R3,[R4+4] ; tem endereço do objeto
	MOV    R1,objeto_ninja
	CMP	   R3,R1
	JZ     nao_arma
	
	MOV R1, [R4]
	MOV R2, [R4+2]
	MOV R3, [R4+4]
	MOV R0,0
	CALL draw_objeto	
	
	SUB    R1,1
	MOV    [R4],R1
	JNC    desenha_arma
	CALL arma_aleatoria
	MOV    R1,29    ; colocar a arma na ultima coluna menos a largura da arma
	MOV    [R4],R1
	; criar nova arma1 (aleatoria prenda ou estrela)
	; aleatoria posicao
	
desenha_arma:
	MOV R0,1 
	CALL draw_objeto
	JMP nao_arma

arma_aleatoria:
	PUSH R0
	PUSH R2
	PUSH R4
	MOV R0, [R4+4]
	MOV R1, random
	MOV R1,[R1]
	MOV R2,3
	AND R1,R2
	JNZ  e_estrela
	MOV R1, objeto_prenda
	MOV [R4+4], R1
	JMP fim_arma_aleatoria
e_estrela:
	MOV R1, objeto_estrela
	MOV [R4+4], R1
fim_arma_aleatoria:
	MOV R1, 0
	POP R4
	POP R2
	POP R0
	RET

nao_arma:
	DEC    R5
	JNZ    move_prox_arma

testa_move_ninja:	
	MOV    R6,int_ninja
	MOV    R5,[R6]
	OR     R5,R5
	JZ     no_interrupts
	XOR    R5,R5
	MOV    [R6],R5

	MOV    R6,mundo
	MOV    R5,[R6]  ; NUMERO DE OBJETOS
move_prox_ninja:
	ADD	   R6,2     ; R3 APONTA PRIMEIRa variavel de estado

	MOV    R4,[R6]  ; TEM variavel de estado
	MOV    R3,[R4+4] ; tem endereço do objeto
	MOV    R1,objeto_ninja
	CMP	   R3,R1
	JNZ    nao_ninja
	
	CALL   MOVER_NINJA_BAIXO  ; desce o ninja1

nao_ninja:
	DEC    R5
	JNZ    move_prox_ninja
no_interrupts:
	POP R6
	POP R5
	POP R4
	POP R3
	POP R2
	POP R1
	POP R0
	RET
;*---------------------------------------------------------------------------

cod_PP:

CALL CLEAR_SCREEN	;chama a rotina que limpa o ecra

; Inicializações
MOV R0,score	; começa o score a 0
XOR R1,R1
MOV [R0],R1

;Inicializa o ninja1
MOV R4,ninja1
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto

;Inicializa o ninja2
MOV R4,ninja2
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto

;Inicializa o ninja3
MOV R4,ninja3
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto

;Inicializa o ninja4
MOV R4,ninja4
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto

;Inicializa as armas
MOV R4, arma1
MOV R1, random
MOV R1,[R1]
MOV R2,3
AND R1,R2
JNZ  e_estrela1	 ;se de 0 a 3, o numero nao for 0 entao aparece estrela
MOV R1, objeto_prenda
MOV [R4+4], R1	 ;coloca o objeto_prenda na arma1
JMP f_arma2	 ;proxima arma

e_estrela1:
MOV R1, objeto_estrela
MOV [R4+4], R1	;coloca o objeto_estrela na arma1

f_arma2:	 ;arma na segunda metade do ecra
MOV R4, arma2
MOV R1, random
MOV R1,[R1]
MOV R2,3
AND R1,R2
JNZ  e_estrela2	 ;se de 0 a 3, o numero nao for 0 entao aparece estrela
MOV R1, objeto_prenda
MOV [R4+4], R1	 ;coloca o objeto_prenda na arma2
JMP desenha_armas

e_estrela2:
MOV R1, objeto_estrela
MOV [R4+4], R1	 ;coloca o objeto_prenda na arma2

desenha_armas:	 
MOV R4,arma1
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto	 ;desenha a arma1

MOV R4,arma2
MOV R1, [R4]
MOV R2, [R4+2]
MOV R3, [R4+4]
MOV R0,1
CALL draw_objeto	 ;desenha a arma2

MOV R5, 0
MOV RCN, R5
MOV BTE, tabint           ; incializa BTE
EI0                     ; permite interrupções 0
EI1
EI 
	
MOV    R2,   TEC_LIN    ; endereço do periférico das linhas
MOV    R3,   TEC_COL    ; endereço do periférico das colunas
MOV    R4,   DISPLAYS   ; endereço do periférico dos displays

;* -- main_loop -----------------------------------------------------------------
;* 
;* Descrição: Faz o varrimento do teclado continuamente 
;*
main_loop:
	MOV    R5,   LINHA      ; começar o scan na coluna 4 para facilitar o loop 

espera_tecla:
	CALL testa_interrupts
	
testar_teclado:	
    MOV    R1, R5           ; testar a linha corrente 
    MOVB   [R2], R1         ; escrever no periférico de saída (linhas)
	MOVB   R0,  [R3]        ; ler do periférico de entrada (colunas)
	MOV    R6,0FH
	AND    R0,R6
    CMP    R0,   0          ; há tecla premida?
    JNZ    tecla            ; tratar tecla
	SHR    R5,   1          ; avançar para a próxima linha
	JZ     main_loop;       ; recomeçar scan da linha inicial 
	JMP    espera_tecla     ; fazer scan da próxima linha
	                                                 ; linha em R5, coluna em R0
tecla:	                    
	MOV    R6,   0          ; contador de linha

numero_linha:	
	SHR    R5,   1          ; move bit de menor peso para carry
	JC     linha_por_4      ; se carry temos a linha
	ADD    R6,   1
	JMP    numero_linha     ; continuar a contar

linha_por_4:	
	SHL    R6,   2          ; multiplica o numero da linha por 4

numero_coluna:
	SHR    R0,   1          ; move bit de menor peso para carry
	JC     processar_tecla         ; sabe-se a tecla
	ADD    R6,   1          ; incrementa
	JMP    numero_coluna    ; ainda nao se sabe a tecla
	

processar_tecla:
    CALL   ACCAO_TECLA	
ha_tecla:
	CALL testa_interrupts

    MOVB   R5,  [R3]        ; lê do periférico de entrada (colunas)
	MOV	   R6, 0FH
	AND    R5, R6
	CMP    R5,   0          ; deteta se ainda se está a carregar na tecla
	JNZ    ha_tecla         ; volta a fazer este loop se ainda nao se largou a tecla
	JMP    main_loop
	
	


	
	