#!/usr/bin/python3
# -*- coding: latin-1 -*-
#
# Proposta de Resolucao do trabalho pratico 1 de FP.
# Rui Maranhao -- rui@computer.org
#
def artigo_def(chr) :
    """
    <artigo_def> ::= A | O
    """
    return chr in ('A', 'O')

def vogal_palavra(chr):
    """
    <vogal_palavra> ::= <artigo_def> | E
    """
    return artigo_def(chr) or chr == 'E'

def vogal(chr):
    """
    <vogal> ::= I | U | <vogal_palavra>
    """
    return (chr in ('I', 'U')) or vogal_palavra(chr)

def ditongo_palavra(inp):
    """
    <ditongo_palavra> ::= AI | AO | EU | OU
    """
    return inp in ('AI', 'AO', 'EU', 'OU')

def ditongo(inp):
    """
    <ditongo> ::= AE | AU | EI | OE | OI | IU  |<ditongo_palavra>
    """
    return (inp in ('AE', 'AU', 'EI', 'OE', 'OI', 'IU')) or ditongo_palavra(inp)

def par_vogais(inp):
    """
    <par_vogais> ::= <ditongo> | IA | IO
    """
    return ditongo(inp) or (inp in ('IA', 'IO'))

def consoante_freq(inp):
    """
    <consoante_freq> ::= D | L | M | N | P | R | S | T | V
    """
    return inp in ('D', 'L', 'M', 'N', 'P', 'R', 'S', 'T', 'V')

def consoante_terminal(inp):
    """
    <consoante_terminal> ::= L| M| R| S| X| Z
    """
    return inp in ('L', 'M', 'R', 'S', 'X', 'Z')

def consoante_final(inp):
    """
    <consoante_final> ::= N | P | <consoante_terminal>
    """
    return (inp in ('N', 'P')) or consoante_terminal(inp)

