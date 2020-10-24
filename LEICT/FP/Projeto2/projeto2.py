#ist190764 Pedro Leitao

#from parte1 import e_palavra
from itertools import permutations
letras=['A','B','C','D','E','F','G','H','I','J','L','M','N','O','P','Q','R','S','T','U','V','X','Z']

def cria_palavra_potencial(string,tup1):
    """Constroi o TAD palavra_potencial."""
    for a in string:
        if a not in letras: #Para cada a contido na variavel string, caso a nao pertenca ao conjunto letras.
            raise ValueError('cria_palavra_potencial:argumentos invalidos.') #Nao e possivel construir o TAD palavra_potencial se a palavra contiver numeros ou letras minusculas.
    for a in tup1:
        if a not in letras: #Para cada a contido na variavel tup1, caso a nao pertenca ao conjunto letras.
            raise ValueError('cria_palavra_potencial:argumentos invalidos.') #Nao e possivel construir o TAD palavra_potencial se o conjunto de letras contiver numeros ou letras minusculas.
    if len(string)>len(tup1): #Caso a variavel string possua um comprimento maior que a variavel tup1.
        raise ValueError('cria_palavra_potencial:a palavra nao e valida.') #Nao e possivel construir o TAD palavra_potencial se a palavra tiver mais letras que o conjunto.
    elif type(string)!=str or type(tup1)!=tuple: #Caso a variavel string nao seja uma string ou a variavel tup1 nao seja um tuplo.
        raise ValueError('cria_palavra_potencial:argumentos invalidos.') #Nao e possivel construir o TAD palavra_potencial se a palavra nao for uma string e o conjunto nao for um tuplo.
    b = list(tup1)
    for a in string:
        if a in b:
            b.remove(a) #Para cada a contido na variavel string, esse mesmo a e removido da lista que possui os elementos presentes na variavel tup1.
        else: #Caso ja nao haja nenhum a para retirar da lista, mas a string ainda possua elementos.
            raise ValueError('cria_palavra_potencial:a palavra nao e valida.') #Caso apareca mais vezes a mesma letra na palavra do que no conjunto.
    return { 'palavra' : string, 'conjunto' : tup1 } #Assim que os dois argumentos forem validos e a palavra for valida em relacao ao conjunto e criado o TAD palavra_potencial.

def palavra_tamanho(p):
    """Calcula o tamanho da palavra introduzida."""
    return len(p['palavra'])

def e_palavra_potencial(argp):
    """Verifica se o argumento introduzido possui as caracteristicas de uma palavra_potencial."""
    if type(argp)!=dict:
        return False #O argumento nao possui as caracteristicas de uma palavra_potencial se nao for um dicionario.
    elif len(argp)!=2:
        return False #Sendo ele um dicionario, o argumento so pode ser considerado palavra_potencial se e so se possuir duas keys.
    elif 'palavra' not in argp or 'conjunto' not in argp:
        return False #Sendo o argumento um dicionario, so e valido se e so se as suas keys se chamarem palavra e conjunto.
    elif type(argp['palavra'])!=str:
        return False #Caso a palavra do argumento nao seja uma string, entao o argumento nao e palavra_potencial.
    elif type(argp['conjunto'])!=tuple:
        return False #Caso o conjunto do argumento nao seja um tuplo, entao o argumento nao e palavra_potencial.
    for a in argp['palavra']:
        if a not in letras:
            return False #As letras da palavra do argumento so podem ser maiusculas, nao podendo conter numeros, para ele ser valido.
    for a in argp['conjunto']:
        if a not in letras:
            return False #As letras do conjunto do argumento so podem ser maiusculas, nao podendo conter numeros, para ele ser valido.
    b = list(argp['conjunto'])
    for a in argp['palavra']:
        if a in b:
            b.remove(a) #Para cada a contido na palavra do argumento, esse mesmo a e removido da lista que possui os elementos presentes no conjunto do argumento.
        else: #Caso ja nao haja nenhum a para retirar da lista, mas a palavra do argumento ainda possua elementos.
            return False #Caso apareca mais vezes a mesma letra na palavra do argumento do que no seu conjunto, o mesmo nao pode ser uma palavra_potencial.
    return True #Caso o argumento nao verifique nenhum dos casos, entao ele possui caracteristicas de uma palavra_potencial.

def palavras_potenciais_iguais(p1,p2):
    """Verifica a igualdade de duas palavras_potenciais."""
    if type(p1)!=dict:
        return False #Uma palavra_potencial tem de ser um dicionario.
    for a in p1['palavra']:
        if a not in letras:
            return False #A palavra de uma palavra_potencial so pode conter letras maiusculas, nao podendo conter numeros.
    for a in p1['conjunto']:
        if a not in letras:
            return False #O conjunto de letras de uma palavra_potencial so pode conter letras maiusculas, nao podendo conter numeros.
    if type(p1['palavra'])!=str:
        return False #A palavra de uma palavra_potencial so pode ser uma string.
    elif type(p1['conjunto'])!=tuple:
        return False #O conjunto de letras de uma palavra_potencial so pode ser um tuplo.
    if type(p2)!=dict:
        return False #Todas as condicoes verificadas no primeiro argumento tem de ser verificadas no segundo.
    for a in p2['palavra']:
        if a not in letras:
            return False
    for a in p2['conjunto']:
        if a not in letras:
            return False
    if type(p2['palavra'])!=str:
        return False
    elif type(p2['conjunto'])!=tuple:
        return False
    elif p1['palavra']==p2['palavra'] and p1['conjunto']==p2['conjunto']:
        return True #Se as palavras das respetivas palavras_potenciais forem iguais e os conjuntos tambem o forem, entao os dois argumentos do tipo palavra_potencial sao iguais.
    else:
        return False

