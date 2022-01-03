import networkx as nx
import matplotlib.pyplot as plt
import matplotlib as mpl
import random
import imageio
import os
from networkx.algorithms import graphical

from networkx.algorithms.dominating import dominating_set
from networkx.drawing.layout import shell_layout
from PIL import Image, ImageDraw

seed = 10
random.seed(seed)

def draw(graph, dicts, infected, recovered, images):
    color_map = ['red' if node in infected else 'green' if node in recovered else 'dodgerblue' for node in graph]
    pos = nx.spring_layout(graph, k = 0.05, seed = seed)
    nx.draw(graph, pos = pos, nodelist=dicts, node_size=15, node_color=color_map, width = 0.1)
    filename = f'{len(images)}.png'
    images.append(filename)
    plt.savefig(filename)
    del filename
    return images

def reset_infection(nodes_number, dicts, healthy, keys):
    for i in range(nodes_number):
        healthy.append(i)
    for i in keys:
        dicts[i] = 0
    return dicts, healthy

def random_node(list):
    index = random.randint(0, len(list) - 1)
    return list[index]

def getHighestKshell(graph):
    i = 1
    while(True):
        if len(nx.k_shell(graph, k=i)) == len(nx.k_shell(graph)) and len(nx.k_shell(graph, k=i+1)) == 0:
            return i
        i += 1

def infect(node, dicts, healthy, infected):
    dicts[node] = 1
    infected.append(node)
    healthy.remove(node)
    return dicts, healthy, infected

def return_to_susceptible(node, dicts, healthy, infected):
    dicts[node] = 0
    infected.remove(node)
    healthy.append(node)
    return dicts, healthy, infected

def become_recovered(node, dicts, infected, recovered):
    dicts[node] = 2
    infected.remove(node)
    recovered.append(node)
    return dicts, infected, recovered

def infect_CR(node, dicts, healthy, infected, recovered):
    dicts[node] = 1
    infected.append(node)
    if node in recovered:
        infected.remove(node)
    if node in healthy:
        healthy.remove(node)
    return dicts, healthy, infected, recovered

def recover_CR(node, dicts, healthy, infected, recovered):
    dicts[node] = 2
    recovered.append(node)
    if node in infected:
        infected.remove(node)
    if node in healthy:
        healthy.remove(node)
    return dicts, healthy, infected, recovered

def run_one_SI_simulation(graph, dicts, healthy, infected, images):
    n_infected = len(infected)
    n_iterations = 0
    infected_aux = infected.copy()
    while len(infected)/len(graph) < 0.9:
        n_iterations += 1
        for j in infected_aux:
            aux = graph.adj[j]
            for k in aux:
                if (random.random() <= beta):
                    if (k in healthy):
                        dicts, healthy, infected = infect(k, dicts, healthy, infected)
                        n_infected += 1
        infected_aux = infected.copy()
        if len(graph) <= 500:
            images = draw(graph, dicts, infected, [], images)
    print("Performed", n_iterations, "iterations and reached", n_infected, "infected nodes")
    global num_models_run
    num_models_run += 1
    ###### CREATE GIF ######
    if len(graph) <= 500:
        with imageio.get_writer(f'my_gif_SI_{num_models_run}.gif', mode='I') as writer:
            for filename in images:
                image = imageio.imread(filename)
                writer.append_data(image)
        for filename in set(images):
            os.remove(filename)
    return n_iterations

