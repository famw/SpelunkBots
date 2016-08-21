# Constructing a starting gene
TODO: improve these instructions

## Basic file structure
```
genomestart <genome_id>
<traits>
<nodes>
<genes>
genomeend <genome_id>
```

---

## Trait
`trait <trait_id> <param_1> <param_1> ... <param_9>`

A trait is a group of parameters that can be expressed as a group more than one time. Traits save a genetic algorithm from having to search vast parameter landscapes on every node. Instead, each node can simply point to a trait and those traits can evolve on their own.

## trait_id
Number used to identify the trait. Must be unique.

## param_1 ... param_9
Parameter values.

---

## Node
`node <node_id> <trait_id> <node_type> <node_placement>`

### node_id
Number used to identify the node. Must be unique.

### trait_id
Number used to identify the trait derived by this node. If `0`, the node doesn't use any traits.

### node_type
* **NEURON** [0]: has a list of its incoming input signals.
* **SENSOR** [1]: can be loaded with a value for output.

### node_placement
Used for genetic marking of nodes
* **HIDDEN** [0]
* **INPUT** [1]
* **OUTPUT** [2]
* **BIAS** [3]

---

## Genes
`gene <trait_id> <input_node_id> <output_node_id> <weight> <recurring> <innovation_num> <mutation_num> <enable>`

### trait_id
Number used to identify the gene. Must be unique.

### input_node_id
Number used to identify the gene's input node.

### output_node_id
Number used to identify the gene's output node.

### weight
The weight of the connection between input and output node that represent this gene.

### recurring
Signalizes if the connection is recurrent.

### innovation_num
Used to identify the origins of the gene. Should increase by `1` for each new gene added.

### mutation_num
Used to perform mutations between genes. Always start at `0`.

### enable
Signalizes if this gene is enabled or not.

---

# Example

## XOR
```
genomestart 1
trait 1 0.1 0 0 0 0 0 0 0
trait 2 0.2 0 0 0 0 0 0 0
trait 3 0.3 0 0 0 0 0 0 0
node 1 0 1 3
node 2 0 1 1
node 3 0 1 1
node 4 0 0 2
gene 1 1 4 0.0 0 1 0 1
gene 2 2 4 0.0 0 2 0 1
gene 3 3 4 0.0 0 3 0 1
genomeend 1
```
