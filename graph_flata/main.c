#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int _;
#define UNUSED(argc) (void)(argc)
typedef enum {False, True} bool;
int min_p;


typedef struct node {
    int vertex;
    bool uniq;
    struct node *next;
} node;

typedef struct Graph {
    int size;
    bool *visited;
    node **points;
} Graph;


node *createNode(int v) {
    node *newNode = malloc(sizeof(node));
    newNode->uniq = False;
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int size) {
    Graph *graph = malloc(sizeof(Graph));
    graph->size = size;
    graph->points = malloc(size * sizeof(node*));
    graph->visited = malloc(size * sizeof(bool));


    for (int i = 0; i < size; i++) {
        graph->points[i] = NULL;
        graph->visited[i] = False;
    }
    return graph;
}

void addEdge(Graph *graph, int first_p, int second_p, bool is_directed) {

    node *newNode = createNode(second_p);
    newNode->next = graph->points[first_p];
    newNode->uniq = True;
    graph->points[first_p] = newNode;


    if (is_directed == False) {
        newNode = createNode(first_p);
        newNode->next = graph->points[second_p];
        newNode->uniq = False;
        graph->points[second_p] = newNode;
    }
}

void DFS(Graph *graph, int start) {
    node *p_list = graph->points[start];
    node *tmp = p_list;

    graph->visited[start] = True;
    printf("  Visited %d \n", start);

    while (tmp != NULL) {
        int connected_v = tmp->vertex;

        if (graph->visited[connected_v] == False) {
            DFS(graph, connected_v);
        }
        tmp = tmp->next;
    }
}
void check(Graph *graph) {
    int counter = 0,counter_v = 0;

    for (int i = 0; i < graph->size; i++) {
        if ((graph->visited[i] == True ) || (graph->points[i])) counter++;
    }

    for (int i = 0; i < graph->size; i++) {
        if (graph->visited[i] == True) counter_v++;
    }

    if (counter == counter_v) printf("\nGraph is connected\n");
    else printf("\nGraph is not connected\n");
}

int fromFile(FILE *input_file, Graph *graph, bool is_directed, bool get_size) {
    fseek(input_file, 0, SEEK_SET);

    int max_p = 0;
    min_p = 1 << 30;

    while (!feof(input_file)) {
        while (isspace(fgetc(input_file)) && !feof(input_file));
        if (feof(input_file)) continue;
        fseek(input_file, -1, SEEK_CUR);

        if ((is_directed == True && fscanf(input_file, "%d--%d",&_, &_))||
        (is_directed == False && fscanf(input_file, "%d->%d",&_,&_))) {
            fseek(input_file, -2, SEEK_CUR);

            int first_p, second_p;
            if (is_directed == True) fscanf(input_file, "%d->%d", &first_p, &second_p);
            else fscanf(input_file, "%d--%d", &first_p, &second_p);

            if (min_p > first_p) min_p = first_p;
            if (min_p > second_p) min_p = second_p;
            if (max_p < first_p) max_p = first_p;
            if (max_p < second_p) max_p = second_p;
            if (get_size == False) addEdge(graph, first_p, second_p, is_directed);
        }
        else {printf("Troubles inside input file"); exit(1);}

    }
    if (get_size == False) {
        return 0;
    }
    else return max_p;
}

void toDot(FILE *output_file, Graph *graph, bool is_directed) {
    if (is_directed == True) fprintf(output_file, "digraph none {");
    else fprintf(output_file, "graph none {");

    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        while (tmp && tmp->uniq == True) {

            if (is_directed == True) fprintf(output_file, "%d->%d;", v, tmp->vertex);
            else fprintf(output_file, "%d--%d;", v, tmp->vertex);
            tmp = tmp->next;
        }
    }
    fputc('}', output_file);
}
void toPNG(FILE* input_file, char* output) {
    char operation[256];
    char dotFile[128];

    fseek(input_file, 0, SEEK_SET);
    fread(dotFile,1,128,input_file);

    snprintf(operation, sizeof(operation), "echo \"%s\" | dot -Tpng >%s", dotFile, output);
    system(operation);



}

void printGraph(Graph *graph) {
    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        if (tmp) printf("\nVertex %d:\n", v);
        while (tmp) {
            printf("Connected to: %d \n", tmp->vertex);
            tmp = tmp->next;
        }
    }
    printf("\n");
}

void deleteGraph(Graph *graph) {
    for (int i = 0; i<=graph->size-1; i++) {
        free(graph->points[i]);
    }
    free(graph->visited);
    free(graph);

}


int main(int argc, char *argv[]) {
    UNUSED(argc);
    bool is_directed;

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w+");
    if (!input_file) {printf("Troubles with opening input file"); exit(1);}
    if (!output_file) {printf("Troubles with opening output file"); exit(1);}

    if (!strcmp(argv[4], "-directed")) is_directed = True;
    else if (!strcmp(argv[4], "-undirected"))is_directed = False;
    else {printf("Invalid arguments"); exit(1);}

    int size = fromFile(input_file, NULL, is_directed, True);
    Graph *graph = createGraph(size+1);

    fromFile(input_file, graph, is_directed, False);
    toDot(output_file, graph, is_directed);
    printGraph(graph);

    printf("DFS start...\n");
    DFS(graph, min_p);
    printf("End of DFS.\n");

    printf("\nChecking of graph connection...");
    check(graph);

    printf("\nPrinting graph to png...\n");
    toPNG(output_file,argv[3]);
    printf("\nGraph printed, please check %s\n", argv[3]);

    fclose(output_file);
    fclose(input_file);
    deleteGraph(graph);
    printf("\nEnd of program.");
    return 0;
}
