Библиотека элементов программного интерфейса для обработки графов
=================================================================



Принципы обработки графовых моделей для системы с набором дискретной математики
===============================================================================

# ВВЕДЕНИЕ

В качестве основного обрабатывающего блока современных ЭВМ
выступает арифметико-логическое устройство. Вместе с тем, при решении
практических задач используется существенно большее количество
математических операций, включая операции над множествами в дискретной математике.

В МГТУ им. Н.Э. Баумана проведен полный цикл создания
принципиально новой универсальной вычислительной системы, начиная от
создания принципов и моделей и заканчивая созданием опытного образца,
проведения тестов и испытаний [2]. Разработано принципиально новое
вычислительное устройство: Процессор с набором команд дискретной
математики (Процессор обработки структур, далее СП), реализующее набор
команд дискретной математики высокого уровня над множествами и
структурами данных. Новая архитектура позволяет более эффективно
решать задачи дискретной оптимизации, основанные на моделях множеств,
графов, и отношений.

В данной работе будет произведена разработка моделей представления различных
типов графов для систем с набором дискретной математики.



# Набор команд дискретном математики

Микропроцессор Leonhard x64 хранит информацию о множествах в виде неперекрывающихся 
B+ деревьев. Последняя версия набора команд Leonhard x64 была расширена
двумя новыми инструкциями (NSM и NGR) для обеспечения требований некоторых
алгоритмов. Каждая инструкция набора включает до трех операндов (таблица 1):

Таблица 1 - Формат данных Leonhard x64

| Структура | Ключ    | Значение |
| --------- | ------- | -------- |
| 3 бита    | 64 бита | 64 бита  |


Набор команд состоит из 20 высокоуровневых кодов операций, перечисленных ниже.
- **Search (SRCH)** выполняет поиск значения, связанного с ключом.
- **Insert (INS)** вставляет пару ключ-значение в структуру. SPU обновляет значение,
если указанный ключ уже находится в структуре.
- **Операция Delete (DEL)** выполняет поиск указанного ключа и удаляет его из
структуры данных.
- **Neighbors (NSM, NGR)** выполняют поиск соседнего ключа, который меньше (или больше) 
заданного и возвращает его значение. Операции могут быть использованы
для эвристических вычислений, где интерполяция данных используется вместо
точных вычислений (например, кластеризация или агрегация).
- **Maximum /minimum (MAX, MIN)** ищут первый или последний ключи в структуре данных.
- **Операция Cardinality (CNT)** определяет количество ключей, хранящихся в структуре.
- **Команды AND, OR, NOT** выполняют объединения, пересечения и дополнения в
двух структурах данных.
- **Срезы (LS, GR, LSEQ, GREQ)** извлекают подмножество одной структуры данных в другую.
- **Переход к следующему или предыдущему (NEXT, PREV)** находят соседний
(следующий или предыдущий) ключ в структуре данных относительно переданного
ключа. В связи с тем, что исходный ключ должен обязательно присутствовать в
структуре данных, операции NEXT/PREV отличаются от NSM/NGR.
- **Удаление структуры (DELS)** очищает все ресурсы, используемые заданной структурой.
- **Команда Squeeze (SQ)** дефрагментирует блоки памяти DSM, используемые структурой.
- **Команда Jump (JT)** указывает SPU код ветвления, который должен быть
синхронизирован с CPU (команда доступна только в режиме MISD).



# Анализ требований, предъявляемых к графовым моделям для систем с набором дискретной математики

Самое главное требование, предъявляемое к моделям является, что графы, 
построенные на базе этой модели, должны соответствовать всем свойствам данного типа графа.
Также должен осуществляться эффективный поиск смежных вершин. Если граф взвешанный, то по модели
должен осуществляться поиск минимальных и максимальных ребер.

Для осуществления эффективного поиска, важно правильно задавать порядок аргументов в ключе.
Наиболее приорететные аргумнты для поиска должны находится в старших разрядах ключа.



# Графовые модели для системы с набором дискретной математики


## 1 Модель для ориентированного остовного графа

Данная модель подходит для невзваешанных ориентированных остовных графов и 
обычных ориентированных графов, у которых нельзя двумя и более ребрами 
соединить две одинаковых вершины. 
Модель использует всего одну SPU структуру:

```
| Ключ                            | Значение       |
| ------------------------------- | -------------- |
| id вершины | id смежной вершины |                |
| -----------| -------------------| -------------- |
| id вершины | 0..0               | Данные вершины | 
```