def run_one_SIS_simulation(graph, dicts, healthy, infected, images):
    n_infected = len(infected)
    n_infected_aux = len(infected)
    n_healthy = len(dicts) - len(infected)
    n_iterations = 0
    infected_aux = infected.copy()
    while len(infected)/len(graph) < 0.9:
        n_iterations += 1
        for j in infected_aux:
            aux = graph.adj[j]
            for k in aux:
                if (random.random() <= beta):
                    if (k in healthy):
                        dicts, healthy, infected = infect(k, dicts, healthy, infected)
                        n_infected += 1
                        n_healthy -= 1
        infected_aux = infected.copy()
        for j in infected_aux:
            if(random.random() <= delta):
                dicts, healthy, infected = return_to_susceptible(j, dicts, healthy, infected)
                n_healthy += 1
                n_infected -= 1
        infected_aux = infected.copy()
        if ( abs(n_infected - n_infected_aux) < len(graph)*variance and len(infected)/len(graph) > 0.5 ) or n_infected == 0:
            break
        n_infected_aux = n_infected
        if len(graph) <= 500:
            images = draw(graph, dicts, infected, [], images)
    if n_iterations > 10:
        print("Performed", n_iterations, "iterations and reached", n_infected, "infected nodes")
    global num_models_run
    num_models_run += 1
    ###### CREATE GIF ######
    if len(graph) <= 500:
        with imageio.get_writer(f'my_gif_SIS_{num_models_run}.gif', mode='I') as writer:
            for filename in images:
                image = imageio.imread(filename)
                writer.append_data(image)
        for filename in set(images):
            os.remove(filename)
    return n_iterations

def run_one_SIR_simulation(graph, dicts, healthy, infected, recovered, images):
    n_infected = len(infected)
    n_healthy = len(dicts) - len(infected)
    n_recovered = 0
    n_iterations = 0
    max_infected = len(infected)
    infected_aux = infected.copy()
    plot_iterations = []
    plot_recovered = []
    plot_infected = []
    plot_healthy = []
    plot_iterations.append(n_iterations)
    plot_recovered.append(n_recovered)
    plot_infected.append(n_infected)
    plot_healthy.append(n_healthy)
    while len(infected)/len(graph) < 0.9:
        n_iterations += 1
        for j in infected_aux:
            aux = graph.adj[j]
            for k in aux:
                if (random.random() <= beta):
                    if (k in healthy):
                        dicts, healthy, infected = infect(k, dicts, healthy, infected)
                        n_infected += 1
                        n_healthy -= 1
        infected_aux = infected.copy()
        n_infected_aux = n_infected
        n_healthy_aux = n_healthy
        for j in infected_aux:
            if(random.random() <= delta):
                dicts, infected, recovered = become_recovered(j, dicts, infected, recovered)
                n_recovered +=1
                n_infected -= 1
        infected_aux = infected.copy()
        if n_infected > max_infected:
            max_infected = n_infected
        if n_iterations >= 2000 or n_infected == 0:
            break
        plot_iterations.append(n_iterations)
        plot_recovered.append(n_recovered)
        plot_infected.append(n_infected)
        plot_healthy.append(n_healthy)
        if len(graph) <= 500:
            images = draw(graph, dicts, infected, recovered, images)
    if n_iterations > 10:
        print("Performed", n_iterations, "iterations and reached", n_infected, "infected nodes, infecting a maximum of", max_infected, "nodes")
    global num_models_run
    num_models_run += 1
    ###### CREATE GIF ######
    if len(graph) <= 500:
        with imageio.get_writer(f'my_gif_SIR_{num_models_run}.gif', mode='I') as writer:
            for filename in images:
                image = imageio.imread(filename)
                writer.append_data(image)
        for filename in set(images):
            os.remove(filename)
    ###### PLOT LINE CHART ######
    plt.plot(plot_iterations, plot_recovered, label='recovered', color='green')
    plt.plot(plot_iterations, plot_infected, label='infected', color='red')
    plt.plot(plot_iterations, plot_healthy, label='healthy', color='blue')
    plt.show()
    return n_iterations, max_infected

