#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

#define UNUSED(argc) (void)(argc)


typedef struct {
    uint64_t first_point;
    uint64_t second_point;
} edge;

typedef struct {
    edge** edges;
    uint64_t size;
    uint64_t size_of_edges;
} graph;


graph* create_graph () {
    graph* new_graph = (graph*)malloc(sizeof(graph));
    new_graph->size = (uint64_t) 0;
    new_graph->size_of_edges = (uint64_t) 0;
    new_graph->edges = NULL;
    return new_graph;
}

edge* create_edge(uint64_t first_point, uint64_t second_point){
    edge* new_edge = (edge*)malloc(sizeof(edge));
    new_edge->first_point = first_point;
    new_edge->second_point = second_point;
    return new_edge;
}

void add_edge(graph* graph, edge* edge){
    if(graph->size == graph->size_of_edges){
        graph->edges = realloc(graph->edges, (uint64_t) (graph->edges + 1));
        ++(graph->size_of_edges);
    }
    graph->edges[graph->size++] = edge;
}

void check(graph* graph, uint64_t maxp, uint64_t minp){

    bool* ingraph = calloc(sizeof(bool) , maxp - minp + 1);

    for (uint64_t i = 0; i < graph->size; ++i){
        ingraph[graph->edges[i]->first_point - minp] = true;
        ingraph[graph->edges[i]->second_point - minp] = true;
    }
    for (uint64_t i = 0; i <= (maxp - minp); ++i){
        if(!ingraph[i]) {
            printf("Center %" PRIu64 " isn\'t in graph\n", i + minp);

        }
    }
}


void file_parse(graph* graph, FILE* input_file){

    uint64_t maxp = 0;
    uint64_t minp = 1 << 31;

    while(!feof(input_file)){
        while(isspace(fgetc(input_file)) && !feof(input_file));
        if (feof(input_file)) continue;
        fseek(input_file, -1, SEEK_CUR);

        uint64_t first_p, second_p;
        fscanf(input_file, "%llu-%llu", &first_p, &second_p);
        printf("%llu-%llu\n", first_p, second_p);

        if(minp > first_p) minp = first_p;
        if(minp > second_p) minp = second_p;
        if(maxp < first_p) maxp = first_p;
        if(maxp < second_p) maxp = second_p;

        add_edge(graph, create_edge(first_p,second_p));
    }
    check(graph, maxp, minp);
}

void print_dot(graph* graph, FILE* output_file){
    fprintf(output_file, "graph noname {\n");
    for (uint64_t i = 0; i < graph->size; ++i){
        fprintf(output_file, "\t%" PRIu64 " -- %" PRIu64 "\n", graph->edges[i]->first_point, graph->edges[i]->second_point);
    }
    fputc('}', output_file);
}



int main(int argc , char* argv[]){
    UNUSED(argc);

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w+");

    graph* graph = create_graph();

    file_parse(graph,input_file);
    print_dot(graph, output_file);
    fclose(output_file);
    fclose(input_file);
    free(graph);

    return 0;
}