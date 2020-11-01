# Sauron

Distributed Systems 2019-2020, 2nd semester project


## Authors
 
**Group T27**

### Team members


| Number | Name              | User                                  | Email                                   |
| -------|-------------------|---------------------------------------| ----------------------------------------|
| 90764  | Pedro Leitão      | <https://github.com/pedroleitao1999>  | pedro.o.leitao@gmail.com                |
| 84757  | Rafael Cabral     | <https://github.com/raffiti1997>      | rafael.castro.cabral@tecnico.ulisboa.pt |
| 90782  | Tomás Gomes       | <https://github.com/4ThoseAbout2Rock> | tomas.gomes@tecnico.ulisboa.pt          |

### Task leaders  

| Task set | To-Do                         | Leader              |
| ---------|-------------------------------| --------------------|
| core     | protocol buffers, silo-client | Whole team          |
| T1       | cam_join, cam_info, eye       | Pedro Leitão        |
| T2       | report, spotter               | Tomás Gomes         |
| T3       | track, trackMatch, trace      | Rafael Cabral       |
| T4       | test T1                       |                     |
| T5       | test T2                       |                     |
| T6       | test T3                       |                     |


## Getting Started

The overall system is composed of multiple modules.
The main server is the _silo_.
The clients are the _eye_ and _spotter_.

See the [project statement](https://github.com/tecnico-distsys/Sauron/blob/master/README.md) for a full description of the domain and the system.

### Prerequisites

Java Developer Kit 11 is required running on Linux, Windows or Mac.
Maven 3 is also required.

To confirm that you have them installed, open a terminal and type:

```
javac -version

mvn -version
```

### Installing

To compile and install all modules:

```
mvn clean install -DskipTests
```

The integration tests are skipped because they require the servers to be running.


## Built With

* [Maven](https://maven.apache.org/) - Build Tool and Dependency Management
* [gRPC](https://grpc.io/) - RPC framework


## Versioning

We use [SemVer](http://semver.org/) for versioning. 
