

# Реализация предусматривает:

1) Cбор данных из файла, который указывается первым аргументом к исполняемому файлу;

	Данные вводятся в формате:
	
	Вершина_1 -- Вершина_2                                                                                                                     
        Вершина_2 -- Вершина_3                                                                                                                      
        И так далее аналогично;
	
	
		К репозиторию прикреплены файлы none_directed и none_undirected 
			c примером содержимого для ориентированного и неориентированного графа соотвественно.


2) Вывод данных в файл (.dot), который указывается вторым аргументом;

	Данные выводятся согласно синтаксису DOT;

		К репозиторию прикреплен файл none_directed.dot и none_undirected.dot c примером содержимого.


3) Визуализация графа в png-файл, который указывается третьим аргументом;

	Производится помощью утилиты dot из пакета graphviz, вызов команды встроен отельной функцией в программу;

	Ориентированный граф:
	
	![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/none_directed.png)	
	
	Неориентированный граф:
	
	![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/none_undirected.png)




4) Выбор между ориентированным и неориентированным графом, статус графа указывается четвертым аргументом при вызове функции;

	Для получения ориентированного графа в качестве аргумента используется "-directed";
	
	Для получения неориентированного графа в качестве аргумента используется "-undirected";
	
	
	При несовпадении типа графа и содержимого файла выдается ошибка и действие программы завершается с кодом (1).


# Дополнительное задание: 
	 8. Найти все петли

## Алгоритм: 
- Проход по каждому узлу и определение, является ли связанная с ним вершина им самим		
``` C
void findLoop(Graph *graph) {
    for (int v = 0; v < graph->size; v++) {
        node *tmp = graph->points[v];
        int n = 0;
        while (tmp) {
            if (v == tmp->vertex && tmp->uniq == True) n++;
            tmp = tmp->next;
        }
        if (n!=0) printf("Vertex %d is looped %d times\n", v, n);
    }
    printf("\n");

}
```
## Структурная схема алгоритма: 
![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/find_loop_upd.png)


# Дополнительное задание:
	2.Отсортировать все вершины по возрастанию степени вершины (быстрая сортировка)

## Алгоритм нахождения степеней вершины: 
``` C
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
```
## Структурная схема алгоритма: 
![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/find_degree_upd.png)

## Алгоритм быстрой сортировки(quickSort): 
``` C
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
```
## Визуализация алгоритма сортировки:
![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/quick_sort.gif)
## Структурная схема алгоритма: 
![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/quick_sort_upd.png)

## Вывод отсортированного списка вершин с их степенью(степенью выхода для ориентированного графа): 
``` C
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
```



# Пример вызова и работы в терминале: 

![Alt-текст](https://github.com/Kobanoss/FLaTA/blob/master/graph_flata/term.png)


Copyright © 2020 by KondrativVO. 

All Rights Reserved.
