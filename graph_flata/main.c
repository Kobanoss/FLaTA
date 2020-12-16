#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define UNUSED(argc) (void)(argc)
typedef enum {
    False, True
} bool;

void swap(int *x_ptr, int *y_ptr) {
    int tmp = *x_ptr;
    *x_ptr = *y_ptr;
    *y_ptr = tmp;
}

int min_p;


typedef struct node {
    int vertex;
    bool uniq;
    struct node *next;
} node;

typedef struct Graph {
    int size;
    bool is_directed;
    bool *visited;
    int *degree;
    node **points;
} Graph;


node *createNode(int v) {
    node *newNode = malloc(sizeof(node));
    newNode->uniq = False;
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int size, bool is_directed) {
    Graph *graph = malloc(sizeof(Graph));
    graph->size = size;
    graph->is_directed = is_directed;
    graph->points = malloc(size * sizeof(node *));
    graph->visited = malloc(size * sizeof(bool));
    graph->degree = malloc(size * sizeof(int));


    for (int i = 0; i < size; i++) {
        graph->points[i] = NULL;
        graph->visited[i] = False;
        graph->degree[i] = 0;
    }
    return graph;
}

void addEdge(Graph *graph, int first_p, int second_p) {

    node *newNode = createNode(second_p);
    newNode->next = graph->points[first_p];
    newNode->uniq = True;
    graph->points[first_p] = newNode;


    if (graph->is_directed == False) {
        newNode = createNode(first_p);
        newNode->next = graph->points[second_p];
        newNode->uniq = False;
        graph->points[second_p] = newNode;
    }
}

void DFS(Graph *graph, int start) {
    node *tmp = graph->points[start];

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
    int counter = 0, counter_v = 0;

    for (int i = 0; i < graph->size; i++) {
        if ((graph->visited[i] == True) || (graph->points[i])) counter++;
    }

    for (int i = 0; i < graph->size; i++) {
        if (graph->visited[i] == True) counter_v++;
    }

    if (counter == counter_v) printf("\nGraph is connected\n");
    else printf("\nGraph is not connected\n");
}

int fromFile(FILE *input_file, Graph *graph, bool get_size) {
    fseek(input_file, 0, SEEK_SET);

    int _;
    int max_p = 0;
    min_p = 1 << 30;

    while (!feof(input_file)) {
        while (isspace(fgetc(input_file)) && !feof(input_file));
        if (feof(input_file)) continue;
        fseek(input_file, -1, SEEK_CUR);

        int step = ftell(input_file);
        if (((graph->is_directed == False) && fscanf(input_file, "%d -- %d", &_, &_)) ||
            ((graph->is_directed == True) && fscanf(input_file, "%d -> %d", &_, &_))) {
            fseek(input_file, step, SEEK_SET);

            int first_p, second_p;
            if (graph->is_directed == True) fscanf(input_file, "%d -> %d", &first_p, &second_p);
            else fscanf(input_file, "%d -- %d", &first_p, &second_p);

            if (min_p > first_p) min_p = first_p;
            if (min_p > second_p) min_p = second_p;
            if (max_p < first_p) max_p = first_p;
            if (max_p < second_p) max_p = second_p;
            if (get_size == False) addEdge(graph, first_p, second_p);
        } else {
            printf("Troubles inside input file");
            exit(130);
        }

    }
    if (get_size == False) {
        return 0;
    } else return max_p;
}

void toDot(FILE *output_file, Graph *graph) {
    if (graph->is_directed == True) fprintf(output_file, "digraph none {");
    else fprintf(output_file, "graph none {");

    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        while (tmp) {
            if (v == tmp->vertex && tmp->uniq == False) {
                tmp = tmp->next;
                continue;
            } else if (tmp->uniq == True) {
                if (graph->is_directed == True) fprintf(output_file, "%d->%d;", v, tmp->vertex);
                else fprintf(output_file, "%d--%d;", v, tmp->vertex);
            }
            tmp = tmp->next;
        }
    }
    fputc('}', output_file);
}