def run_one_competing_rumours_simulation(graph, dicts, healthy, infected, recovered, images):
    n_infected = len(infected)
    n_healthy = len(dicts) - len(infected) - len(recovered)
    n_recovered = len(recovered)
    n_iterations = 0
    max_infected = len(recovered)
    max_recovered = len(infected)
    infected_aux = infected.copy()
    recovered_aux = recovered.copy()
    print(nx.degree_histogram(graph))
    while len(healthy)/len(graph) > 0.05:
        n_iterations += 1
        for j in infected_aux:
            aux = graph.adj[j]
            for k in aux:
                if (random.random() <= beta):
                    if (k in healthy):
                        dicts, healthy, infected, recovered = infect_CR(k, dicts, healthy, infected, recovered)
                        n_infected += 1
                        n_healthy -= 1
                    if (k in recovered):
                        dicts, healthy, infected, recovered = infect_CR(k, dicts, healthy, infected, recovered)
                        n_infected += 1
                        n_recovered -= 1
        for l in recovered_aux:
            aux = graph.adj[l]
            for m in aux:
                if (random.random() <= beta):
                    if (m in healthy):
                        dicts, healthy, infected, recovered = recover_CR(m, dicts, healthy, infected, recovered)
                        n_recovered += 1
                        n_healthy -= 1
                    if (m in infected):
                        dicts, healthy, infected, recovered = recover_CR(m, dicts, healthy, infected, recovered)
                        n_recovered += 1
                        n_infected -= 1
        infected_aux = infected.copy()
        recovered_aux = recovered.copy()
        if(n_iterations % 5 == 0):
            print("After", n_iterations, "iterations there are:", n_recovered, "recovered, and", n_infected, "infected")
        if n_infected > max_infected:
            max_infected = n_infected
        if n_recovered > max_recovered:
            max_recovered = n_recovered
        if len(graph) <= 500:
            images = draw(graph, dicts, infected, recovered, images)
    global num_models_run
    num_models_run += 1
    print("After", n_iterations, "iterations there are:", n_recovered, "recovered, and", n_infected, "infected")
    ###### CREATE GIF ######
    if len(graph) <= 500:
        with imageio.get_writer(f'my_gif_SIR_{num_models_run}.gif', mode='I') as writer:
            for filename in images:
                image = imageio.imread(filename)
                writer.append_data(image)
        for filename in set(images):
            os.remove(filename)
    return n_iterations, n_recovered, n_infected

def find_best_and_worst_kshells(graph, low_ks):
    bestKsList = []
    worstKsList = []
    bestKsNodes = nx.k_shell(graph)
    bestKsList = list(bestKsNodes.nodes)
    worstKsNodes = nx.k_shell(graph, k=low_ks)
    worstKsList = list(worstKsNodes.nodes)
    return bestKsList, worstKsList

def find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList):
    minDeg = nodes_number
    minDegNode = 0
    for node in degrees:
        if node[1] < minDeg and node[0] in bestKsList:
            minDeg = node[1]
            minDegNode = node[0]
    return minDegNode, minDeg

def find_highest_k_from_lowest_ks(degrees, worstKsList):
    maxDeg = 0
    maxDegNode = 0
    for node in degrees:
        if node[1] > maxDeg and node[0] in worstKsList:
            maxDeg = node[1]
            maxDegNode = node[0]
    return maxDegNode, maxDeg

def nodes_setup(nodes_number):
    healthy = []
    infected = []
    dicts = {}
    keys = range(nodes_number)
    dicts, healthy = reset_infection(nodes_number, dicts, healthy, keys)
    return dicts, healthy, infected

def nodes_setup_SIR(nodes_number):
    healthy = []
    infected = []
    recovered = []
    dicts = {}
    keys = range(nodes_number)
    dicts, healthy = reset_infection(nodes_number, dicts, healthy, keys)
    return dicts, healthy, infected, recovered

def run_all_SI_simulations(graph, nodes_number, low_ks):
    print("Network with", nodes_number, "nodes in the SI model")
    images = []
    dicts, healthy, infected = nodes_setup(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    minDegNode, minDeg = find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList)
    print("Deg = ", minDeg)
    ###### SIMULATION 1 - HIGH KS ######
    starterNode = minDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_ks_iterations = run_one_SI_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 1 FOR THE SI MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    maxDegNode, maxDeg = find_highest_k_from_lowest_ks(degrees, worstKsList)
    print("Deg = ", maxDeg)
    ###### SIMULATION 2 - HIGH K ######
    starterNode = maxDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_k_iterations = run_one_SI_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 2 FOR THE SI MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations

