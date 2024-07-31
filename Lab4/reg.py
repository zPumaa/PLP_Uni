import sys

#Function to get input file and return it in dictionary form
def getFile(file):
    nodeDictionary = {}
    with open(file, 'r') as f:
        for line in f:
            numbers = line.strip().split()
            #Check to see if input file contains only integers or not
            try:
                node, *clashes = map(int, numbers)
            except ValueError:
                raise ValueError("Input file should only contain integers")
            nodeDictionary[node] = set(clashes)
    return nodeDictionary


#Function which writes to number a node and its corresponding colour to a file
def writeFile(file, colours):
    with open(file, 'w') as f:
        #Get the ID of each colour
        colour_id = colours.keys()
        #Write the output in ascending order with each node followed by its colour
        for node in sorted(colour_id):
            colour_output = str(node) + str(colours[node]) + '\n'
            f.write(colour_output)

# Sort nodes by number of neighbours in descending order, and then by node ID in ascending order
def sort_nodes(nodeDict):
    nodes = list(nodeDict.keys())
    nodes.sort(key=lambda x: (-len(nodeDict[x]), x))
    return nodes

#Function to correctly assign a colour to each node
def colouring(graph):
    nodes = sort_nodes(graph)
    #nodes = sorted(graph.keys(), key=lambda x: (-len(graph[x]), x)) 
    colours = {}
    #Creating a list of colours that can be used for nodes
    colour_list = list('ABCDEFGHIJKLMNOPQRSTUVWXYZ')[:len(nodes)] # A, B, C, ..., Z
    for node in nodes:
        neighbours = graph[node]
        #Colours used by neighbouring nodes that have been assigned a colour
        colours_in_use = {colours[n] for n in neighbours if n in colours}
        freeColours = set(colour_list) - colours_in_use
        if not freeColours:
            return None # Cannot allocate registers with 26 or less colours
        colours[node] = min(freeColours)
    return colours

if len(sys.argv) != 3:
    print("Usage: python program_name.py input_file output_file")
    sys.exit(1)
input_file = sys.argv[1]; output_file = sys.argv[2]
graph = getFile(input_file)
colours = colouring(graph)
if colours is None:
    print("Cannot allocate registers with 26 or less colours.")
    sys.exit(1)
writeFile(output_file, colours)
print(f"Colours written to {output_file}.")