void toPNG(FILE *input_file, char *output) {
    char operation[256];
    char dotFile[128];

    fseek(input_file, 0, SEEK_SET);
    fread(dotFile, 1, 128, input_file);

    snprintf(operation, sizeof(operation), "echo \"%s\" | dot -Tpng >%s", dotFile, output);
    system(operation);

}

void findLoop(Graph *graph) {
    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        int n = 0;
        while (tmp) {
            if (v == tmp->vertex && tmp->uniq == True) n++;
            tmp = tmp->next;
        }
        if (n != 0) printf("Vertex %d is looped %d times\n", v, n);
    }
    printf("\n");

}

void findDegree(Graph *graph) {
    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        int n = 0;
        while (tmp) {
            n++;
            tmp = tmp->next;
        }
        if (n != 0) printf("Vertex %d has %d degree\n", v, n);
        graph->degree[v] = n;
    }
    printf("\n");
}

void quickSort(int *arr, int first, int last) {
    if (first < last) {
        int left = first, right = last, middle = arr[(left + right) / 2];
        do {
            while (arr[left] < middle) left++;
            while (arr[right] > middle) right--;
            if (left <= right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--;
            }
        } while (left <= right);
        quickSort(arr, first, right);
        quickSort(arr, left, last);
    }
}

void degreeSort(Graph *graph) {
    int *degrees = malloc(sizeof(graph->degree));
    for (int v = 0; v < graph->size; v++) degrees[v] = graph->degree[v];
    int points[graph->size];
    for (int i = 0; i < graph->size; i++) points[i] = i;

    quickSort(degrees, 0, graph->size - 1);

    for (int v = 0; v < graph->size; v++) {
        for (int i = 0; i < graph->size; i++) {
            if (degrees[v] == graph->degree[points[i]]) {
                if (degrees[v]!=0 && points[i]!=0) {
                    points[i] = -1;
                    printf("Vertex%d with %d degree\n", i, degrees[v]);
                }
            }
        }
    }
    printf("\n");
}

void printGraph(Graph *graph) {
    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        if (tmp) printf("\nVertex %d:\n", v);
        while (tmp) {
            if (v == tmp->vertex && tmp->uniq == False) {
                tmp = tmp->next;
                continue;
            }
            printf("Connected to: %d \n", tmp->vertex);
            tmp = tmp->next;
        }
    }
    printf("\n");
}

void deleteGraph(Graph *graph) {
    for (int i = 0; i <= graph->size - 1; i++) {
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
    if (!input_file) {
        printf("Troubles with opening input file");
        exit(130);
    }
    if (!output_file) {
        printf("Troubles with opening output file");
        exit(130);
    }

    if (!strcmp(argv[4], "-directed")) is_directed = True;
    else if (!strcmp(argv[4], "-undirected"))is_directed = False;
    else {
        printf("Invalid arguments");
        exit(130);
    }

    Graph *get_size = createGraph(-1, is_directed);
    int size = fromFile(input_file, get_size, True);
    deleteGraph(get_size);
    Graph *graph = createGraph(size + 1, is_directed);

    fromFile(input_file, graph, False);
    toDot(output_file, graph);
    printGraph(graph);
    findLoop(graph);
    printf("Vertex degrees:\n");
    findDegree(graph);
    printf("Quick sorted degree's array:\n");
    degreeSort(graph);

    printf("DFS start...\n");
    DFS(graph, min_p);
    printf("End of DFS.\n");

    printf("\nChecking of graph connection...");
    check(graph);

    printf("\nPrinting graph to png...\n");
    toPNG(output_file, argv[3]);
    printf("\nGraph printed, please check %s\n", argv[3]);

    fclose(output_file);
    fclose(input_file);
    deleteGraph(graph);
    printf("\nEnd of program.");
    return 0;
}