Структура хранит смежные вершины. 
Старший аргумент ключа является идентификатор вершины, 
а младший - идентификатор смежной вершины. 
Данные вершины хранится в значении для записи с ключом, 
у которого аргумент `id вершины` соответствует этой вершине 
и аргумент `id смежной вершины` равен 0.

Для неориентированного остовного графа необходимо каждое ребро представлять 
дважды относительно входящих в него вершин. 

**Ограничения:** 
1. Идентификаторы вершин не должны быть равны 0.
2. Нельзя двумя и более ребрами соединить две одинаковых вершины.


## 2 Модель для взвешанного оринтированного остовного графа

Данная модель подходит для взваешанных ориентированных остовных графов. 
Модель использует всего одну SPU структуру:

```
| Ключ                                   | Значение       |
| ---------- + ---- + ------------------ + -------------- |
| id вершины | Вес  | id смежной вершины |                |
| ---------- + ---- + ------------------ + -------------- |
| id вершины | 0..0                      | Данные вершины |
| id вершины | 0..0 | id смежной вершины | Вес ребра      | 
```

Структура хранит смежные вершины. 
Старший аргумент ключа является идентификатор вершины, 
второй - вес ребра,
а младший - идентификатор смежной вершины. 
Данные вершины хранится в значении для записи с ключом, 
у которого аргумент `id вершины` соответствует этой вершине 
и аргумент `id смежной вершины` равен 0.

Для нахождения веса ребра между вершинами необходимо добавлять запись, в которой 
аргументы `id вершины` и `id смежной вершины` соответствуют смежным вершинам
аргумент `Вес` равен 0, 
а значение равно весу ребра.

Для неориентированного остовного графа необходимо каждое ребро представлять 
дважды относительно входящих в него вершин. 

Отличие данной модели от предыдущей - в SPU структуру был добавлен аргумент `Вес ребра`,
благодаря которому, будет осуществлятся быстрый поиск смежной вершины
соединенными через ребро с минимальным или максимальным весом.

**Ограничения:** 
1. Идентификаторы вершин не должны быть равны 0.
2. Вес ребра представлен натуральным числом.


## 3 Модель для ориентированного графа

Модель подходит для ориентированных графов 
и использует одну структуру SPU: 

```
| Ключ                                         | Значение             |
| -------------------------------------------- | -------------------- |
| id вершины | id ребра | id смежной вершины   |                      |
| -----------| ------------------------------- | -------------------- |
| 0..0       | id ребра | id смежной вершины 1 | id смежной вершины 2 | 
| id вершины | 0..0                            | Данные вершины       | 
```

Структура хранит смежные вершины. 
Старший аргумент ключа является идентификатор вершины, 
второй - идентификотор ребра,
а младший - идентификатор смежной вершины. 
Данные вершины хранится в значении для записи с ключом, 
у которого аргумент `id вершины` соответствует этой вершине, 
а остальные аргументы равны 0.

Для поиска по идентификатору вершины необходимо добавлять запись, в которой 
аргумент `id вершины` равен 0, 
`id ребра` соответствует ребру, 
а младший аргумент и значение соответствует идентификаторам смежных вершин.
Поиск такой записи будет происходить с помощью команды `ngr(0, id, 0)`.

Для неориентированного графа необходимо каждое ребро представлять 
дважды относительно входящих в него вершин. 

**Ограничения:** Идентификаторы вершин не должны быть равны 0.



## 4 Модель для взвешанного ориентированного графа

Модель подходит для взваешанных ориентированных графов 
и использует одну структуру SPU: 

```
| Ключ                                               | Значение             |
| ---------- + --- + -------- + -------------------- + -------------------- |
| id вершины | Вес | id ребра | id смежной вершины   |                      |
| ---------- + --- + -------- + -------------------- + -------------------- |
| 0..0             | id ребра | id смежной вершины 1 | id смежной вершины 2 | 
| id вершины | 0..0                                  | Данные вершины       | 
```

Структура хранит смежные вершины. 
Старший аргумент ключа является идентификатор вершины, 
второй - вес ребра,
третий - идентификотор ребра,
а младший - идентификатор смежной вершины. 
Данные вершины хранится в значении для записи с ключом, 
у которого аргумент `id вершины` соответствует этой вершине, 
а остальные аргументы равны 0.

Отличие данной модели от предыдущей - в SPU структуру был добавлен аргумент `Вес ребра`,
благодаря которому, будет осуществлятся быстрый поиск смежной вершины,
соединенной ребом с минимальным / максимальным весом.

Для поиска по идентификатору вершины необходимо добавлять запись, в которой 
аргумент `id вершины` и `Вес` равны 0, 
`id ребра` соответствует ребру, 
а младший аргумент и значение соответствует идентификаторам смежных вершин.
Поиск такой записи будет происходить с помощью команды `ngr(0, 0, id, 0)`.

