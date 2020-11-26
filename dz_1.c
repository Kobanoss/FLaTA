/*
ИУ4-33Б
Кондратив В.О.
5 вариант - задание №1 - реализовать стэк
Делаю все в одном файле, чтобы было проще проверять
*/

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

#include <stdio.h>
#include <stdlib.h>


typedef struct node {
    int value;
    struct node *next;
} node;


void push(node **head, int value) {
    node *tmp = malloc(sizeof(node));
    if (tmp == NULL) {
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

int peek(node* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

int getSize(node *head) {
    int size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}

int pop(node **head) {
    node *out;
    int value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

void printStack(node* head) {
    printf("Stack -> \n");
    while (head) {
        printf("> %d \n", peek(head));
        head = head->next;
    }
}

int main() {
    int max_el;
    node *head = NULL;

    printf("Test func for show realisation.\n");
    printf("Input amount of stack elements:\n");
    scanf("%d", &max_el);
    printf("\n");

    for (int i = 0; i < max_el; i++) {
        push(&head, i);
    }
    printStack(head);
    printf("\nSize of the stack = %d\n\n", getSize(head));

    while (head) {
        printf("Value on head of the stack: %d\n", peek(head));
        printf("Deleted value from the head of the stack: %d\n\n", pop(&head));
    }
    printf("Size of the stack = %d\n", getSize(head));

    printf("\nEnd of program.");
    return 0;
}
