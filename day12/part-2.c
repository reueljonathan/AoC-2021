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
    int num_visits;
};

struct cave_graph {
    size_t length;
    int node_visited_twice;
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

    cave_a->edges[index] = (struct cave  *) malloc(sizeof(struct cave));
    cave_a->edges[index] = cave_b;
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
    graph->nodes[index]->num_visits = 0;
    graph->nodes[index]->num_edges = 0;
    graph->nodes[index]->name = (char *) malloc(sizeof(char) * CAVE_NAME_MAX_SIZE);
    memcpy(graph->nodes[index]->name, vertex_name, strlen(vertex_name));
    graph->nodes[index]->edges = NULL;

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

    printf("###### Created start node:\n");
    printf("- name: '%s'\n", cave_start->name);
    printf("- num edges: '%d'\n", cave_start->num_edges);

    printf("-- cave_start->name: '%s'\n", cave_start->name);
    add_cave_edge(cave_start, cave_end);
    print_graph(graph);

    printf("\n\n-- cave_end->name: '%s'\n", cave_end->name);
    add_cave_edge(cave_end, cave_start);
    print_graph(graph);

}

size_t count_ways_to_end(struct cave_graph * graph, struct cave * cave) {
    if (cave == NULL) return 0;
    if (strcmp(cave->name, "end") == 0) return 1;

    size_t num_paths = 0;
    struct cave * edge;

    cave->num_visits++;

    if (cave->num_visits == 2 && !cave->is_big) {
        graph->node_visited_twice = TRUE;
    }

    for (int i = 0; i < cave->num_edges; i++) {
        edge = cave->edges[i];

        if (strcmp(edge->name, "start") != 0) {

            if (edge->is_big || edge->num_visits == 0) {
                num_paths += count_ways_to_end(graph, edge);
            } else if (edge->num_visits == 1 && !graph->node_visited_twice) {
                num_paths += count_ways_to_end(graph, edge);
            }
        }
    }

    cave->num_visits--;

    if (cave->num_visits == 1 && !cave->is_big) {
        graph->node_visited_twice = FALSE;
    }

    return num_paths;
}


int main() {
    char line[20];
    struct cave_graph graph;

    graph.nodes = NULL;
    graph.length = 0;
    graph.node_visited_twice = FALSE;

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

    printf("num ways to end: %u\n", count_ways_to_end(&graph, start_cave));

    return 0;
}
