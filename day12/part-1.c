#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#define CAVE_NAME_MAX_SIZE 16
#define DEBUG 1
#define FALSE 0
#define TRUE 1

struct cave {
    size_t num_edges;
    char * name;
    struct cave ** edges;
    int is_big;
    int visited;
};

struct cave_graph {
    size_t length;
    struct cave ** nodes; 
};

int index_of(char * str, char c) {
    int index = 0;

    while(str[index]) {
        if (str[index] == c) {
           return index; 
        }

        index++;
    }

    return -1;
}

void print_graph(struct cave_graph * graph) {
    printf("graph vertexes:\n[\n");
    for (int i = 0; i < graph->length; i++) {
       printf("  %s (%u edges) -", graph->nodes[i]->name, graph->nodes[i]->num_edges); 

       for(int j = 0; j < graph->nodes[i]->num_edges; j++) {
            printf(" %s (%d)", graph->nodes[i]->edges[j]->name, j);
       }

       printf("\n");
    }
    printf("]\n");
}

void add_cave_edge(struct cave * cave_a, struct cave * cave_b) {
    size_t index = 0;

    cave_a->num_edges += 1;
    printf("cave_a->num_edges: %d\n", cave_a->num_edges);
    if (cave_a->edges == NULL) {
        printf("Node has no edges... creating...\n");
        cave_a->edges = (struct cave **) malloc(sizeof(struct cave *) * cave_a->num_edges);
    } else {
        cave_a->edges = (struct cave **) realloc(cave_a->edges, sizeof(struct cave *) * cave_a->num_edges);
    }
    index = cave_a->num_edges - 1;

    /*
    printf("##### CAVE A\n");
    printf("- name: %s\n", cave_a->name);
    printf("- num_edges: %d\n", cave_a->num_edges);
    printf("- is_big: %d\n", cave_a->is_big);
    printf("- visited: %d\n", cave_a->visited);
    */
    cave_a->edges[index] = (struct cave  *) malloc(sizeof(struct cave));

    printf("add_cave_edge(): %s -> %s\n", cave_a->name, cave_b->name);
    printf("\tcave_a:\n");
    printf("\t- name: %s\n", cave_a->name);
    printf("\t- num_edges: %d\n", cave_a->num_edges);
    cave_a->edges[index] = cave_b;
    printf("\tadded cave edge (index %d):\n", index);
    printf("\t- name: %s\n", cave_b->name);
    printf("\t- num_edges: %d\n", cave_b->num_edges);
    printf("\tadded cave edge (index %d):\n", index);
    printf("\t- name: %s\n", cave_a->edges[index]->name);
    printf("\t- num_edges: %d\n", cave_a->edges[index]->num_edges);
}

struct cave * add_graph_vertex(struct cave_graph * graph, char * vertex_name) {
    int index = 0;

    if (graph->nodes == NULL) {
        graph->nodes = (struct cave **) malloc(sizeof(struct cave *) * 1);
        graph->length = 1;
    } else {
        for (int i = 0; i < graph->length; i++) {
            if (strcmp(vertex_name, graph->nodes[i]->name) == 0) {
                printf("found node '%s'\n\n", vertex_name);
                return graph->nodes[i];
            }
        }

        index = graph->length;
        graph->length++;

        graph->nodes = (struct cave **) realloc(graph->nodes, sizeof(struct cave *) * graph->length);
    }

    assert(graph->nodes != NULL);
    graph->nodes[index] = (struct cave *) malloc(sizeof(struct cave));
    graph->nodes[index]->is_big = vertex_name[0] >= 'A' && vertex_name[0] <= 'Z';
    graph->nodes[index]->visited = FALSE;
    graph->nodes[index]->num_edges = 0;
    graph->nodes[index]->name = (char *) malloc(sizeof(char) * CAVE_NAME_MAX_SIZE);
    printf("copying name '%s' to node at index %u\n", vertex_name, index);
    memcpy(graph->nodes[index]->name, vertex_name, strlen(vertex_name));
    printf("\t\tcopied name: '%s'\n", graph->nodes[index]->name);
    graph->nodes[index]->edges = NULL;

#if DEBUG
    /*
    printf("vertex '%s' added to graph\n", graph->nodes[index].name);
    printf("- graph length: %u\n", graph->length);
    printf("last added node:\n");
    printf("- is_big: %d\n", graph->nodes[index].is_big);
    printf("- visited: %d\n", graph->nodes[index].visited);
    printf("- name: %s\n", graph->nodes[index].name);
    printf("- num_edges: %d\n\n", graph->nodes[index].num_edges);
    */
#endif

    return graph->nodes[index];
}

void add_graph_edge(struct cave_graph * graph, char * edge) {
    char * start_node = (char *) malloc(sizeof(char) * CAVE_NAME_MAX_SIZE);
    char * end_node = (char *) malloc(sizeof(char) * CAVE_NAME_MAX_SIZE);
    int index_separator = index_of(edge, '-');
    size_t len_str_edge = strlen(edge);

    // the correct here should be if the allocations occurred succesfully.. but whatever...
    memcpy(start_node, edge, index_separator);
    start_node[index_separator] = '\0';

    memcpy(end_node, edge + index_separator + 1, sizeof(char) * (len_str_edge - index_separator - 1));
    end_node[len_str_edge] = '\0';

    struct cave * cave_start = add_graph_vertex(graph, start_node);
    struct cave * cave_end   = add_graph_vertex(graph, end_node);

    printf("-- cave_start->name: '%s'\n", cave_start->name);
    add_cave_edge(cave_start, cave_end);
    print_graph(graph);

    printf("\n\n-- cave_end->name: '%s'\n", cave_end->name);
    add_cave_edge(cave_end, cave_start);
    print_graph(graph);


    printf("###### I WANT SLEEEP!!! num_edges: %u\n", cave_start->num_edges);
}

size_t count_ways_to_end(struct cave * cave) {
    if (cave == NULL) return 0;
    if (strcmp(cave->name, "end") == 0) return 1;

    size_t num_paths = 0;
    struct cave * edge;

    cave->visited = TRUE;

    for (int i = 0; i < cave->num_edges; i++) {
        edge = cave->edges[i];
        if (edge->is_big ||
            (!edge->visited && strcmp(edge->name, "start") != 0)) {
            num_paths += count_ways_to_end(edge);
        }
    }

    cave->visited = FALSE;

    return num_paths;
}


int main() {
    char line[20];
    struct cave_graph graph;

    graph.nodes = NULL;
    graph.length = 0;

    while (scanf("%s", line) != EOF) {
#if DEBUG
        printf("line read: %s\n", line);
#endif
        add_graph_edge(&graph, line);
    }
    
#if DEBUG
    print_graph(&graph);
#endif

    // part 1
    struct cave * start_cave;

    // find 'start' cave
    for(int i = 0; i < graph.length; i++) {
        if (strcmp(graph.nodes[i]->name, "start") == 0) {
            start_cave = graph.nodes[i];
            i = graph.length; // stop loop
        }
    }

    printf("num ways to end: %u\n", count_ways_to_end(start_cave));

    return 0;
}
