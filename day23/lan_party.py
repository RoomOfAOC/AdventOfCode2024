import sys
import networkx as nx
from itertools import combinations

data = open(sys.argv[1]).readlines()
G = nx.Graph()
for line in data:
    edges = line.strip().split("-")
    G.add_edge(edges[0], edges[1])

cliques = list(nx.find_cliques(G))
#print(cliques)

inter_connections = set()
for c in cliques:
    if len(c) >= 3 and any(cc[0] == 't' for cc in c):
        for ic in combinations(c, 3):
            if any(v[0] == 't' for v in ic):
                inter_connections.add('-'.join(sorted(ic)))
#print(inter_connections)
print(f'How many contain at least one computer with a name that starts with t?\n{len(inter_connections)}')

LAN = sorted(sorted(cliques, key=len, reverse=True)[0])
#print(LAN)
print(f'What is the password to get into the LAN party?\n{",".join(LAN)}')