def run_all_SIS_simulations(graph, nodes_number, low_ks):
    print("Network with", nodes_number, "nodes in the SIS model")
    images = []
    dicts, healthy, infected = nodes_setup(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    minDegNode, minDeg = find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList)
    print("Deg = ", minDeg)
    ###### SIMULATION 1 - HIGH KS ######
    starterNode = minDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_ks_iterations = run_one_SIS_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 1 FOR THE SIS MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    maxDegNode, maxDeg = find_highest_k_from_lowest_ks(degrees, worstKsList)
    print("Deg = ", maxDeg)
    ###### SIMULATION 2 - HIGH K ######
    starterNode = maxDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_k_iterations = run_one_SIS_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 2 FOR THE SIS MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations

def run_all_SIR_simulations(graph, nodes_number, low_ks):
    print("Network with", nodes_number, "nodes in the SIR model")
    images = []
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    minDegNode, minDeg = find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList)
    print("Deg = ", minDeg)
    ###### SIMULATION 1 - HIGH KS ######
    starterNode = minDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_ks_iterations, high_ks_max_infected = run_one_SIR_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM 1 FOR THE SIR MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    maxDegNode, maxDeg = find_highest_k_from_lowest_ks(degrees, worstKsList)
    print("Deg = ", maxDeg)
    ###### SIMULATION 2 - HIGH K ######
    starterNode = maxDegNode
    dicts, healthy, infected = infect(starterNode, dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_k_iterations, high_k_max_infected = run_one_SIR_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM 2 FOR THE SIR MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected

def find_two_nodes_with_lowest_k_from_highest_ks(graph, nodes_number, degrees, bestKsList, connected_nodes):
    minDeg = nodes_number
    secondMinDeg = nodes_number
    twoMinDegs = []
    minDegNode = 0
    secondMinDegNode = 0
    twoMinDegNodes = []
    for node in degrees:
        if node[1] < minDeg and node[0] in bestKsList:
            if not connected_nodes or node[0] in graph.adj[minDegNode] or minDegNode == 0:
                secondMinDeg = minDeg
                minDeg = node[1]
                secondMinDegNode = minDegNode
                minDegNode = node[0]
    twoMinDegs.append(minDeg)
    twoMinDegs.append(secondMinDeg)
    twoMinDegNodes.append(minDegNode)
    twoMinDegNodes.append(secondMinDegNode)
    return twoMinDegNodes, twoMinDegs

def find_two_nodes_with_highest_k_from_lowest_ks(graph, degrees, worstKsList, connected_nodes):
    maxDeg = 0
    secondMaxDeg = 0
    twoMaxDegs = []
    maxDegNode = 0
    secondMaxDegNode = 0
    twoMaxDegNodes = []
    for node in degrees:
        if node[1] > maxDeg and node[0] in worstKsList:
            if not connected_nodes or node[0] in graph.adj[maxDegNode] or maxDegNode == 0:
                secondMaxDeg = maxDeg
                maxDeg = node[1]
                secondMaxDegNode = maxDegNode
                maxDegNode = node[0]
    twoMaxDegs.append(maxDeg)
    twoMaxDegs.append(secondMaxDeg)
    twoMaxDegNodes.append(maxDegNode)
    twoMaxDegNodes.append(secondMaxDegNode)
    return twoMaxDegNodes, twoMaxDegs

def run_all_SI_simulations_for_more_than_one_node(graph, nodes_number, low_ks, connected_nodes):
    print("Network with", nodes_number, "nodes in the SI model")
    images = []
    dicts, healthy, infected = nodes_setup(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    twoMinDegNodes, twoMinDegs = find_two_nodes_with_lowest_k_from_highest_ks(graph, 
        nodes_number, degrees, bestKsList, connected_nodes)
    print("First node with Deg = ", twoMinDegs[0])
    print("Second node with Deg = ", twoMinDegs[1])
    ###### SIMULATION 1 - HIGH KS ######
    dicts, healthy, infected = infect(twoMinDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMinDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_ks_iterations = run_one_SI_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 1 FOR THE SI MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    twoMaxDegNodes, twoMaxDegs = find_two_nodes_with_highest_k_from_lowest_ks(graph, degrees, worstKsList, connected_nodes)
    print("First node with Deg = ", twoMaxDegs[0])
    print("Second node with Deg = ", twoMaxDegs[1])
    ###### SIMULATION 2 - HIGH K ######
    dicts, healthy, infected = infect(twoMaxDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMaxDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_k_iterations = run_one_SI_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 2 FOR THE SI MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations

def run_all_SIS_simulations_for_more_than_one_node(graph, nodes_number, low_ks, connected_nodes):
    print("Network with", nodes_number, "nodes in the SIS model")
    images = []
    dicts, healthy, infected = nodes_setup(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    twoMinDegNodes, twoMinDegs = find_two_nodes_with_lowest_k_from_highest_ks(graph, 
        nodes_number, degrees, bestKsList, connected_nodes)
    print("First node with Deg = ", twoMinDegs[0])
    print("Second node with Deg = ", twoMinDegs[1])
    ###### SIMULATION 1 - HIGH KS ######
    dicts, healthy, infected = infect(twoMinDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMinDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_ks_iterations = run_one_SIS_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 1 FOR THE SIS MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    twoMaxDegNodes, twoMaxDegs = find_two_nodes_with_highest_k_from_lowest_ks(graph, degrees, worstKsList, connected_nodes)
    print("First node with Deg = ", twoMaxDegs[0])
    print("Second node with Deg = ", twoMaxDegs[1])
    ###### SIMULATION 2 - HIGH K ######
    dicts, healthy, infected = infect(twoMaxDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMaxDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, [], images)
    high_k_iterations = run_one_SIS_simulation(graph, dicts, healthy, infected, images)
    print("FINISHED SIM 2 FOR THE SIS MODEL")
    dicts, healthy, infected = nodes_setup(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations

def run_all_SIR_simulations_for_more_than_one_node(graph, nodes_number, low_ks, connected_nodes):
    print("Network with", nodes_number, "nodes in the SIR model")
    images = []
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    twoMinDegNodes, twoMinDegs = find_two_nodes_with_lowest_k_from_highest_ks(graph, 
        nodes_number, degrees, bestKsList, connected_nodes)
    print("First node with Deg = ", twoMinDegs[0])
    print("Second node with Deg = ", twoMinDegs[1])
    ###### SIMULATION 1 - HIGH KS ######
    dicts, healthy, infected = infect(twoMinDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMinDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_ks_iterations, high_ks_max_infected = run_one_SIR_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM 1 FOR THE SIR MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    ###### LOW KS & HIGH K ######
    twoMaxDegNodes, twoMaxDegs = find_two_nodes_with_highest_k_from_lowest_ks(graph, degrees, worstKsList, connected_nodes)
    print("First node with Deg = ", twoMaxDegs[0])
    print("Second node with Deg = ", twoMaxDegs[1])
    ###### SIMULATION 2 - HIGH K ######
    dicts, healthy, infected = infect(twoMaxDegNodes[0], dicts, healthy, infected)
    dicts, healthy, infected = infect(twoMaxDegNodes[1], dicts, healthy, infected)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_k_iterations, high_k_max_infected = run_one_SIR_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM 2 FOR THE SIR MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    return high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected

def run_all_competing_rumours_simulations(graph, nodes_number, low_ks):
    print("Network with", nodes_number, "nodes in the competing rumours model")
    images = []
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    bestKsList, worstKsList = find_best_and_worst_kshells(graph, low_ks)
    ###### HIGH KS & LOW K ######
    degrees = nx.degree(graph)
    minDegNode, minDeg = find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList)
    maxDegNode, maxDeg = find_highest_k_from_lowest_ks(degrees, worstKsList)
    print("Recovered Deg = ", minDeg)
    print("Infected Deg = ", maxDeg)
    ###### SIMULATION 1 - HIGH KS ######
    recoveredStarterNode = minDegNode
    infectedStarterNode = maxDegNode
    dicts, healthy, infected, recovered = recover_CR(recoveredStarterNode, dicts, healthy, infected, recovered)
    dicts, healthy, infected, recovered = infect_CR(infectedStarterNode, dicts, healthy, infected, recovered)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_ks_iterations, high_ks_max_recovered, high_k_max_infected = run_one_competing_rumours_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM1 FOR COMPETING RUMOURS MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    minDegNode, minDeg = find_lowest_k_from_highest_ks(nodes_number, degrees, bestKsList)
    maxDegNode, maxDeg = find_highest_k_from_lowest_ks(degrees, bestKsList)
    print("Recovered Deg = ", maxDeg)
    print("Infected Deg = ", minDeg)
    recoveredStarterNode = maxDegNode
    infectedStarterNode = minDegNode
    dicts, healthy, infected, recovered = recover_CR(recoveredStarterNode, dicts, healthy, infected, recovered)
    dicts, healthy, infected, recovered = infect_CR(infectedStarterNode, dicts, healthy, infected, recovered)
    if nodes_number <= 500:
        images = draw(graph, dicts, infected, recovered, images)
    high_ks_iterations, high_ks_max_recovered, high_k_max_infected = run_one_competing_rumours_simulation(graph, dicts, healthy, infected, recovered, images)
    print("FINISHED SIM2 FOR COMPETING RUMOURS MODEL")
    dicts, healthy, infected, recovered = nodes_setup_SIR(nodes_number)
    images = []
    return high_ks_iterations, high_ks_max_recovered, high_k_max_infected

###### GRAPHS CREATION AND CONFIGURATION ######
n_test = 10000
n_plot = 500
beta = 0.035
delta = 0.025 #Low recovery rate
gamma = 2.8
tau2 = 2
mu = 0.5
variance = 0.005 
num_models_run = 0

LFR_G_test = nx.LFR_benchmark_graph(n_test, gamma, tau2, mu, min_degree = 2, seed = seed)
LFR_G_test.remove_edges_from(nx.selfloop_edges(LFR_G_test))
LFR_G_plot = nx.LFR_benchmark_graph(n_plot, gamma, tau2, mu, min_degree = 4, seed = seed)
LFR_G_plot.remove_edges_from(nx.selfloop_edges(LFR_G_plot))

DMS_G_test = nx.duplication_divergence_graph(n_test, 0.5, seed = seed)
DMS_G_plot = nx.duplication_divergence_graph(n_plot, 0.5, seed = seed)

print("The LFR benchmark network with 10000 nodes has a maximum k-shell value of", getHighestKshell(LFR_G_test))
print("The LFR benchmark network with 500 nodes has a maximum k-shell value of", getHighestKshell(LFR_G_plot))
print("The duplication divergence network with 10000 nodes has a maximum k-shell value of", getHighestKshell(DMS_G_test))
print("The duplication divergence network with 500 nodes has a maximum k-shell value of", getHighestKshell(DMS_G_plot))

LFR_G_low_ks = 2
DMS_G_low_ks = 7
LFR_G_plot_low_ks = 4
DMS_G_plot_low_ks = 4

###### GRAPH MODEL 1 - COMPETING RUMORS ######
print("Running the simulation for the Competing Rumors model")
run_all_competing_rumours_simulations(LFR_G_test, n_test, LFR_G_low_ks)
run_all_competing_rumours_simulations(LFR_G_plot, n_plot, LFR_G_plot_low_ks)

###### GRAPH MODEL 2 - LFR-Benchmark ######
print("Running the simulations for the LFR-Benchmark network")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations(LFR_G_test, n_test, LFR_G_low_ks)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations(LFR_G_plot, n_plot, LFR_G_plot_low_ks)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations(LFR_G_test, n_test, LFR_G_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations(LFR_G_plot, n_plot, LFR_G_plot_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations(LFR_G_test, n_test, LFR_G_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations(LFR_G_plot, n_plot, LFR_G_plot_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

###### GRAPH MODEL 3 - DMS-Link Duplication ######
print("Running the simulations for the DMS-Link Duplication network")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations(DMS_G_test, n_test, DMS_G_low_ks)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations(DMS_G_plot, n_plot, DMS_G_plot_low_ks)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations(DMS_G_test, n_test, DMS_G_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations(DMS_G_plot, n_plot, DMS_G_plot_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations(DMS_G_test, n_test, DMS_G_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations(DMS_G_plot, n_plot, DMS_G_plot_low_ks)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

###### RUN SIMULATIONS WITH MORE THAN ONE SPREADER ######

###### GRAPH MODEL 1 - LFR-Benchmark ######
print("Running the simulations for the LFR-Benchmark network with more than one spreader without connected spreaders")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations_for_more_than_one_node(LFR_G_test, n_test, LFR_G_low_ks, False)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations_for_more_than_one_node(LFR_G_plot, n_plot, LFR_G_plot_low_ks, False)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations_for_more_than_one_node(LFR_G_test, n_test, LFR_G_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations_for_more_than_one_node(LFR_G_plot, n_plot, LFR_G_plot_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations_for_more_than_one_node(LFR_G_test, 
        n_test, LFR_G_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations_for_more_than_one_node(LFR_G_plot, 
        n_plot, LFR_G_plot_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

###### WITH CONNECTED SPREADERS ######
print("Running the simulations for the LFR-Benchmark network with more than one spreader with connected spreaders")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations_for_more_than_one_node(LFR_G_test, 
        n_test, LFR_G_low_ks, True)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

###### GRAPH MODEL 2 - DMS-Link Duplication ######
print("Running the simulations for the DMS-Link Duplication network with more than one spreader without connected spreaders")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations_for_more_than_one_node(DMS_G_test, n_test, DMS_G_low_ks, False)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations_for_more_than_one_node(DMS_G_plot, n_plot, DMS_G_plot_low_ks, False)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations_for_more_than_one_node(DMS_G_test, n_test, DMS_G_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations = run_all_SIS_simulations_for_more_than_one_node(DMS_G_plot, n_plot, DMS_G_plot_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations_for_more_than_one_node(DMS_G_test, 
        n_test, DMS_G_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

average_high_ks = 0
average_high_k = 0
average_high_ks_max_infected = 0
average_high_k_max_infected = 0
i = 0
while i < 5:
    high_ks_iterations, high_k_iterations, high_ks_max_infected, high_k_max_infected = run_all_SIR_simulations_for_more_than_one_node(DMS_G_plot, 
        n_plot, DMS_G_plot_low_ks, False)
    if high_ks_iterations > 10 and high_k_iterations > 10:
        average_high_ks += high_ks_iterations / 5
        average_high_k += high_k_iterations / 5
        average_high_ks_max_infected += high_ks_max_infected / 5
        average_high_k_max_infected += high_k_max_infected / 5
        i += 1
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average, and reached a maximum of", 
    average_high_ks_max_infected, "infected nodes on average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average, and reached a maximum of", 
    average_high_k_max_infected, "infected nodes on average")

###### WITH CONNECTED SPREADERS ######
print("Running the simulations for the DMS-Link Duplication network with more than one spreader with connected spreaders")

average_high_ks = 0
average_high_k = 0
for i in range(5):
    high_ks_iterations, high_k_iterations = run_all_SI_simulations_for_more_than_one_node(DMS_G_test, n_test, DMS_G_low_ks, True)
    average_high_ks += high_ks_iterations / 5
    average_high_k += high_k_iterations / 5
print("The network with the highest ks and lower k needed", average_high_ks, "iterations in average")
print("The network with lower ks and higher k needed", average_high_k, "iterations in average")
