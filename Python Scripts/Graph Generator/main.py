# Course: High Performance Computing 2023/2024
#
# Lecturer: Francesco Moscato  fmoscato@unisa.it
#
# Student:
# Pepe Lorenzo        0622702121      l.pepe29@studenti.unisa.it          
# 
#
#                REQUIREMENTS OF THE ASSIGNMENT:
# Design, Implement, and Analyze a parallel version of VF2-pp based on MPI
#  
#  
# Copyright (C) 2024 - All Rights Reserved
#  
# This program is free software: you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
#  See the GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License along with ContestOMP. 
#  If not, see <http://www.gnu.org/licenses/>.

import random
import os
import argparse

# Generate a connected Token Graph
def generate_graph(N, C, path, name):
    if not (0 <= C <= 100):
        raise ValueError("Percentage C must be between 0 and 100")

    max_edges = (N * (N - 1)) // 2
    num_edges = int((C / 100) * max_edges)

    edges = set()

    nodes = list(range(N))
    random.shuffle(nodes)
    for i in range(N - 1):
        edges.add((nodes[i], nodes[i + 1]))

    while len(edges) < num_edges:
        u, v = random.sample(range(N), 2)
        if (u, v) not in edges and (v, u) not in edges:
            edges.add((u, v))

    adjacency_list = {i: set() for i in range(N)}
    for u, v in edges:
        adjacency_list[u].add(v)
        adjacency_list[v].add(u)

    output_lines = [f"{N} {N}"]
    for node in sorted(adjacency_list):
        neighbors = " ".join(map(str, sorted(adjacency_list[node])))
        output_lines.append(f"{node}\t{neighbors}")

    os.makedirs(path, exist_ok=True)
    file_path = os.path.join(path, name)
    with open(file_path, "w") as f:
        f.write("\n".join(output_lines))

    print(f"Graph saved to {file_path}")


def load_graph(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()

    N, _ = map(int, lines[0].split())
    adjacency_list = {i: [] for i in range(N)}

    for line in lines[1:]:
        parts = line.strip().split("\t")
        node = int(parts[0])
        neighbors = list(map(int, parts[1].split())) if len(parts) > 1 else []
        adjacency_list[node] = neighbors

    return N, adjacency_list


def generate_isomorphic_graph(N, X, adjacency_list, path, name):
    permutation = list(range(N))
    random.shuffle(permutation)
    mapping = {old: new for old, new in zip(range(N), permutation)}

    # Create new adjacency list based on mapping
    new_adjacency_list = {mapping[node]: sorted(mapping[neighbor] for neighbor in neighbors)
                          for node, neighbors in adjacency_list.items()}

    output_lines = [f"{N} {N}"]
    for node in sorted(new_adjacency_list):
        neighbors = " ".join(map(str, new_adjacency_list[node]))
        output_lines.append(f"{node}\t{neighbors}")

    UpdFileName = f"{name}_{X}.txt"
    os.makedirs(path, exist_ok=True)
    file_path = os.path.join(path, UpdFileName)
    with open(file_path, "w") as f:
        f.write("\n".join(output_lines))

    print(f"Isomorphic graph #{X} saved to {file_path}")


def create_isomorphic_graph(n, x, input_file, output_path, output_name):
    _, adjacency_list = load_graph(input_file)
    for i in range(x):
        generate_isomorphic_graph(n, i+1, adjacency_list, output_path, output_name)
    
def check_positive(value):
    ivalue = int(value)
    if ivalue <= 0:
        raise argparse.ArgumentTypeError("Number of nodes can't be negative or zero.")
    return ivalue

def check_constraints(value):
    ivalue = int(value)
    if ivalue < 10 or ivalue >100:
        raise argparse.ArgumentTypeError("Coverage must be in the range [10,100]")
    return ivalue

def check_isoValue(value):
    ivalue = int(value)
    if ivalue < 1 or ivalue > 8:
        raise argparse.ArgumentTypeError("Number of isomorphic graph must be in the range [1,8]")
    return ivalue

if __name__ == '__main__':
   
    parser = argparse.ArgumentParser()
    parser.add_argument('--nodes', type=check_positive, default=10)
    parser.add_argument('--coverage', type=check_constraints, default=30)
    parser.add_argument('--isographs', type=check_isoValue, default=1)
    args = parser.parse_args()

    # Directory paths
    fullGraphpath = "./graphs/Token"
    subGraphPath = "./graphs/Pattern"

    # File names
    fullFileName = f"G_n{args.nodes}_c{args.coverage}.txt"
    subFileName = f"G_Iso_n{args.nodes}_c{args.coverage}"

    # Generate Token graph
    generate_graph(args.nodes, args.coverage, fullGraphpath, fullFileName)

    # Path to Token graph
    fullGraphPath = os.path.join(fullGraphpath, fullFileName)

    # Generate Isomorphic Graph
    create_isomorphic_graph(args.nodes, args.isographs, fullGraphPath, subGraphPath, subFileName)
