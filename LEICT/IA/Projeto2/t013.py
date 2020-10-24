# Grupo 013 - 90705 Andre Silva / 90764 Pedro Leitao

import random
import math

# LearningAgent to implement
# no knowledeg about the environment can be used
# the code should work even with another environment
class LearningAgent:

        # init
        # nS maximum number of states
        # nA maximum number of action per state
        def __init__(self,nS,nA):

                self.nS = nS
                self.nA = nA

                self.actionValues = [-math.inf]*nS
                for i in range(0, nS):
                    self.actionValues[i] = [-math.inf]*nA

                self.eValue = 1

                self.actionTable = [0]*nS
                for j in range(0, nS):
                    self.actionTable[j] = [0]*nA


        # Select one action, used when learning
        # st - is the current state
        # aa - is the set of possible actions
        # for a given state they are always given in the same order
        # returns
        # a - the index to the action in aa
        def selectactiontolearn(self,st,aa):

                num = random.random()
                a = 0
                aux = []

                if num < self.eValue:
                    a = random.randint(0, len(aa) - 1)

                    minimo = self.actionTable[st][0]
                    for j in range(1, len(aa)):
                        if self.actionTable[st][j] < minimo:
                            minimo = self.actionTable[st][j]

                    for k in range(0, len(aa)):
                        if self.actionTable[st][k] == minimo and self.actionTable[st][k] < 500:
                            aux.append(k)

                    if aux != []:
                        a = aux[random.randint(0, len(aux) - 1)]

                else:
                    maximo = self.actionValues[st][0]
                    for i in range(1, len(aa)):
                        if self.actionValues[st][i] > maximo:
                            maximo = self.actionValues[st][i]
                            a = i
                if self.eValue > 0.2:
                    self.eValue = self.eValue * 0.95

                self.actionTable[st][a] += 1

                return a

        # Select one action, used when evaluating
        # st - is the current state
        # aa - is the set of possible actions
        # for a given state they are always given in the same order
        # returns
        # a - the index to the action in aa
        def selectactiontoexecute(self,st,aa):

                a = 0
                maximo = self.actionValues[st][0]
                for i in range(1, len(aa)):
                    if self.actionValues[st][i] > maximo:
                        maximo = self.actionValues[st][i]
                        a = i
                return a


        # this function is called after every action
        # st - original state
        # nst - next state
        # a - the index to the action taken
        # r - reward obtained
        def learn(self,ost,nst,a,r):

                value = self.actionValues[ost][a]

                if value == -math.inf:
                    value = 0
                maximo = self.actionValues[nst][0]

                for i in range(1, self.nA):
                    if self.actionValues[nst][i] > maximo:
                        maximo = self.actionValues[nst][i]

                if maximo == -math.inf:
                    maximo = 0
                    self.actionValues[ost][a] = r + 0.8 * maximo

                else:
                    self.actionValues[ost][a] = value + 0.3 * (r + 0.8 * maximo - value)

                return
