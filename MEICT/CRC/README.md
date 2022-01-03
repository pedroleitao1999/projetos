# Network Science

Network Science 2021-2022, Most efficient information spreaders in a network

## Authors
 
**Group 36**

### Team members

| Number | Name              |
| -------|-------------------|
| 90764  | Pedro Leitão      |
| 90782  | Tomás Gomes       |
| 93844  | João Sá           |

## Overview of the code

The code was written in Visual Studio Code and run in its integrated terminal.

It uses diverse functions created by us to simulate the propagation of a rumor in different models (SI, SIS, SIR and Competing Rumors).

Since we run several different simulations in networks with a high number of nodes, it is useful to comment the simulations that we don't want to run (in the case were we just want to run a specific simulation) in order to avoid a long runtime. For this reason, it is also useful to comment the content of the draw() function if we don't want to create any image of the graph at a certain iteration or generate a gif with the propagation in the graph.

## Configuration and installation instructions

The file with the code that we used to do our project is project.py and uses several libraries that should be installed before running the program.

To install those libraries, we write in the terminal the pip install command for each library:

pip install networkx

pip install matplotlib

pip install imageio
