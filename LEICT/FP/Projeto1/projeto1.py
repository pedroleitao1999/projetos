#ist190764 Pedro Leitao

artigo_def=['A','O']
vogal_palavra=['E']+artigo_def
vogal=['I','U']+vogal_palavra
consoante=['B','C','D','F','G','H','J','L','M','N','P','Q','R','S','T','V','X','Z']
par_consoantes=['BR','CR','FR','GR','PR','TR','VR','BL','CL','FL','GL','PL']
consoante_terminal=['L','M','R','S','X','Z']
consoante_final=['N','P']+consoante_terminal
consoante_freq=['D','L','M','N','P','R','S','T','V']
ditongo_palavra=['AI','AO','EU','OU']
ditongo=['AE','AU','EI','OE','OI','IU']+ditongo_palavra
par_vogais=['IA','IO']+ditongo

def silaba_final(string):
    """Verifica se a string introduzida possui as caracteristicas de silaba final de uma palavra."""
    if monossilabo_2(string):
        return True
    elif monossilabo_3(string):
        return True
    elif silaba_4(string):
        return True
    elif silaba_5(string):
        return True
    else:
        return False

def silaba_5(string):
    """Verifica se a string introduzida possui as caracteristicas de uma silaba com 5 letras."""
    if len(string)!= 5:
        return False
    if string[0:2] in par_consoantes and string[2] in vogal and string[3]=='N' and string[4]=='S':
        return True
    else:
        return False

def silaba_4(string):
    """Verifica se a string introduzida possui as caracteristicas de uma silaba com 4 letras."""
    if len(string)!= 4:
        return False
    if string[0:2] in par_vogais and string[2]=='N' and string[3]=='S':
        return True
    elif string[0] in consoante and string[1] in vogal and string[2]==['N','I'] and string[3]=='S':
        return True
    elif string[0:2] in par_consoantes and string[2:4] in par_vogais:
        return True
    elif string[0] in consoante and string[1:3] in par_vogais and string[3] in consoante_final:
        return True
    else:
        return False

def silaba_3(string):
    """Verifica se a string introduzida possui as caracteristicas de uma silaba com 3 letras."""
    if len(string)!= 3:
        return False
    if string[0]=='Q' and string[1]=='U' and string[2] in ['A','E','I']:
        return True
    elif string[0]=='G' and string[1]=='U' and string[2] in ['E','I']:
        return True
    elif string[0] in vogal and string[1]=='N' and string[2]=='S':
        return True
    elif string[0] in consoante and string[1:3] in par_vogais:
        return True
    elif string[0] in consoante and string[1] in vogal and string[2] in consoante_final:
        return True
    elif string[0:2] in par_vogais and string[2] in consoante_final:
        return True
    elif string[0:2] in par_consoantes and string[2] in vogal:
        return True
    else:
        return False

def silaba_2(string):
    """Verifica se a string introduzida possui as caracteristicas de uma silaba com 2 letras."""
    if len(string)!= 2:
        return False
    if string in par_vogais:
        return True
    elif string[0] in consoante and string[1] in vogal:
        return True
    elif string[0] in vogal and string[1] in consoante_final:
        return True
    else:
        return False

def monossilabo_3(string):
    """Verifica se a string introduzida possui as caracteristicas de um monossilabo com 3 letras."""
    if len(string)!= 3:
        return False
    if string[0] in consoante and string[1] in vogal and string[2] in consoante_terminal:
        return True
    elif string[0] in consoante and string[1:3] in ditongo:
        return True
    elif string[0:2] in par_vogais and string[2] in consoante_terminal:
        return True
    else:
        return False

def monossilabo_2(string):
    """Verifica se a string introduzida possui as caracteristicas de um monossilabo com 2 letras."""
    if len(string)!= 2:
        return False
    if string[0] in ['A','I'] and string[1]=='R':
        return True
    elif string[0] in ['E','U'] and string[1]=='M':
        return True
    elif string[0] in vogal_palavra and string[1]=='S':
        return True
    elif string in ditongo_palavra:
        return True
    elif string[0] in consoante_freq and string[1] in vogal:
        return True
    else:
        return False

def e_silaba(string):
    """Verifica se a string introduzida possui as caracteristicas de uma silaba."""
    if type(string)!= str:
        raise ValueError('e_silaba:argumento invalido')
    if string in vogal:
        return True
    elif silaba_2(string):
        return True
    elif silaba_3(string):
        return True
    elif silaba_4(string):
        return True
    elif silaba_5(string):
        return True
    else:
        return False

def e_monossilabo(string):
    """Verifica se a string introduzida possui as caracteristicas de um monossilabo."""
    if type(string)!= str:
        raise ValueError('e_monossilabo:argumento invalido')
    if string in vogal_palavra:
        return True
    elif monossilabo_2(string):
        return True
    elif monossilabo_3(string):
        return True
    else:
        return False

def e_palavra(string):
    """Verifica se a string introduzida possui as caracteristicas de uma palavra."""
    if type(string)!= str:
        raise ValueError('e_palavra:argumento invalido')
    def polissilabo(string):
        """Verifica se a string introduzida possui as caracteristicas de um polissilabo."""
        def silabas_nao_finais(string, tam):
            """Verifica se a string introduzida possui as caracteristicas de uma ou mais silabas que nao sao a silaba final"""
            if len(string)==0:
                return True
            elif tam>5:
                return False
            else:
                if e_silaba(string[0:tam]) and silabas_nao_finais(string[tam:len(string)],1):
                    return True
                else:
                    return silabas_nao_finais(string,tam+1)
        for i in range(2,6):
            if silaba_final(string[len(string)-i:len(string)]):
                nova_str = string[0:len(string)-i]
                if silabas_nao_finais(nova_str, 1):
                    return True
        return False
    if e_monossilabo(string):
        return True
    elif polissilabo(string):
        return True
    else:
        return False