**Ограничения:** 
1. Идентификаторы вершин не должны быть равны 0.
2. Вес ребра представлен целым положительным числом.



## 5 Модель для гиперграфа

Данная модель подходит для гиперграфов и использует 2 структуры SPU:

1.  **Связь вершина -> ребро.** 

    ```
    | Ключ                  | Значение       |
    | --------------------- | -------------- |
    | id вершины | id ребра |                |
    | -----------| ---------| -------------- |
    | id вершины | 0..0     | Данные вершины | 
    ```

    Структура хранит вершины и исходящие из них ребра. 
    По этой структуре происходит поиск смежных ребер для указанной вершины.
    Старший аргумент ключа является идентификатор вершины, 
    а младший - идентификатор смежного ребра. 
    Данные вершины хранится в значении для записи с ключом, 
    у которого аргумент `id вершины` соответствует этой вершине 
    и аргумент `id ребра` равен 0.

2.  **Связь ребро -> вершина.**

    ```
    | Ключ                  | Значение  |
    | --------------------- | --------- |
    | id ребра | id вершины |           |
    | ---------| -----------| --------- |
    | id ребра | 0..0       | Вес ребра | 
    ```
    
    Структура хранит ребра и вершины, в которые входят эти ребра. 
    По этой структуре происходит поиск смежных вершин для указанного ребра.
    Старший аргумент ключа является идентификатор ребра, 
    а младший - идентификатор смежной вершины. 
    
    Для взвешанных графов в значении для записи с ключом, 
    у которого аргумент `id ребра` соответствует указанному ребру,
    а аргумент `id вершины` равен 0, значение будет соответствовать весу ребра.
    Таким образом, модель будет обеспечивать быстрое изменение веса ребра,
    однако, чтобы получить ребро с минимальны весом, придется вытащить все ребра
    и найти среди них ребро с минимальным весом.

Может показаться, что если в первой структуре размещать исходящие из вершины ребра,
а во второй - входящие в вершину ребра, то модель будет подходить для ультрографов.
Однако, в таком случае нельзя по вершине будет найти все входящие в неё ребра.

**Ограничения:** Идентификаторы вершины и ребра не должны быть равны 0.



## 6 Модель для ультраграфа

Данная модель подходит для ультрарафов и использует 2 структуры SPU:

1.  **Связь вершина -> ребро.** 

    ```
    | Ключ                                     | Значение       |
    | ---------------- | ---------- | -------- | -------------- |
    | Бит инцидетности | id вершины | id ребра |                |
    | ---------------- | ---------- | -------- | -------------- |
    | 0                | id вершины | 0..0     | Данные вершины | 
    ```

    Структура хранит вершины и исходящие из них ребра. 
    По этой структуре происходит поиск смежных ребер для указанной вершины.
    Старший бит ключа (`Бит инцидетности`) показывает является ли 
    данное ребро входящим или исходящим (0 - входящее, 1 - исходящее),
    второй аргумент ключа является идентификатор вершины, 
    а младший аргумент - идентификатор смежного ребра. 
    Данные вершины хранится в значении для записи с ключом, 
    у которого аргумент `id вершины` соответствует этой вершине 
    и аргумент `id ребра` равен 0.

2.  **Связь ребро -> вершина.**

    ```
    | Ключ                                     | Значение  |
    | ---------------- | -------- | ---------- | --------- |
    | Бит инцидетности | id ребра | id вершины |           |
    | ---------------- | -------- | ---------- | --------- |
    | 0                | id ребра | 0..0       | Вес ребра | 
    ```
    
    Структура хранит ребра и вершины, в которые входят эти ребра. 
    По этой структуре происходит поиск смежных вершин для указанного ребра.
    Старший бит ключа (`Бит инцидетности`) показывает является ли 
    данное ребро входящим или исходящим (0 - входящее, 1 - исходящее),
    второй аргумент ключа является идентификатор ребра, 
    а младший аргумент - идентификатор смежной вершины. 
    
    Для взвешанных графов в значении для записи с ключом, 
    у которого аргумент `id ребра` соответствует указанному ребру,
    а аргумент `id вершины` равен 0, значение будет соответствовать весу ребра.
    Таким образом, модель будет обеспечивать быстрое изменение веса ребра,
    однако, чтобы получить ребро с минимальны весом, придется вытащить все ребра
    и найти среди них ребро с минимальным весом.
    
**Ограничения:** Идентификаторы вершины и ребра не должны быть равны 0.



## 7 Модель для взвешанного ультраграфа

