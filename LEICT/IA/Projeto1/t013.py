# Grupo 013: 90705 Andre Silva / 90764 Pedro Leitao

import math
import pickle
import time

class Node:

    def __init__(self, position, veichle, parentInit, parentGoal):
        self.position = position
        self.veichle = veichle
        self.parentInit = parentInit
        self.parentGoal = parentGoal

class Map:

    def __init__(self, graph):
        self.graph = graph
        pass

    def BFS(self, numAgents, initNodes, goalNodes, tickets = []):

        totalPaths = []
        agent = 0

        while agent < numAgents:

            visitedInit = [False]*(len(self.graph))
            visitedGoal = [False]*(len(self.graph))

            initQueue = []
            goalQueue = []

            initTotal = []
            goalTotal = []

            visitedInit[initNodes[agent].position] = True
            visitedGoal[goalNodes[agent].position] = True

            initQueue.append(initNodes[agent])
            goalQueue.append(goalNodes[agent])

            initTotal.append(initNodes[agent])
            goalTotal.append(goalNodes[agent])

            intersectionNodes = []

            while initQueue and goalQueue:

                initNode = initQueue.pop(0)
                goalNode = goalQueue.pop(0)

                for i in self.graph[initNode.position]:
                    position = i[1]
                    veichle = i[0]
                    parentInit = initNode.position
                    parentGoal = 0
                    thisNode = Node(position, veichle, parentInit, parentGoal)
                    if visitedInit[position] == False:
                        initQueue.append(thisNode)
                        initTotal.append(thisNode)
                        visitedInit[position] = True
                    if visitedGoal[position] == True:
                        for i in goalTotal:
                            if i.position == position:
                                parentGoal = i.parentGoal
                                i.parentInit = parentInit
                        thisIntersectionNode = Node(position, veichle, parentInit, parentGoal)
                        intersectionNodes.append(thisIntersectionNode)

                for i in self.graph[goalNode.position]:
                    goalNode.veichle = i[0]
                    veichle = i[0]
                    position = i[1]
                    parentInit = 0
                    parentGoal = goalNode.position
                    thisNode = Node(position, veichle, parentInit, parentGoal)
                    if visitedGoal[position] == False:
                        goalQueue.append(thisNode)
                        goalTotal.append(thisNode)
                        visitedGoal[position] = True
                    if visitedInit[position] == True:
                        for i in initTotal:
                            if i.position == position:
                                parentInit = i.parentInit
                                i.parentGoal = parentGoal
                                veichle = i.veichle
                        thisIntersectionNode = Node(position, veichle, parentInit, parentGoal)
                        intersectionNodes.append(thisIntersectionNode)

            path  = self.createPath(intersectionNodes, initTotal, goalTotal, tickets)
            totalPaths.append(path)
            return self.createOutput(numAgents, totalPaths)

    def checkPath(self, path, tickets = []):
        aux = tickets
        taxi = 0
        bus = 0
        subway = 0
        for i in path:
            if i.veichle == 0:
                taxi += 1
                if aux[0] < taxi:
                    return False
            elif i.veichle == 1:
                bus += 1
                if aux[1] < bus:
                    return False
            elif i.veichle == 2:
                subway += 1
                if aux[2] < subway:
                    return False
        return True

    def createPath(self, intersectionNodes = [], initNodes = [], goalNodes = [], tickets = []):

        for i in intersectionNodes:

            path = []

            path.append(i)

            while path[0].parentInit != 0:
                for m in initNodes:
                    if m.position == path[0].parentInit:
                        path.insert(0, m)

            while path[len(path) - 1].parentGoal != 0:
                for n in goalNodes:
                    if n.position == path[len(path) - 1].parentGoal:
                        path.append(n)

            num = 0
            while num < len(path) - 1:
                self.getVeichle(num, path)
                num = num + 1

            if(self.checkPath(path, tickets) == True):
                return path
        return []

    def getVeichle(self, num, path = []):

        for i in self.graph[path[num].position]:
            if i[1] == path[num + 1].position:
                path[num + 1].veichle = i[0]



    def createOutput(self, numAgentes, finalPaths = []):

        output = []

        for i in range(0, len(finalPaths[0])):
            veiculos = []
            posicoes = []
            total = []
            agent = 0
            for p in finalPaths:
                if(p[i].veichle != -3):
                    veiculos.insert(agent, p[i].veichle)
                posicoes.insert(agent, p[i].position)
                agent += 1
            total.append(veiculos)
            total.append(posicoes)
            output.append(total)

        return output

class SearchProblem:

  def __init__(self, goal, model, auxheur = []):
     self.goal = goal
     self.model = model
     self.auxheur = auxheur
     pass

  def search(self, init, limitexp = 2000, limitdepth = 10, tickets = [math.inf,math.inf,math.inf]):
      map = Map(self.model)
      initNodes = []
      goalNodes = []
      for i in init:
          num = 0
          node = Node(init[num], -3, 0, 0)
          initNodes.append(node)
          num += 1
      for g in self.goal:
          num = 0
          node = Node(self.goal[num], -3, 0, 0)
          goalNodes.append(node)
          num += 1
      numAgents = len(initNodes)
      list = map.BFS(numAgents, initNodes, goalNodes, tickets)
      return list