def palavra_potencial_menor(p1,p2):
    """Verifica se uma determinada palavra_potencial e inferior alfabeticamente a palavra_potencial seguinte."""
    if type(p1)!=dict:
        return False #Sao verificadas as mesmas condicoes que foram na funcao palavras_potenciais_iguais.
    for a in p1['palavra']:
        if a not in letras:
            return False
    for a in p1['conjunto']:
        if a not in letras:
            return False
    if type(p1['palavra'])!=str:
        return False
    elif type(p1['conjunto'])!=tuple:
        return False
    if type(p2)!=dict:
        return False
    for a in p2['palavra']:
        if a not in letras:
            return False
    for a in p2['conjunto']:
        if a not in letras:
            return False
    if type(p2['palavra'])!=str:
        return False
    elif type(p2['conjunto'])!=tuple:
        return False
    elif p1['palavra']<p2['palavra']:
        return True #Se a palavra do primeiro argumento for alfabeticamente anterior a do segundo argumento, entao a condicao e verdadeira.
    else:
        return False

def palavra_potencial_para_cadeia(p):
    """Representa a palavra contida na palavra_potencial introduzida."""
    return p['palavra']

def cria_conjunto_palavras():
    """Cria um TAD conjunto_palavras vazio."""
    return []

def numero_palavras(c):
    """Calcula o numero de palavras presentes num conjunto_palavras."""
    return len(c)

def subconjunto_por_tamanho(c,n):
    """Representa um conjunto_palavras apenas com as palavras com o numero de letras escolhido."""
    d=cria_conjunto_palavras()
    for a in c:
        if palavra_tamanho(a)==n:
            acrescenta_palavra(d,a) #Para cada palavra no TAD conjunto_palavras, se a palavra possuir n letras, entao e acrescentada ao novo conjunto.
    return d

def acrescenta_palavra(c,p):
    """Acrescenta uma palavra_potencial a um conjunto_palavras."""
    if not e_conjunto_palavras(c) or not e_palavra_potencial(p):
        raise ValueError('acrescenta_palavra:argumentos invalidos.') #Tanto o conjunto_palavras como a palavra_potencial tem de ser argumentos validos.
    if not (p in c):
        c+=[p] #Caso a palavra_potencial ainda nao esteja no conjunto_palavras, ela e acrescentada ao mesmo.

def e_conjunto_palavras(argc):
    """Verifica se o argumento introduzido possui as caracteristicas de um conjunto_palavras."""
    if type(argc)!=list:
        return False #O argumento tem de ser uma lista, caso contrario nao possui as caracteristicas de um conjunto_palavras.
    for a in argc:
        if not e_palavra_potencial(a):
            return False #Para cada a dentro no argumento, se a nao possuir caracteristicas de uma palavra_potencial, entao o argumento nao possui caracteristicas de um conjunto_palavras,
    return True #Se nenhuma das condicoes se verificar, entao o argumento pode ser considerado um conjunto_palavras.

def conjunto_palavras_para_cadeia(c):
    """Representa por ordem do menor para o maior numero de letras da palavra_potencial e consequentemente por ordem alfabetica as palavras_potencial do conjunto_palavras."""
    a=0 #Inicialmente, o valor de a e zero.
    while a<(numero_palavras(c)-1):
        if palavra_tamanho(c[a])<=palavra_tamanho(c[a+1]):
            if palavra_potencial_menor(c[a],c[a+1]):
                a+=1
            else: #Ordena as palavras alfabeticamente.
                c[a],c[a+1]=c[a+1],c[a]
                a=0
        else: #Ordena as palavras por numero de letras.
            c[a],c[a+1]=c[a+1],c[a]
            a=0
    return c #E devolvido o conjunto com as palavras ordenadas da forma correta.

def cria_jogador(nome):
    """Cria um novo TAD jogador."""
    if type(nome)!=str:
        raise ValueError('cria_jogador:argumento invalido.') #O nome tem de ser uma string, caso contrario nao e valido.
    return { 'nome' : nome, 'pontuacao' : 0, 'palavras_validas' : [], 'palavras_invalidas' : [] } #Se a condicao anterior nao se verificar entao o argumento e valido e e criado o respetivo TAD jogador.

def jogador_nome(j):
    """Apresenta o nome do jogador."""
    return j['nome']

def jogador_pontuacao(j):
    """Apresenta a pontuacao do jogador num determindado momento."""
    return j['pontuacao']