Данная модель подходит для взвешанных ультрарафов и использует 2 структуры SPU:

1.  **Связь вершина -> ребро.** 

    ```
    | Ключ                                                 | Значение               |
    | ---------------- + ---------- + --------- + -------- + ---------------------- |
    | Бит инцидетности | id вершины | Вес ребра | id ребра |                        |
    | ---------------- + ---------- + --------- + -------- + ---------------------- |
    | 0                | id вершины | 0..0                 | Данные вершины         | 
    | 0                | id вершины | 1..1                 | Кол-во входящих ребер  | 
    | 1                | id вершины | 1..1                 | Кол-во исходящих ребер |
    | 0                | 0..0                              | Общее кол-во вершин    |  
    ```

    Структура хранит вершины и исходящие из них ребра. 
    По этой структуре происходит поиск смежных ребер для указанной вершины.
    Старший бит ключа (`Бит инцидетности`) показывает является ли 
    данное ребро входящим или исходящим (0 - входящее, 1 - исходящее),
    второй аргумент ключа является идентификатор вершины, 
    третий - вес ребра,
    а младший аргумент - идентификатор смежного ребра. 
    Данные вершины хранится в значении для записи с ключом, 
    у которого аргумент `id вершины` соответствует этой вершине 
    и аргумент `id ребра` равен 0.
    
    Чтобы не пересчитывать количество смежных ребер предлагается для записи 
    с битом инцидетности равным 0 и старшими аргументами (`Вес ребра` и `id ребра`),
    у которых биты равны 1, хранить количество входящих ребер, а для записи 
    с битом инцидетности равным 1 хранить количество исходящих ребер.
    
    По адресу 0 хранится общее количество вершин.

2.  **Связь ребро -> вершина.**

    ```
    | Ключ                                     | Значение                             |
    | ---------------- + -------- + ---------- + ------------------------------------ |
    | Бит инцидетности | id ребра | id вершины |                                      |
    | ---------------- + -------- + ---------- + ------------------------------------ |
    | 0                | id ребра | 0..0       | Вес ребра                            | 
    | 0                | id ребра | 1..1       | Кол-во вершин, в кот. входит ребро   | 
    | 1                | id ребра | 1..1       | Кол-во вершин, из кот. выходит ребро |
    | 0                | 0..0                  | Общее количество ребер               | 
    ```
    
    Структура хранит ребра и вершины, в которые входят эти ребра. 
    По этой структуре происходит поиск смежных вершин для указанного ребра.
    Старший бит ключа (`Бит инцидетности`) показывает является ли 
    данное ребро входящим или исходящим (0 - входящее, 1 - исходящее),
    второй аргумент ключа является идентификатор ребра, 
    а младший аргумент - идентификатор смежной вершины. 
    
    Для взвешанных графов в значении для записи с ключом, 
    у которого аргумент `id ребра` соответствует указанному ребру,
    а аргумент `id вершины` равен 0, значение будет соответствовать весу ребра.
    
    Чтобы не пересчитывать количество смежных вершин предлагается для записи 
    с битом инцидетности равным 0 и старшими аргументами (`Вес ребра` и `id ребра`),
    у которых биты равны 1, хранить количество вершин, из которых выходит ребро,
    а для записи с битом инцидетности равным 1 хранить количество вершин, 
    в которое входит ребро.
    
    По адресу 0 хранится общее количество ребер.

Отличие данной модели от предыдущей - в первую SPU структуру был добавлен аргумент `Вес ребра`,
благодаря которому, будет осуществлятся быстрый поиск ребер вершины 
с минимальным / максимальным весом. Однако для изменения веса у ребра 
придется пробежаться по всем связям *вершина -> ребро* и у каждой изменить вес.
    
**Ограничения:** 
1. Идентификаторы вершины и ребра не должны быть равны 0, 
а также все биты идентификаторов не должны быть равными 1.
2. Вес ребра должен быть целым положительным числом.

Эта графовая модель для системы с набором дискретной математики наиболее универсальна
и будет выбрана в качестве реализации в библиотеки элементов программного интерфейса 
для обработки графов.



# ЗАКЛЮЧЕНИЕ

В результате проеделанной работы были разработаны модели представления различных типов графов 
для систем с дискретным набором команд. Наиболее перспективная модель в качестве реализации 
является модель для взвешанного ультрографа, т.к. она является наиболее универсальная, 
с помощью её можно построить любой граф. А также в данной модели наиболее эффективный поиск ребер 
и вершин. Но в расплату за хороший поиск минимальных и максимальных ребер приходится 
расплачиваться относительно долгим изменением веса у ребер.
