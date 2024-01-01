# Banker's Algorithm in C

This repository contains an implementation of the Banker's Algorithm in C, a resource allocation and deadlock avoidance algorithm used in operating systems.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
- [Compilation](#compilation)
- [Tests](#Tests)
- [License](#license)

## Overview

The Banker's Algorithm is a resource allocation and deadlock avoidance algorithm that ensures the safe execution of processes by carefully managing resource requests.

This implementation includes:

- Functions for initializing the system, processes, and resources.
- Functions for simulating the resource allocation and deallocation.
- Implementation of the Banker's Algorithm to check for safe states.

The Banker's algorithm uses a claim graph consisting of processes, multi-unit resources, request edges, allocation edges, and claim edges.

The graph can be represented by a set of arrays:

The number of units of each resource is represented as a one-dimensional array R[m] (Resource vector), where m is the number of resources and each entry R[j] records the number of units of resource Rⱼ. The maximum claims are represented as a two-dimensional array P[n][m] where each entry P[i][j] contains an integer that records the maximum number of units of resource Rⱼ that process pᵢ will ever request. The allocation edges and the request edges are represented using analogous arrays.

Using the given information, the program creates the current representation of the system (the set of arrays).
The program then enters an interactive session during which the user inputs commands of the form:

        request(i, j, k) or release(i, j, k),

where `i` is a process number, `j` is a resource number, and `k` is the number of units of `Rⱼ` the process `pᵢ` is requesting or releasing.
For each request operation, the program responds whether the request has been granted or denied. If granted output a safe execution sequence for the processes.
We can utilize the Banker's algorithm by inputting a sequence of requests and releases that, without using the Banker's algorithm, would lead to a deadlock.

## Usage

To use the Banker's Algorithm in your C program, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/viccon23/bankers-algorithm.git
    ```

2. Use the provided functions to initialize the system, processes, and resources, and to check for safe states.

## Compilation

Compile the source code using a C compiler. For example, using GCC:

  ```bash
  gcc -o bankers_algorithm code.c
  ```

## Tests

Demonstrate the correctness of your program by running the following Maximum Claim matrix, Resource vector and series of requests.

### Scenario 1

Maximum Claim
|  | R0 | R1 | R2 |
|----|---|---|---|
| P0 | 3 | 2 | 2 |
| P1 | 6 | 1 | 3 |
| P2 | 3 | 1 | 4 |
| P3 | 4 | 2 | 2 |

Resource Vector

| R0 | R1 | R2 |
|----|---|---|
| 9 | 3 | 6 |

Resource requests (in order):
* request(0, 0, 1)
* request(1, 0, 6)
* request(2, 0, 2)
* request(1, 1, 1)
* request(2, 1, 1)
* request(1, 2, 2)
* request(2, 2, 1)
* request(3, 2, 2)
* request(0, 2, 1) <-- Should not be granted will move to unsafe state
* request(2, 2, 4) <-- Error requesting more than maximum claim
* request(3, 0, 1) <-- Not granted, not enough resources


### Scenario 2

Maximum Claim
|  | R0 | R1 | R2 | R3 |
|----|---|---|---|---|
| P0 | 5 | 5 | 9 | 5 |
| P1 | 3 | 3 | 2 | 2 |
| P2 | 4 | 4 | 7 | 5 |
| P3 | 2 | 3 | 3 | 3 |
| P4 | 1 | 2 | 5 | 2 |
| P5 | 4 | 4 | 4 | 4 |

Resource Vector

| R0 | R1 | R2 | R3 |
|----|---|---|---|
| 10 | 9 | 15 | 6 |

Resource requests (in order):
* request(0, 0, 1)
* request(1, 0, 1)
* request(2, 0, 2)
* request(3, 0, 1)
* request(5, 0, 1)

* request(0, 1, 2)
* request(1, 1, 1)
* request(5, 1, 1)

* request(0, 2, 2)
* request(2, 2, 4)
* request(3, 2, 1)
* request(4, 2, 1)
* request(5, 2, 1)

* request(1, 3, 1)
* request(2, 3, 1)
* request(4, 3, 1)

* request(5, 3, 3) <-- Should not be granted will cause deadlock
* request(2, 0, 3) <-- Error requesting more than maximum claim
* request(0, 2, 7) <-- Not granted, not enough resources
