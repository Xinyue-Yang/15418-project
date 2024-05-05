# # Converts delaunay graphs from https://www.cc.gatech.edu/dimacs10/archive/delaunay.shtml

import glob, os, random


def convert_delaunay_graph(input_file):
    dir_path = "input/"
    file_path = input_file.split("/")
    graph_name = file_path[2][:-6]
    output_file_name = graph_name + ".network"

    with open(input_file, "r") as f:
        num_vertices, num_edges = map(int, f.readline().split())
        source, sink = 0, num_vertices - 1

        line = f.readline()
        curr_vertex = 0
        edges = []

        while line:
            neighbors = line.split()
            for neighbor in neighbors:
                capacity = random.randint(1, 1000)
                if (
                    curr_vertex != sink
                    and neighbor != source
                    and curr_vertex != neighbor
                ):
                    edges.append((curr_vertex, neighbor, capacity))

            curr_vertex += 1
            line = f.readline()

    with open(os.path.join(dir_path, output_file_name), "w") as f:
        f.write(f"{num_vertices} {source} {sink} {num_edges * 2}\n")
        for edge in edges:
            f.write(f"{edge[0]} {edge[1]} {edge[2]} {0}\n")


def main():
    for file in glob.glob("input/raw_graph_files/*.graph"):
        print(file)
        convert_delaunay_graph(file)


if __name__ == "__main__":
    main()
