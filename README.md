Библиотека элементов программного интерфейса для обработки графов
=================================================================

Принципы обработки графовых моделей для системы с набором дискретной матиматики
===============================================================================

# Графовые модели для системы с набором дискретной матиматики

## 1 Модель для ультраграфа

Данная модель подходит для ультрографов и использует 2 структуры SPU:

1.  **Связь вершина -> ребро.** 

    ```
    | Ключ                  | Значение       |
    | --------------------- | -------------- |
    | id вершины | id ребра |                |
    | -----------| ---------| -------------- |
    |            | 0..0     | Данные вершины | 
    ```

    Структура хранит вершины и исходящие из них ребра. 
    По этой структуре происходит поиск исходящих из вершины ребер.
    Старший аргумент ключа является идентификатор вершины, 
    а младший - идентификатор исходящего ребра. 
    Данные вершины хранится в значении для записи с ключом, 
    у которого аргумент `id вершины` соответствует этой вершине 
    и аргумент `id ребра` равен 0.

2.  **Связь ребро -> вершина.**

    ```
    | Ключ                  | Значение  |
    | --------------------- | --------- |
    | id ребра | id вершины |           |
    | ---------| -----------| --------- |
    |          | 0..0       | Вес ребра | 
    ```
    
    Структура хранит ребра и вершины, в которые входят эти ребра. 
    По этой структуре происходит поиск вершин, в которое входит указанное ребро.
    Старший аргумент ключа является идентификатор ребра, 
    а младший - идентификатор вершины, в которое входит данное ребро. 
    
    Для взвешенного графа в значении для записи с ключом, 
    у которого аргумент `id ребра` соответствует указанному ребру,
    а аргумент `id вершины` равен 0, значение будет соответствовать весу ребра.
    
**Ограничения:** Идентификаторы вершины и ребра не должны быть равны 0.