def consoante(inp):
    """
    <consoante>::= B| C| D| F| G| H| J| L| M| N| P| Q| R| S| T| V| X| Z
    """
    return inp in ('B', 'C', 'D', 'F', 'G', 'H', 'J', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z')

def par_consoantes(inp):
    """
    <par_consoantes>::= BR| CR| FR| GR| PR| TR| VR | BL| CL| FL| GL| PL
    """
    return inp in ('BR', 'CR', 'FR', 'GR', 'PR', 'TR', 'VR', 'BL', 'CL', 'FL', 'GL', 'PL')

def monossilabo_2(inp):
    """
    <monossilabo_2>::= AR | IR | EM | UM | <vogal_palavra> S | <ditongo_palavra> | <consoante_freq> <vogal>
    """
    if len(inp) != 2:
        return False

    if inp in ('AR', 'IR', 'EM', 'UM'):
        return True
    else:
        return (ditongo_palavra(inp)) or (vogal_palavra(inp[0]) and inp[1] == 'S') or (consoante_freq(inp[0]) and vogal(inp[1]))

def monossilabo_3(inp):
    """
    <monossilabo_3> ::=  <consoante> <vogal> <consoante_terminal>
                       | <consoante> <ditongo>
                       | <par_vogais> <consoante_terminal>
                       | <consoante> <vogal> <vogal>
    """
    if len(inp) == 3:
        return ((consoante(inp[0]) and vogal(inp[1]) and consoante_terminal(inp[2])) or
                (par_vogais(inp[0:2]) and consoante_terminal(inp[2:])) or
                (consoante(inp[0]) and ditongo(inp[1:])))

    return False

def monossilabo(inp):
    """
    <monossilabo> ::= <vogal_palavra>|<monossilabo_2>|<monossilabo_3>
    """
    return vogal_palavra(inp) or monossilabo_2(inp) or monossilabo_3(inp)


def silaba_2(inp):
    """
    silaba_2 ::= <par_vogais> | <consoante> <vogal> | <vogal> <consoante_final>
    """
    if len(inp) != 2:
        return False

    return par_vogais(inp) or (consoante(inp[0]) and vogal(inp[1:])) or (vogal(inp[0]) and consoante_final(inp[1:]))

def silaba_3(inp):
    """
    <silaba3>::= QUA| QUE| QUI| GUE| GUI|<vogal> NS
              |<consoante><par_vogais>
              |<consoante><vogal><consoante_final>
              |<par_vogais><consoante_final>
              |<par_consoantes><vogal>
    """
    if len(inp) != 3:
        return False

    return (inp in ('QUA', 'QUE', 'QUI', 'GUE', 'GUI') or
        vogal(inp[0]) and inp[1:] == "NS" or
        consoante(inp[0]) and par_vogais(inp[1:]) or
        consoante(inp[0]) and vogal(inp[1]) and consoante_final(inp[2:]) or
        par_vogais(inp[0:2]) and consoante_final(inp[2:]) or
        par_consoantes(inp[0:2]) and vogal(inp[2:]))

def silaba_4(inp):
    """
    <silaba_4>::=  <par_vogais> NS
                |  consoante><vogal> NS
                |  <consoante><vogal> IS
                |  <par_consoantes><par_vogais>
                |  <consoante><par_vogais><consoante_final>
    """
    if len(inp) != 4:
        return False

    return ((par_vogais(inp[0:2]) and inp[2:] == 'NS') or
            (consoante(inp[0]) and vogal(inp[1]) and inp[2:] == 'NS') or
            (consoante(inp[0]) and vogal(inp[1]) and inp[2:] == 'IS') or
            (par_consoantes(inp[0:2]) and par_vogais(inp[2:])) or
            (consoante(inp[0]) and par_vogais(inp[1:3]) and consoante_final(inp[3:])))

def silaba_5(inp):
    """
    <silaba_5> ::= <par_consoantes><vogal> NS

    """
    if len(inp) < 5:
        return False

    return par_consoantes(inp[0:2]) and vogal(inp[2]) and inp[3:] == 'NS'

def silaba_final(inp):
    """
    <silaba_final>::=<monossilabo_2> | <monossilabo_3> | <silaba_4> | <silaba_5>
    """
    return monossilabo_2(inp) or monossilabo_3(inp) or silaba_4(inp) or silaba_5(inp)

def silaba(inp):
    """
    <silaba>::=<vogal>|<silaba_2>|<silaba_3>|<silaba_4>|<silaba_5>
    """
    return vogal(inp) or silaba_2(inp) or silaba_3(inp) or silaba_4(inp) or silaba_5(inp)

def palavra(inp):
    """
    <palavra>::=<silaba>* <silaba_final>|<monossilabo>
    """

    def palavra_auxilar(inp):
        if inp == '' or silaba(inp):
            return True

        for i in range(1, len(inp)):
            if silaba(inp[:i]) and palavra_auxilar(inp[i:]):
                return True

    if monossilabo(inp) or silaba_final(inp):
        return True

    for i in range(len(inp)-5, len(inp) - 1):
        if silaba_final(inp[i:]) and palavra_auxilar(inp[:i]):
            return True

    return False


def e_silaba(string):
    """
    Verifica se uma string é silaba
    :param string: string a verificar
    :return: Verdadeiro (True) ou Falso (False)
    """
    if not isinstance(string, str):
        raise ValueError('e_silaba:argumento invalido')

    return silaba(string)

def e_monossilabo(string):
    """
    Verifica se uma string é monossilabo
    :param string: a verificar
    :return: Verdadeiro (True) ou Falso (False)
    """
    if not isinstance(string, str):
        raise ValueError('e_monossilabo:argumento invalido')

    return monossilabo(string)

def e_palavra(string):
    """
    Verifica se uma string e uma palavra valida

    :param string: palavra a verificar
    :return: Verdadeiro (True) ou Falso (False)
    """
    if not isinstance(string, str):
        raise ValueError('e_palavra:argumento invalido')

    return palavra(string)
