; *********************************************************************
; 	Afonso Castel�o - 90700
; 	Pedro Lu�s      - 90763
; 	Pedro Leit�o    - 90764
; *********************************************************************
; Este programa:
; - Recebe input de um teclado e deteta a linha e coluna.
; - Calcula o valor em hexadecimal da tecla.
; - Quando se carrega de 0 a 9, a calculadora s� reage se a tecla anterior tiver sido C ou D.
; - Quando se carrega no A ou B, a calculadora n�o reage.
; - Quando se carrega no C ou no D, efetua-se a soma ou subtra��o, respetivamente, da pr�xima tecla, caso esta seja de 0 a 9. Caso seja de C a F ignora a primeira
; instru��o e passa a desempenhar a fun�ao desta ultima tecla.
; - Quando se carrega no E subtrai 3 incrementa 3 ao valor e em F subtrai 3.
; - Converte o valor para decimal.
; - Limita o valor de 0 a 99 decimal.
; - Faz display do valor em decimal. 
; **********************************************************************
; * 						  Constantes							   *
; **********************************************************************
DISPLAYS   EQU 0A000H  ; endere�o dos displays de 7 segmentos (perif�rico POUT-1)
TEC_LIN    EQU 0C000H  ; endere�o das linhas do teclado (perif�rico POUT-2)
TEC_COL    EQU 0E000H  ; endere�o das colunas do teclado (perif�rico PIN)
LINHA      EQU 8       ; primeira linha a testar (4� linha, 1000b)
TECLA_INI  EQU 0000H   ; estado inicial da tecla
NUM_INI    EQU 0000H   ; numero inicial no displays

; **********************************************************************
; * 						  C�digo								   *
; **********************************************************************

PLACE      0

inicio:		
; inicializa��es
    MOV    R2,   TEC_LIN    ; endere�o do perif�rico das linhas
    MOV    R3,   TEC_COL    ; endere�o do perif�rico das colunas
    MOV    R4,   DISPLAYS   ; endere�o do perif�rico dos displays
    MOV    R1,   0          ; contador de linha
	MOV    R7,   TECLA_INI  ; guarda estado inicial da tecla em R7
	MOV    R8,   NUM_INI    ; R8 � o valor em hexa do display (come�a a 0)
	MOVB [R4],   R8         ; coloca o display a 0
	
scan:
	MOV    R5,   LINHA      ; come�ar o scan na coluna 4 para facilitar o loop 

espera_tecla:
    MOV    R1,   R5         ; testar a linha corrente 
    MOVB [R2],   R1         ; escrever no perif�rico de sa�da (linhas)
	MOVB   R0,  [R3]        ; ler do perif�rico de entrada (colunas)
    CMP    R0,   0          ; h� tecla premida?
    JNZ    tecla            ; tratar tecla
	SHR    r5,   1          ; avan�ar para a pr�xima linha
	JZ     scan;            ; recome�ar scan da linha inicial 
	JMP    espera_tecla     ; fazer scan da pr�xima linha
	                     
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
	JC     ha_tecla         ; sabe-se a tecla
	ADD    R6,   1          ; incrementa
	JMP    numero_coluna    ; ainda nao se sabe a tecla
	
ha_tecla:
    MOVB   R5,  [R3]        ; l� do perif�rico de entrada (colunas)
	CMP    R5,   0          ; deteta se ainda se est� a carregar na tecla
	JNZ    ha_tecla         ; volta a fazer este loop se ainda nao se largou a tecla
	
calculo:
	MOV    R5,   9
	CMP    R6,   R5         ; para as teclas de AH a FH
	JGT    letras           ; vai para as opera�oes das letras
	JMP    numero           ; vai para os procedimentos para os numeros decimais
	
letras:
	MOV    R5,   000CH
	CMP    R6,   R5	       ; para as teclas A e B
	JLT    scan            ; volta a scanar o teclado
	CMP    R6,   R5	       ; para a tecla C
	JZ     tecla_c_d       ; salta para o procedimento do C ou D
	MOV    R5,   000DH     
	CMP    R6,   R5        ; para a tecla D
	JZ     tecla_c_d	   ; salta para um procedimento do C ou D
	MOV    R5,   000EH
	CMP    R6,   R5	       ; para a tecla E
	JZ     tecla_e	       ; salta para a opera�ao da tecla E
	MOV    R5,   000FH
	CMP    R6,   R5        ; para a tecla F
	JZ     tecla_f		   ; salta para a opera�ao da tecla F          

tecla_c_d:
	MOV    R7,   R6        ; guarda a tecla para o pr�ximo ciclo
	JMP    scan            ; volta a scanar o teclado
	
tecla_e:
	ADD    R8,   3		   ; adiciona 3 ao valor
	MOV    R7,   R6        ; guarda o valor da tecla
	JMP    limites         

tecla_f:
	SUB    R8,   3		   ; subtrai 3 ao valor
	MOV    R7,   R6        ; guarda o valor da tecla
	JMP    limites
	
numero:
	MOV    R5,   0CH
	CMP    R7,   R5        ; deteta se a ultima tecla foi C
	JZ 	   soma
	MOV    R5,   0DH
	CMP    R7,   R5		   ; deteta se a ultima letra foi D
	JNZ    scan 		   ; se nao for C nem D nao reage

subtrai:
	SUB    R8,   R6 	   ; subtrai o valor da tecla
	MOV    R7,   R6  	   ; guarda o valor da tecla
	JMP    limites

soma:
	ADD    R8,   R6        ; soma o valor da tecla
	MOV    R7,   R6        ; guarda o valor da tecla
	JMP    limites
	
limites:
	MOV    R5,   63H       
	CMP    R8,   R5        ; compara o valor ao limite maximo
	JGT    maximo
	CMP    R8,   0         ; compara o valor ao limite minimo
	JLT    minimo
	JMP    decimal

maximo:
	MOV    R8,   63H       ; quando o valor fica maior que 63H for�a a voltar a 63H
	JMP    decimal   
	
minimo:
	MOV    R8,   0         ; quando o valor � menor que 0 for�a-o a 0
	JMP    decimal
	
decimal:
	MOV   R10,   10
	MOV    R5,   R8        
	DIV    R5,   R10       ; obtem o valor das dezenas
	MOV    R9,   R5        
	SHL    R9,   4         ; coloca esse valor no nibble high do R9
	MOV    R5,   R8
	MOD    R5,   R10       ; obtem o valor das unidades
	OR     R9,   R5        ; coloca esse valor no nibble low
	
	MOVB  [R4],  R9        ; escreve o valor em decimal no display
	JMP    scan
	