def jogador_palavras_validas(j):
    """Apresenta todas as palavras validas que o jogador tem ate ao momento."""
    return j['palavras_validas']

def jogador_palavras_invalidas(j):
    """Apresenta todas as palavras invalidas que o jogador tem ate ao momento."""
    return j['palavras_invalidas']

def adiciona_palavra_valida(j,p):
    """Adiciona uma nova palavra valida ao jogador recebendo tambem a respetiva pontuacao."""
    if not e_jogador(j) or not e_palavra_potencial(p):
        raise ValueError('adiciona_palavra_valida:argumentos invalidos.') #Os argumentos tem de ser validos para a funcao ser possivel.
    j['palavras_validas']=j['palavras_validas']+(p['palavra'],) #Adiciona a palavra a key palavras_validas.
    j['pontuacao']=j['pontuacao']+len(p['palavra']) #Adiciona a pontuacao obtida a key pontuacao.

def adiciona_palavra_invalida(j,p):
    """Adiciona uma nova palavra invalida ao jogador sendo ele penalizado pela respetiva pontuacao."""
    if not e_jogador(j) or not e_palavra_potencial(p):
        raise ValueError('adiciona_palavra_invalida:argumentos invalidos.') #Os argumentos tem de ser validos para a funcao ser possivel.
    j['palavras_invalidas']=j['palavras_invalidas']+(p['palavra'],) #Adiciona a palavra a key palavras_invalidas.
    j['pontuacao']=j['pontuacao']-len(p['palavra']) #Reduz a pontuacao respetiva na key pontuacao.

def e_jogador(argj):
    """Verifica se o argumento introduzido possui as caracteristicas de jogador."""
    if type(argj)!=dict:
        return False #O argumento tem de ser um dicionario para possuir as caracteristicas da TAD jogador.
    elif len(argj)!=4:
        return False #Sendo um dicionario, o argumento tem de ter 4 keys para ser considerado jogador.
    elif 'nome' not in argj or 'pontuacao' not in argj or 'palavras_validas' not in argj or 'palavras_invalidas' not in argj:
        return False #Sendo um dicionario, as keys do argumento tem de ser nome, pontuacao, palavras_validas e palavras_invalidas, caso contrario nao e considerado jogador.
    elif type(argj['nome'])!=str:
        return False #A key nome tem de ser uma string para o argumento ser valido como jogador.
    elif type(argj['pontuacao'])!=int:
        return False #A key pontuacao tem de ser um numero inteiro para o argumento ser valido como jogador.
    elif type(argj['palavras_validas'])!=list:
        return False #A key palavras_validas tem de ser uma lista para o argumento ser valido como jogador.
    elif type(argj['palavras_invalidas'])!=list:
        return False #A key palavras_invalidas tem de ser uma lista para o argumento ser valido como jogador.
    for a in argj['palavras_validas']:
        if not e_palavra_potencial(a):
            return False #A key palavras_validas tem de possuir as caracteristicas de uma palavra_potencial para o argumento ser valido.
    for a in argj['palavras_invalidas']:
        if not e_palavra_potencial(a):
            return False #A key palavras_invalidas tem de possuir as caracteristicas de uma palavra_potencial para o argumento ser valido.
    return True #Caso o argumento nao verifique nenhum dos casos, entao ele possui caracteristicas de um jogador.

def jogador_para_cadeia(j):
    """Apresenta os dados de um jogador num determinado momento."""
    print('JOGADOR',j['nome'],'PONTOS=',j['pontuacao'],'VALIDAS',j['palavras_validas'],'INVALIDAS',j['palavras_invalidas'])

def gera_todas_palavras_validas(tup1):
    """Apresenta todas as palavras validas de acordo com a gramatica Guru e com o conjunto de letras inserido."""
    b = ''.join(tup1)
    lst = [] #E criada uma lista vazia.
    lst2 = [] #E criada uma segunda lista vazia.
    for a in range(1, len(tup1) + 1): #Para cada a que esteja entre 1 e o comprimento da variavel tup1.
        for c in permutations(b, a):
            lst += [''.join(c)]
    for argp in lst: #Para cada variavel argp que esteja dentro da lista.
        if e_palavra_potencial(argp): #Se possuir as caracteristicas de uma palavra_potencial.
            lst2 += [a.join(argp)] #E adicionada uma palavra valida a lista.
    return lst2 #E retornada a lista com todas as palavras validas.

def guru_mj(tup1):
    """Permite jogar um jogo completo de Palavra Guru MultiJogador."""
    n=1 #O valor de n inicialmente e 1.
    a=input('Introduza o nome dos jogadores (-1 para terminar)... ','Jogador ',n+1,' -> ')
    b=input('JOGADA k - Falta descobrir n palavras')
    print('Introduza o nome dos jogadores (-1 para terminar)... ')
    input('Jogador ',n,' -> ')
    if a=='-1':
        return b #Se for introduzido -1 no nome dos jogadores, o jogo passa para o passo seguinte.
    else:
        return a #O jogo continua a adicionar jogadores enquanto nao for introduzido -1.


