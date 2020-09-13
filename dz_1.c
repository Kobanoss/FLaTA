// ИУ4-33Б
// Кондратив В.О.
// 5 вариант - задание №1 - реализовать стэк
// Делаю все в одном файле, чтобы было проще проверять

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// В основу лепим обычный односвязный список

typedef struct Node_tag {
    int value;
    struct Node_tag *next;
} Node_t;


void push(Node_t **head, int value) {
    Node_t *tmp = malloc(sizeof(Node_t));
    if (tmp == NULL) {
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

// функция pop в ее классическом представлении - удаляет элемент на котором стоит указатель, возвращая его значение
// Перекидывая указатель head на следующий элемент

int pop(Node_t **head) {
    Node_t *out;
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

// Обычный peek - возвращает значение с head'a

int peek(const Node_t* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

// Пробежка по ноде и вывод всего стэка

void printStack(const Node_t* head) {
    printf("Stack -> \n");
    while (head) {
        printf("> %d \n", peek(head));
        head = head->next;
    }
}

// Пробежка по ноде и подсчет кол-ва элементов

size_t getSize(const Node_t *head) {
    size_t size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}


int main() {
    int i;
    int max_el;
    Node_t *head = NULL;
    printf("Test func for show realisation.\n\n");
    printf("Input amount of stack elements: ");
    scanf("%d", &max_el);
    printf("\n");
    for (i = 0; i < max_el; i++) {
        push(&head, i);
    }
    printStack(head);
    printf("\nSize of the stack = %zu\n\n", getSize(head));
    while (head) {
        printf("Value on head of the stack: %d\n", peek(head));
        printf("Deleted value from the head of the stack: %d\n\n", pop(&head));
    }

    printf("Size of the stack = %zu\n", getSize(head));

    return 0;
}
