Библиотека элементов программного интерфейса для обработки графов
=================================================================

Был реализован интерфейс для создания ультраграфов для 
систем с дискретным набором команд. 
Библеотека реализует интерфейс графа предоставляемой библиотекой 
**Boost graph library**. 
Т.о. для программ использующих графы предоставляемый библиотекой boost, 
можно легко перейти на этот граф, просто подменив их. 
Также реализовав интерфейс графа boost, 
появляется возможность использовать множество его графовых алгоритмов.


## Структуры для хранения ультраграфа

Для хранения ультраграфа используется следующие 2 структуры:

1.  **Структура связь вершина -> ребро.** 

    ```
    | Ключ                                                | Значение               |
    | -------- + ---------------- + ---------- + -------- + ---------------------- |
    | id графа | Бит инцидетности | id вершины | id ребра |                        |
    | -------- + ---------------- + ---------- + -------- + ---------------------- |
    | id графа | 0                | 0..0                  | Общее кол-во вершин    |  
    | id графа | 0                | id вершины | 0..0     | Данные вершины         | 
    | id графа | 0                | id вершины | 1..1     | Кол-во исходящих ребер | 
    | id графа | 1                | id вершины | 1..1     | Кол-во входящих ребер  |
    ```
    
    Структура хранит вершины и исходящие из них ребра. 
    По этой структуре происходит поиск смежных ребер для указанной вершины.
    Старший аргумент  - идентификатор графа
    Аргумент `Бит инцидетности` показывает является ли 
    данное ребро входящим или исходящим (0 - исходящее, 1 - входящее),
    третий аргумент ключа является идентификатор вершины, 
    а младший аргумент - идентификатор смежного ребра. 
    Данные вершины хранятся в значении для записи с ключом, 
    у которого аргумент бит инцидетности рваен 0, 
    аргумент `id вершины` соответствует этой вершине 
    и аргумент `id ребра` равен 0.

2.  **Структура связь ребро -> вершина.**

    ```
    | Ключ                                                | Значение                             |
    | -------- + ---------------- + -------- + ---------- + ------------------------------------ |
    | id графа | Бит инцидетности | id ребра | id вершины |                                      |
    | -------- + ---------------- + -------- + ---------- + ------------------------------------ |
    | id графа | 0                | 0..0                  | Общее количество ребер               | 
    | id графа | 0                | id ребра | 0..0       | Данные ребра                         | 
    | id графа | 0                | id ребра | 1..1       | Кол-во вершин, из кот. выходит ребро | 
    | id графа | 1                | id ребра | 1..1       | Кол-во вершин, в кот. входит ребро   |
    ```

    Структура хранит ребра и вершины, в которые входят эти ребра. 
    По этой структуре происходит поиск смежных вершин для указанного ребра.
    Аргумент `Бит инцидетности` показывает является ли 
    данное ребро входящим или исходящим (0 - исходящее, 1 - входящее),
    третий аргумент ключа является идентификатор ребра, 
    а младший аргумент - идентификатор смежной вершины. 
    К ребру можно прикрепить информацию о ребре. 
    Эти данные хранятся в значении для записи с ключом, 
    у которого аргумент бит инцидетности рваен 0, 
    аргумент `id ребра` соответствует этому ребру,
    и аргумент `id вершины` равен 0.

При поиске имеет та вершина или ребро, что имеет меньший id. 
Т.о. для взвешенных графов следует формировать id ребра так, 
чтобы в старших битах идентификатора ребра находился вес этого ребра
(для формирования такого идентификатора имеется вспомогательный метод 
`edge_descriptor get_edge_descriptor(id_t edge_id, weight_t weight);`).


## Пример работы с SpuUltraGraph

```c++
#include "SpuUltraGraph.h"

int main() {
    return 0;
}
```
