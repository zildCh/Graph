
#ifndef AGLRGZ_GRAPH_H
#define AGLRGZ_GRAPH_H

#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <map>
#include <unordered_set>
#include <stack>

using namespace std;

//Дескриптор вершины графа
template<class NameT, class DataT>
class Vertex {
    NameT name;
    DataT data;
    int index;
    bool nameSet;
    bool dataSet;

public:
    Vertex(int index) :
        nameSet(false),
        dataSet(false),
        index(index) {}

    Vertex(NameT name, DataT data, int index) :
        name(name),
        data(data),
        nameSet(true),
        dataSet(true),
        index(index) {}

    void setName(NameT _name) {
        name = _name;
        nameSet = true;
    }

    void setData(DataT _data) {
        data = _data;
        dataSet = true;
    }

    NameT getName() {
        return name;
    }

    DataT getData() {
        return data;
    }

    bool operator==(const Vertex<NameT, DataT>& other) {
        if (nameSet != other.nameSet)
            return false;
        if (dataSet != other.dataSet)
            return false;
        if (nameSet)
            if (name != other.name)
                return false;
        if (dataSet)
            if (data != other.data)
                return false;
        return true;
    }

    bool operator<(const Vertex<NameT, DataT>& other) {
        return (this->index < other.index);
    }

    bool isNameSet() {
        return nameSet;
    }

    bool isDataSet() {
        return dataSet;
    }
};

//Дескриптор ребра графа
template<class VertexT, class WeightT, class DataT>
class Edge {
    VertexT* v1, * v2;   //Вершины, которые соединяет ребро
    WeightT weight;     //Вес
    DataT data;         //Данные ребра
    bool weightSet, dataSet;

public:
    //Конструкторы
    Edge(VertexT* v1, VertexT* v2) :
        v1(v1),
        v2(v2),
        weightSet(false),
        weight(0),
        dataSet(false) {}

    Edge(VertexT* v1, VertexT* v2, WeightT weight) :
        v1(v1),
        v2(v2),
        weight(weight),
        weightSet(true),
        dataSet(false) {}

    Edge(VertexT* v1, VertexT* v2, WeightT weight, DataT data) :
        v1(v1),
        v2(v2),
        weight(weight),
        data(data),
        weightSet(true),
        dataSet(true) {}

    //Установка и получение веса и данных
    void setWeight(WeightT weight) {
        this->weight = weight;
        weightSet = true;
    }

    void setData(DataT data) {
        this->data = data;
        dataSet = true;
    }

    WeightT getWeight() {
        return weight;
    }

    DataT getData() {
        return data;
    }

    VertexT* getVertex1() {
        return v1;
    };

    VertexT* getVertex2() {
        return v2;
    };

    bool isWeightSet() {
        return weightSet;
    }

    bool isDataSet() {
        return dataSet;
    }
};

//форма представления
template<class EdgeT>
class GraphForm {
public:
    //Вставка и удаление вершин и рёбер
    virtual bool InsertV(int index) = 0;

    virtual bool DeleteV(int index) = 0;

    virtual bool InsertE(int v1, int v2, EdgeT* t) = 0;

    virtual bool DeleteE(int v1, int v2) = 0;

    //Удалить входящие и исходящие из вершины рёбра
    virtual int DeleteEsFromVertex(int index) = 0;

    //Проверка и получение
    virtual bool hasEdge(int v1, int v2) = 0;

    virtual EdgeT* getEdge(int v1, int v2) = 0;
};

//форма представления матричная
template<class EdgeT>
class GraphMatrixForm : public GraphForm<EdgeT> {
    friend class EdgeIterator;

    bool directed;
public:
    vector<vector<EdgeT*>> matrix;        //Матрица смежности

    explicit GraphMatrixForm(bool directed) : directed(directed) {}

    //Вставка и удаление вершин и рёбер
    bool InsertV(int index) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (index < 0 || index > size) return false;
        //Создаём новую пустую строку
        vector<EdgeT*> newLine;
        newLine.assign(size, NULL);
        //Вставляем новую строку:
        matrix.insert(matrix.begin() + index, newLine);
        ++size;
        //Вставляем новый столбец:
        for (int i = 0; i < size; ++i)
            matrix[i].insert(matrix[i].begin() + index, (EdgeT*)NULL);
        return true;
    }

    bool DeleteV(int index) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (index < 0 || index >= size) return false;
        //Удаляем строку:
        matrix.erase(matrix.begin() + index);
        --size;
        //Удаляем столбец:
        for (int i = 0; i < size; i++)
            matrix[i].erase(matrix[i].begin() + index);
        return true;
    }

    bool InsertE(int v1, int v2, EdgeT* t) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //Петля или ребро уже есть
        if (v1 == v2 || matrix[v1][v2] != NULL) return false;
        //Вставляем ребро
        matrix[v1][v2] = t;
        return true;
    }

    bool DeleteE(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size) return false;
        //Ребра нет
        if (v1 == v2 || matrix[v1][v2] == NULL) return false;
        matrix[v1][v2] = NULL;
        return true;
    }

    //Удалить входящие и исходящие из вершины рёбра
    int DeleteEsFromVertex(int index) {
        int size = matrix.size(); //Число вершин
        int deleted = 0;
        //Неверный номер вершины
        if (index < 0 || index >= size) return 0;
        //Удаляем связанные с вершиной рёбра
        for (int i = 0; i < size; i++) {
            if (matrix[i][index] != NULL) {
                delete matrix[i][index];
                matrix[i][index] = NULL;
                ++deleted;
                //Стираем симметричное ребро
                if (!directed)
                    matrix[index][i] = NULL;
            }
            if (matrix[index][i] != NULL) {
                delete matrix[index][i];
                matrix[index][i] = NULL;
                ++deleted;
            }
        }
        return deleted;
    }

    //Проверка и получение
    bool hasEdge(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
            return false;
        if (v1 == v2) //Петля
            return false;
        if (matrix[v1][v2] != NULL)
            return true;
        return false;
    }

    EdgeT* getEdge(int v1, int v2) {
        int size = matrix.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
            throw exception();
        if (v1 == v2 || matrix[v1][v2] == NULL)//Петля
            throw exception();
        return matrix[v1][v2];
    }

    //Итератор рёбер
    class EdgeIterator {
        bool isEnd;
        GraphMatrixForm* f;
        int curI, curJ;
    public:
        EdgeIterator(GraphMatrixForm* f) {
            this->f = f;
        }

        EdgeIterator& beg() {
            for (int i = 0; i < f->matrix.size(); ++i)
                for (int j = 0; j < f->matrix.size(); ++j)
                    if (f->matrix[i][j]) {
                        curI = i;
                        curJ = j;
                        isEnd = false;
                        return *this;
                    }
            isEnd = true;
            return *this;
        }

        EdgeIterator& end() {
            isEnd = true;
            return *this;
        }


        EdgeIterator& operator++() {
            if (isEnd)
                return *this;
            ++curJ;
            while (curI < f->matrix.size()) {
                while (curJ < f->matrix.size()) {
                    if (f->matrix[curI][curJ])
                        return *this;
                    ++curJ;
                }
                ++curI;
                curJ = (f->directed ? 0 : curI + 1);
            }
            isEnd = true;
            return *this;
        }

        EdgeT* operator*() {
            if (isEnd)
                throw exception();
            if (!(f->matrix[curI][curJ]))
                throw exception();
            return f->matrix[curI][curJ];
        }
    };

    //Итератор исходящих рёбер
    class OutputEdgeIterator {
        int curI, curJ;
        bool isEnd;
        GraphMatrixForm* f;
    public:
        OutputEdgeIterator(GraphMatrixForm* f, int vIndex) {
            this->f = f;
            curI = vIndex;
            beg();
        }

        OutputEdgeIterator& beg() {
            for (curJ = 0; curJ < f->matrix.size(); ++curJ)
                if (f->matrix[curI][curJ]) {
                    isEnd = false;
                    return *this;
                }
            isEnd = true;
            return *this;
        }

        OutputEdgeIterator& end() {
            for (curJ = f->matrix.size() - 1; curJ >= 0; --curJ)
                if (f->matrix[curI][curJ]) {
                    isEnd = false;
                    return *this;
                }
            isEnd = true;
            return *this;
        }

        OutputEdgeIterator& operator++() {
            if (isEnd)
                return *this;
            ++curJ;
            while (curJ < f->matrix.size()) {
                if (f->matrix[curI][curJ])
                    return *this;
                ++curJ;
            }
            isEnd = true;
            return *this;
        }

        EdgeT* operator*() {
            if (isEnd)
                throw exception();
            return f->matrix[curI][curJ];
        }
    };
};


//форма представления список
template<class EdgeT>
class GraphListForm : public GraphForm<EdgeT> {
    //Элемент списка
    class Node {
    public:
        EdgeT* edge; //Само ребро
        int v2;      //Вторая вершина, которую ребро соединяет
    };

    bool directed;
    vector<list<Node>> edgeList; //Списки смежности
public:
    GraphListForm(bool directed) : directed(directed) {}

    //Вставка и удаление вершин и рёбер
    bool InsertV(int index) {
        int size = edgeList.size(); //Число вершин
        if (index < 0 || index > size) //Неверный номер вершины
            return false;
        //Создаём новый список смежности
        list<Node> newList;
        //Вставляем
        edgeList.insert(edgeList.begin() + index, newList);
        ++size;
        //Обновляем дескрипторы
        for (int i = 0; i < size; ++i)
            for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if ((*j).v2 >= index)//если текущая вершина имеет больший номер, чем вставляемая,
                    ++((*j).v2);//то увеличиваем этот номер
        return true;
    }

    bool DeleteV(int index) {
        int size = edgeList.size(); //Число вершин
        if (index < 0 || index >= size)  //Неверный номер вершины
            return false;
        //Удаляем из списков записи о рёбрах
        for (int i = 0; i < size; ++i)
            for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if ((*j).v2 == index) {
                    edgeList[i].erase(j);
                    break;
                }
        //Удаляем список смежности
        edgeList.erase(edgeList.begin() + index);
        --size;
        //Обновляем дескрипторы
        for (int i = 0; i < size; ++i)
            for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if ((*j).v2 > index)//если текущая вершина имеет больший номер, чем удаляемая,
                    --((*j).v2);//то уменьшить этот номер
        return true;
    }

    bool InsertE(int v1, int v2, EdgeT* t) {
        int size = edgeList.size(); //Число вершин
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)//Неверный номер вершины
            return false;
        if (v1 == v2 || hasEdge(v1, v2))  //Петля или ребро уже есть
            return false;
        //Вставляем ребро
        Node node;
        node.edge = t;
        node.v2 = v2;
        edgeList[v1].push_front(node);
        return true;
    }

    bool DeleteE(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
            return false;
        //Ребра нет
        if (v1 == v2 || hasEdge(v1, v2) == false)
            return false;
        //Удаляем ребро
        for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if ((*j).v2 == v2) {
                edgeList[v1].erase(j);
                break;
            }
        return true;
    }

    //Удалить входящие и исходящие из вершины рёбра
    int DeleteEsFromVertex(int index) {
        int size = edgeList.size(); //Число вершин
        int deleted = 0;
        //Неверный номер вершины
        if (index < 0 || index >= size)
            return 0;
        //Удаляем связанные с вершиной рёбра
        for (int i = 0; i < size; ++i)
            for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
                if ((*j).v2 == index) {
                    delete (*j).edge;
                    edgeList[i].erase(j);
                    ++deleted;
                    //Стираем симметричное ребро
                    if (!directed)
                        for (typename list<Node>::iterator k = edgeList[index].begin();
                            k != edgeList[index].end(); ++k)
                            if ((*k).v2 == i) {
                                edgeList[index].erase(k);
                                break;
                            }
                    break;
                }
        if (directed) {
            for (typename list<Node>::iterator z = edgeList[index].begin(); z != edgeList[index].end(); ++z) {
                delete (*z).edge;
                deleted++;
            }
        }
        return deleted;
    }

    //Проверка и получение
    bool hasEdge(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
            return false;
        //Петля
        if (v1 == v2)
            return false;
        for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if ((*j).v2 == v2)
                return true;
        return false;
    }

    EdgeT* getEdge(int v1, int v2) {
        int size = edgeList.size(); //Число вершин
        //Неверный номер вершины
        if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
            throw exception();
        //Петля
        if (v1 == v2)
            throw exception();
        for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
            if ((*j).v2 == v2)
                return (*j).edge;
        throw exception();
    }

    //Итератор рёбер
    class EdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool isEnd;
        GraphListForm* f;
    public:
        EdgeIterator(GraphListForm* f) {
            this->f = f;
            beg();
        }

        //Установить итератор рёбер на начальное ребро
        EdgeIterator& beg() {
            for (curI = 0; curI < f->edgeList.size(); ++curI)
                for (curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                    if ((*curJ).edge) {
                        isEnd = false;
                        return *this;
                    }
            isEnd = true;
            return *this;
        }

        EdgeIterator& end() {
            isEnd = true;
            return *this;
        }

        //Переход к следующей позиции
        EdgeIterator& operator++() {
            if (isEnd)
                return *this;
            ++curJ;
            while (curI < f->edgeList.size()) {
                while (curJ != f->edgeList[curI].end()) {
                    if ((*curJ).edge && (f->directed || !f->directed && (*curJ).v2 > curI))
                        return *this;
                    ++curJ;
                }
                ++curI;
                if (curI < f->edgeList.size())
                    curJ = f->edgeList[curI].begin();
            }
            isEnd = true;
            return *this;
        }

        EdgeT* operator*() {
            if (isEnd)
                throw exception();
            return (*curJ).edge;
        }
    };

    //Итератор исходящих рёбер
    class OutputEdgeIterator {
        int curI;
        typename list<typename GraphListForm<EdgeT>::Node>::iterator curJ;
        bool isEnd;
        GraphListForm* f;
    public:
        OutputEdgeIterator(GraphListForm* f, int vIndex) {
            this->f = f;
            curI = vIndex;
            beg();
        }

        OutputEdgeIterator& beg() {
            for (curJ = f->edgeList[curI].begin(); curJ != f->edgeList[curI].end(); ++curJ)
                if ((*curJ).edge) {
                    isEnd = false;
                    return *this;
                }
            isEnd = true;
            return *this;
        }

        OutputEdgeIterator& end() {
            isEnd = true;
            return *this;
        }

        OutputEdgeIterator& operator++() {
            if (isEnd)
                return *this;
            ++curJ;
            while (curJ != f->edgeList[curI].end()) {
                if ((*curJ).edge)
                    return *this;
                ++curJ;
            }
            isEnd = true;
            return *this;
        }

        EdgeT* operator*() {
            if (isEnd)
                throw exception();
            return (*curJ).edge;
        }
    };
};

template<class VertexT, class EdgeT>
class Graph {
    friend class VertexIterator;  //дружественный класс
    vector<VertexT*> vertexVector;//Вектор вставленных
    GraphForm<EdgeT>* data;       //Данные графа
    bool directed;                //Направленный граф
    bool dense;                   //М-граф
    int edgeCounter;              //Число рёбер

    //Получение индекса вершины по дескриптору
    int getIndexFromName(string n) {
        int index = 0;
        for (int i = 0; i < vertexVector.size(); ++i) {
            if (vertexVector[index]->getName() == n)
                break;
            ++index;
        }
        if (index == vertexVector.size())
            throw exception();
        return index;
    };

public:
    //Создать пустой L-граф с нулевым числом вершин и рёбер
    Graph() {
        data = new GraphListForm<EdgeT>(false);
        directed = false;
        dense = false;
        edgeCounter = 0;
    }

    //Создать граф с V вершинами, без ребер, типа D, формы представления F
    Graph(int vertexCount, bool directed, bool dense) {
        if (dense)
            data = new GraphMatrixForm<EdgeT>(directed);
        else
            data = new GraphListForm<EdgeT>(directed);
        //Создаём вершины и помещаем в структуру
        for (int i = 0; i < vertexCount; ++i)
            data->InsertV(i);
        for (int i = 0; i < vertexCount; ++i) {
            VertexT* v = new VertexT(i);
            char p[17];
            _itoa_s(i, p, 10);
            v->setName(p);
            vertexVector.push_back(v);
            data->InsertV(i);
        }
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;
    }

    //Создать граф с V вершинами, с E случайными ребрами, типа D , формы F
    Graph(int vertexCount, int edgeCount, bool directed, bool dense) {
        if (dense)
            data = new GraphMatrixForm<EdgeT>(directed);
        else
            data = new GraphListForm<EdgeT>(directed);
        //Создаём вершины и помещаем в структуру
        for (int i = 0; i < vertexCount; ++i)
            data->InsertV(i);
        for (int i = 0; i < vertexCount; ++i) {
            VertexT* v = new VertexT(i);
            char p[17];
            _itoa_s(i, p, 10);
            v->setName(p);
            vertexVector.push_back(v);
            data->InsertV(i);
        }
        this->directed = directed;
        this->dense = dense;
        edgeCounter = 0;

        //Вставляем рёбра
        if (edgeCount <= 0) return;
        //Корректируем число рёбер, чтобы не превышало максимумы
        if (directed) {
            if (edgeCount > vertexCount * (vertexCount - 1))
                edgeCount = vertexCount * (vertexCount - 1);
            srand(time(NULL));
        }
        else {
            if (edgeCount > (vertexCount * (vertexCount - 1)) / 2)
                edgeCount = (vertexCount * (vertexCount - 1)) / 2;
        }
        //Вставляем
        int v1, v2;

        while (edgeCounter < edgeCount) {
            v1 = rand() % vertexCount;
            v2 = rand() % vertexCount;
            if (v1 == v2)
                continue;
            if (data->hasEdge(v1, v2))
                continue;

            EdgeT* t = new EdgeT(vertexVector[v1], vertexVector[v2]);
            data->InsertE(v1, v2, t);
            t->setWeight((rand() % 9) + 1);
            if (!directed)
                data->InsertE(v2, v1, t);
            ++edgeCounter;
        }
    }

    //конструктор копирования
    Graph(const Graph<VertexT, EdgeT>& G) {
        delete data;
        if (G.Dense())
            data = new GraphMatrixForm<EdgeT>(*(G.data));
        else
            data = new GraphListForm<EdgeT>(*(G.data));
        directed = G.directed;
        dense = G.dense;
    }

    //деструктор
    ~Graph() {
        while (vertexVector.size() != 0)
            DeleteV(vertexVector.size() - 1);
        delete data;
    }

    //возвращает число вершин в графе
    int V() {
        return vertexVector.size();
    }

    //возвращает число ребер в графе
    int E() {
        return edgeCounter;
    }

    //возвращает тип графа
    bool Directed() {
        return directed;
    }

    //возвращает форму представления графа
    bool Dense() {
        return dense;
    }

    //возвращает коэффициент насыщенности графа
    double K() {
        int max = vertexVector.size() * (vertexVector.size() - 1);
        if (!directed) {
            max /= 2;
        }
        if (max == 0) {
            return -1;
        }
        else {
            return (double)edgeCounter / (double)max;
        }
    }

    //преобразует граф к L-графу
    void ToListGraph() {
        //Создаём структуру хранения L-графа
        GraphForm<EdgeT>* newData = new GraphListForm<EdgeT>(this->directed);

        //Создаём вершины
        for (int i = 0; i < vertexVector.size(); ++i)
            newData->InsertV(i);

        //Переносим рёбра
        for (int i = 0; i < vertexVector.size(); ++i)
            for (int j = 0; j < vertexVector.size(); ++j)
                if (data->hasEdge(i, j))
                    newData->InsertE(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = false;
    }

    //преобразует граф к M-графу
    void ToMatrixGraph() {
        //Создаём структуру хранения М-графа
        GraphForm<EdgeT>* newData = new GraphMatrixForm<EdgeT>(this->directed);

        //Создаём вершины
        for (int i = 0; i < vertexVector.size(); ++i)
            newData->InsertV(i);

        //Переносим рёбра
        for (int i = 0; i < vertexVector.size(); ++i)
            for (int j = 0; j < vertexVector.size(); ++j)
                if (data->hasEdge(i, j))
                    newData->InsertE(i, j, data->getEdge(i, j));
        delete data;
        data = newData;
        dense = true;
    }

    //добавляет вершину к графу и возвращает адрес дескриптора вновь созданной вершины
    VertexT* InsertV(string name) {
        VertexT* v = new VertexT(vertexVector.size());
        data->InsertV(vertexVector.size());
        v->setName(name);
        vertexVector.push_back(v);
        return v;
    }

    //добавляет безымянную вершину к графу и возвращает адрес дескриптора вновь созданной вершины
    VertexT* InsertV() {
        VertexT* v = new VertexT(vertexVector.size());
        data->InsertV(vertexVector.size());
        vertexVector.push_back(v);
        return v;
    }

    //удаляет вершину из графа, заданную адресом дескриптора v
    bool DeleteV(VertexT* v) {
        int index = getIndex(v);
        edgeCounter -= data->DeleteEsFromVertex(index);
        if (data->DeleteV(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }

    bool DeleteV(int index) {
        edgeCounter -= data->DeleteEsFromVertex(index);
        if (data->DeleteV(index)) {
            vertexVector.erase(vertexVector.begin() + index);
            return true;
        }
        return false;
    }

    bool DeleteV(string n) {
        VertexT* v = new VertexT(-1);
        bool rez;
        try {
            v = getVertexFromName(n);
        }
        catch (exception&) {
            return false;
        }
        rez = DeleteV(v);
        return rez;
    }

    VertexT* getVertex(int index) {
        if (index < 0 || index >= vertexVector.size())
            throw exception();
        return vertexVector[index];
    }

    VertexT* getVertexFromName(string n) {
        int i;
        for (i = 0; i < vertexVector.size(); i++)
            if (vertexVector[i]->getName() == n)
                return vertexVector[i];
        throw exception();
    }

    int getIndex(VertexT* v) {
        int index = 0;
        for (int i = 0; i < vertexVector.size(); ++i) {
            if (vertexVector[index] == v)
                break;
            ++index;
        }
        if (index == vertexVector.size())
            throw exception();
        return index;
    }

    //добавляет ребро (v1, v2) к графу, соединяющую вершины, заданные адресами дескрипторов v1 и v2
    bool InsertE(string v1, string v2, int _weight = 0) {
        VertexT* _v1;
        VertexT* _v2;
        try {
            _v1 = getVertexFromName(v1);
            _v2 = getVertexFromName(v2);
            InsertE(_v1, _v2, _weight);
        }
        catch (exception&) {
            return false;
        }
        return true;
    }

    EdgeT* InsertE(VertexT* v1, VertexT* v2, int _weight = 0) {
        EdgeT* e = new EdgeT(v1, v2, _weight);
        if (!data->InsertE(getIndex(v1), getIndex(v2), e)) throw exception();
        if (!directed)
            data->InsertE(getIndex(v2), getIndex(v1), e);
        ++edgeCounter;
        return e;
    }

    //удаляет ребро, соединяющее вершины, заданные адресами дескрипторов v1 и v2
    bool DeleteE(string v1, string v2) {
        VertexT* _v1;
        VertexT* _v2;
        try {
            _v1 = getVertexFromName(v1);
            _v2 = getVertexFromName(v2);
        }
        catch (exception&) {
            return false;
        }
        if (!DeleteE(_v1, _v2)) return false;
        else return true;
    };

    bool DeleteE(VertexT* v1, VertexT* v2) {
        if (data->DeleteE(getIndex(v1), getIndex(v2))) {
            --edgeCounter;
            if (!directed)
                data->DeleteE(getIndex(v2), getIndex(v1));
            return true;
        }
        else
            return false;
    }

    bool hasEdge(VertexT* v1, VertexT* v2) {
        int ind1, ind2;
        try {
            ind1 = getIndex(v1);
            ind2 = getIndex(v2);
        }
        catch (exception&) {
            return false;
        }
        return data->hasEdge(ind1, ind2);
    }

    bool hasEdge(int v1, int v2) {
        if (v1 < 0 || v1 >= vertexVector.size())
            return false;
        if (v2 < 0 || v2 >= vertexVector.size())
            return false;
        return data->hasEdge(v1, v2);
    }

    EdgeT* getEdge(VertexT* v1, VertexT* v2) {
        EdgeT* e;
        try {
            e = data->getEdge(getIndex(v1), getIndex(v2));
        }
        catch (exception&) {
            throw exception();
        }
        return e;
    }

    void print_graph() {
        int i, j;
        VertexT* v;
        EdgeT* e;
        if (Dense()) {
            cout << "  ";
            for (i = 0; i < V(); i++) {
                v = getVertex(i);
                cout << setw(4) << v->getName();
            }
            cout << endl;
            for (i = 0; i < 5 * V(); i++)
                cout << "_";
            cout << endl;
            for (i = 0; i < V(); i++) {
                v = getVertex(i);
                cout << v->getName() << "|";
                for (j = 0; j < V(); j++)
                    if (hasEdge(i, j)) {
                        e = getEdge(getVertex(i), getVertex(j));
                        cout << setw(4) << 1 << " " << e->getWeight();
                    }
                    else
                        cout << setw(4) << "0" << " " << "0";
                cout << endl;
            }
        }
        else {
            for (i = 0; i < V(); i++) {
                v = getVertex(i);
                cout << "*" << v->getName() << "->";
                for (j = 0; j < V(); j++) {
                    v = getVertex(j);
                    if (hasEdge(i, j))
                        cout << v->getName() << " " << getEdge(getVertex(i), getVertex(j))->getWeight() << "->";
                }
                cout << endl;
            }
        }
    };

    //Итератор вершин
    class VertexIterator {
        Graph<VertexT, EdgeT>* graph;
        bool isEnd;
        int current;
    public:
        VertexIterator() {
            graph = nullptr;
        }

        VertexIterator(Graph<VertexT, EdgeT>& g) {
            graph = &g;
        }

        //возвращает итератор, установленный на первую вершину графа
        VertexIterator& beg() {
            if (graph->vertexVector.size() == 0) {
                isEnd = true;
                return *this;
            }
            current = 0;
            isEnd = false;
            return *this;
        }

        //Переход к следующей позиции
        VertexIterator& operator++() {
            if (!isEnd) {
                current++;
                if (graph->vertexVector.size() == current)
                    isEnd = true;
            }
            return *this;
        }

        //Установить итератор вершин на конечную вершину
        VertexIterator& end() {
            current = graph->vertexVector.size();
            isEnd = true;
            return *this;
        };

        //Получение вершины
        VertexT* operator*() {
            try {
                graph->vertexVector.at(current);
            }
            catch (exception&) {
                throw exception();
            }
            return graph->vertexVector[current];
        }

        VertexIterator(VertexIterator* pIterator) {
            this->graph = pIterator->graph;
            this->current = pIterator->current;
            this->isEnd = pIterator->isEnd;
        }
    };

    //Итератор рёбер
    class EdgeIterator {
        Graph<VertexT, EdgeT>* graph; //Обрабатываемый граф
        typename GraphMatrixForm<EdgeT>::EdgeIterator* mIt;
        typename GraphListForm<EdgeT>::EdgeIterator* lIt;
        bool useM{};
    public:

        EdgeIterator(Graph<VertexT, EdgeT>& g) {
            graph = &g;
            if (graph->Dense()) {
                mIt = new typename GraphMatrixForm<EdgeT>::EdgeIterator(static_cast<GraphMatrixForm<EdgeT> *>(g.data));
                useM = true;
            }
            else {
                lIt = new typename GraphListForm<EdgeT>::EdgeIterator(static_cast<GraphListForm<EdgeT> *>(g.data));
                useM = false;
            }
        }

        //Установка в начало
        EdgeIterator& beg() {
            if (useM)
                mIt->beg();
            else
                lIt->beg();
            return *this;
        }

        //Сдвиг на следующий элемент
        EdgeIterator& operator++() {
            if (useM)
                ++(*mIt);
            else
                ++(*lIt);
            return *this;
        }

        EdgeIterator& end() {
            if (useM)
                mIt->end();
            else
                lIt->end();
            return *this;
        }

        //Получение ребра
        EdgeT* operator*() {
            if (useM) {
                try {
                    return *(*mIt);
                }
                catch (exception&) {
                    throw exception();
                }
            }
            else {
                try {
                    return *(*lIt);
                }
                catch (exception&) {
                    throw exception();
                }
            }
        }

        EdgeIterator() {
            this->graph = nullptr;
            this->mIt = nullptr;
            this->lIt = nullptr;
        }

        EdgeIterator(EdgeIterator* pIterator) {
            this->mIt = pIterator->mIt;
            this->lIt = pIterator->lIt;
            this->graph = pIterator->graph;
            this->useM = pIterator->useM;
        }
    };

    //Итератор исходящих рёбер
    class OutputEdgeIterator {
        Graph<VertexT, EdgeT>* graph; //Обрабатываемый граф
        typename GraphMatrixForm<EdgeT>::OutputEdgeIterator* mIt;
        typename GraphListForm<EdgeT>::OutputEdgeIterator* lIt;
        bool useM;
    public:
        OutputEdgeIterator(Graph<VertexT, EdgeT>& g) {
            graph = &g;
            int curV1 = -1;
        }

        OutputEdgeIterator(Graph<VertexT, EdgeT>& g, VertexT& v) {
            graph = &g;
            int curV1 = graph->getIndex(&v);
            if (graph->Dense()) {
                mIt = new  typename GraphMatrixForm<EdgeT>::OutputEdgeIterator (static_cast<GraphMatrixForm<EdgeT> *>(g.data), curV1);
                useM = true;
            }
            else {
                lIt = new typename GraphListForm<EdgeT>::OutputEdgeIterator (static_cast<GraphListForm<EdgeT> *>(g.data), curV1);
                useM = false;
            }
        }

        //Установка в начало
        OutputEdgeIterator& beg() {
            if (useM)
                mIt->beg();
            else
                lIt->beg();
            return *this;
        }

        //Сдвиг на следующий элемент
        OutputEdgeIterator& operator++() {
            if (useM)
                ++(*mIt);
            else
                ++(*lIt);
            return *this;
        }

        OutputEdgeIterator& end() {
            if (useM)
                mIt->end();
            else
                lIt->end();
            return *this;
        }

        //Получение ребра
        EdgeT* operator*() {
            if (useM) {
                try {
                    return *(*mIt);
                }
                catch (exception&) {
                    throw exception();
                }
            }
            else {
                try {
                    return *(*lIt);
                }
                catch (exception&) {
                    throw exception();
                }
            }
        }

        OutputEdgeIterator(OutputEdgeIterator* pIterator) {
            this->graph = pIterator->graph;
            this->useM = pIterator->useM;
            this->lIt = pIterator->lIt;
            this->mIt = pIterator->mIt;
        }

        OutputEdgeIterator() {
            this->lIt = nullptr;
            this->mIt = nullptr;
            this->graph = nullptr;
        }
    };
};

//template<class DVertex, class DEdge>
//class Task1 {
//public:
//    typedef Vertex<int, list<DVertex*>*> reduce_vertex;
//    typedef Edge<reduce_vertex, int, list<DEdge*>*> reduce_edge;
//private:
//    Graph<DVertex, DEdge>* g;
//    stack<DVertex*> S, path;
//    int cnt, scnt;
//    map<DVertex*, int> pre, id;
//    void scR(DVertex* w);
//    DVertex* otherVertex(DEdge* e, DVertex* v);
//    void init();
//    bool found;
//    list<DEdge*> undefined_edge;
//    Graph<reduce_vertex, reduce_edge>* result;
//    map<int, reduce_vertex*> map_vertex;
//    void push_edge_to_graph(DEdge* e);
//public:
//    Task1(Graph<DVertex, DEdge>* g);
//    Task1(const Task1& obj);
//    ~Task1() {}
//    void Set(Graph<DVertex, DEdge>* g);
//    void Restart();
//    Graph<reduce_vertex, reduce_edge>* Result();
//};
//
//
//template<class DVertex, class DEdge>
//Task1<DVertex, DEdge>::Task1(Graph<DVertex, DEdge>* g) {
//    //if (!g->Directed()) throw NotDirectedGraphException();
//    Set(g);
//}
//
//template<class DVertex, class DEdge>
//Task1<DVertex, DEdge>::Task1(const Task1<DVertex, DEdge>& obj) {
//    this->g = obj.g;
//    this->cnt = obj.cnt;
//    this->scnt = obj.scnt;
//    this->id = obj.id;
//    this->pre = obj.pre;
//    this->S = obj.S;
//    this->path = obj.path;
//}
//
//template<class DVertex, class DEdge>
//void Task1<DVertex, DEdge>::Set(Graph<DVertex, DEdge>* g) {
//    //if (!g->Directed()) throw NotDirectedGraphException();
//    this->g = g;
//    Restart();
//}
//
//template<class DVertex, class DEdge>
//void Task1<DVertex, DEdge>::Restart() {
//    this->init();
//    VertexIterator i(*this);  // Use your custom VertexIterator
//    for (i.beg(); i != i.end(); ++i) {
//        if (pre[*i] == -1) scR(*i);
//        found = false;
//    }
//   /* Graph<DVertex, DEdge>::VertexIterator i = beg();
//    for (; i != this->g->endV(); ++i) {
//        if (pre[*i] == -1) scR(*i);
//        found = false;
//    }*/
//    for (typename list<DEdge*>::iterator j = undefined_edge.begin(); j != undefined_edge.end(); ++j) {
//        this->push_edge_to_graph(*j);
//    }
//}
//
//
//
//
//
//
//template<class DVertex, class DEdge>
//Graph<typename Task1<DVertex, DEdge>::reduce_vertex, typename Task1<DVertex, DEdge>::reduce_edge>* Task1<DVertex, DEdge>::Result() {
//    return result;
//}
//
//template<class DVertex, class DEdge>
//void Task1<DVertex, DEdge>::scR(DVertex* w) {
//    DVertex* v = NULL;
//    pre[w] = cnt++;
//    S.push(w); path.push(w);
//    typename Graph<DVertex, DEdge>::out_edge_iterator i = g->beginEout(w);
//    for (; i != g->endEout(w); ++i) {
//        DVertex* t = otherVertex(*i, w);
//        if (pre[t] == -1) {
//            scR(t);
//            if (found) {
//                undefined_edge.push_back(*i);
//                found = false;
//            }
//            else {
//
//            }
//        }
//        else if (id[t] == -1)
//            while (pre[path.top()] > pre[t])
//                path.pop();
//        else
//            if (id[w] != id[t])
//                undefined_edge.push_back(*i);
//    }
//    if (path.top() == w)
//        path.pop();
//    else
//        return;
//    found = true;
//    list<DVertex*>* l = new list<DVertex*>();
//    do {
//        v = S.top(); S.pop();
//        id[v] = scnt;
//        l->push_back(v);
//    } while (v != w);
//    Task1<DVertex, DEdge>::reduce_vertex* V = result->InsertV();
//    V->SetName(scnt);
//    V->SetData(l);
//    map_vertex[scnt] = V;
//    scnt++;
//}
//
//template<class DVertex, class DEdge>
//void Task1<DVertex, DEdge>::init() {
//    VertexIterator i(*this);  // Use your custom VertexIterator
//    for (i.beg(); i != i.end(); ++i) {
//        DVertex* v = *i;
//        pre[v] = -1;
//        id[v] = -1;
//    }
//    cnt = scnt = 0;
//    found = false;
//    result = new Graph<Task1<DVertex, DEdge>::reduce_vertex, Task1<DVertex, DEdge>::reduce_edge>(0, true, true);
//}
//
//template<class DVertex, class DEdge>
//DVertex* Task1<DVertex, DEdge>::otherVertex(DEdge* e, DVertex* v) {
//    DVertex* v1 = e->getV1();
//    DVertex* v2 = e->getV2();
//    if (v == v1)
//        return v2;
//    else
//        return v1;
//}
//
//template<class DVertex, class DEdge>
//void Task1<DVertex, DEdge>::push_edge_to_graph(DEdge* e) {
//    int id1 = id[e->getV1()];
//    int id2 = id[e->getV2()];
//    Task1<DVertex, DEdge>::reduce_vertex* v1 = map_vertex[id1];
//    Task1<DVertex, DEdge>::reduce_vertex* v2 = map_vertex[id2];
//    Task1<DVertex, DEdge>::reduce_edge* E = result->GetEdge(v1, v2);
//    if (E == NULL) {
//        E = result->InsertE(v1, v2);
//        list<DEdge*>* l = new list<DEdge*>();
//        l->push_back(e);
//        E->SetData(l);
//        return;
//    }
//    list<DEdge*>* l = E->GetData();
//    l->push_back(e);
//}
//
//

//template<class VertexT, class EdgeT>
//class TaskOne {
//
//
//    Graph<VertexT, EdgeT>* graph;
//    vector<VertexT*> result;
//    int weight;
//
//    // ... (ваш код)
//
//    // Добавляем функцию для обхода графа в глубину
//    void DFS(int v, vector<bool>& visited, stack<int>& st) {
//        visited[v] = true;
//        for (int i = 0; i < graph->V(); i++) {
//            if (!visited[i] && graph->hasEdge(v, i) && graph->getEdge(v, graph->getVertex(i))->getWeight() > weight) {
//                DFS(i, visited, st);
//            }
//        }
//        st.push(v);
//    }
//
//    // Добавляем функцию для поиска компонент сильной связности
//    void SCCUtil(int v, vector<bool>& visited, vector<VertexT*>& component) {
//        visited[v] = true;
//        component.push_back(graph->getVertex(v));
//        for (int i = 0; i < graph->V(); i++) {
//            if (!visited[i] && graph->hasEdge(i, v) && graph->getEdge(graph->getVertex(i), graph->getVertex(v))->getWeight() > weight) {
//                SCCUtil(i, visited, component);
//            }
//        }
//    }
//
//    // Добавляем функцию для построения редуцированного графа
//    Graph<VertexT, EdgeT> buildReducedGraph(const vector<vector<VertexT*>>& components) {
//        unordered_set<int> visitedComponents;
//        Graph<VertexT, EdgeT> reducedGraph;
//
//        // Добавляем вершины в редуцированный граф
//        for (const auto& component : components) {
//            reducedGraph.InsertV(graph->getVertex(graph->getIndex(component[0])));
//            visitedComponents.insert(component[0]);
//        }
//
//        // Добавляем рёбра в редуцированный граф
//        for (const auto& component : components) {
//            for (int i : component) {
//                for (int j = 0; j < graph->V(); j++) {
//                    if (visitedComponents.find(j) == visitedComponents.end() &&
//                        graph->hasEdge(i, j) && graph->getEdge(graph->getVertex(i), graph->getVertex(j))->getWeight() > weight) {
//                        reducedGraph.addEdge(graph->getVertex(i), graph->getVertex(j), graph->getEdge(graph->getVertex(i), graph->getVertex(j))->getWeight());
//                    }
//                }
//            }
//        }
//
//        return reducedGraph;
//    }
//
//public:
//    TaskOne(Graph<VertexT, EdgeT>* g, int _weight) {
//        Set(g, _weight);
//    }
//
//    TaskOne(const TaskOne& other) {
//        this->result = other.result;
//        this->weight = other.weight;
//        this->graph = other.graph;
//    }
//
//    void Set(Graph<VertexT, EdgeT>* g, int _weight) {
//        this->graph = g;
//        this->weight = _weight;
//        this->result.clear();
//        //Restart();
//    }
//    // Функция для построения редуцированного графа сильно связных компонент
//    Graph<VertexT, EdgeT> buildReducedGraph() {
//        stack<int> st;
//        vector<bool> visited(graph->V(), false);
//
//        // Обходим граф и добавляем вершины в стек в порядке завершения
//        for (int i = 0; i < graph->V(); i++) {
//            if (!visited[i]) {
//                DFS(i, visited, st);
//            }
//        }
//
//        // Инвертируем граф
//        //Graph<VertexT, EdgeT> transposedGraph = graph->transpose();
//
//        visited.assign(graph->V(), false);
//
//        // Посещаем вершины в порядке завершения и находим компоненты сильной связности
//        vector<vector<VertexT*>> components;
//        while (!st.empty()) {
//            int v = st.top();
//            st.pop();
//
//            if (!visited[v]) {
//                vector<VertexT*> component;
//                SCCUtil(v, visited, component);
//                components.push_back(component);
//            }
//        }
//
//        // Строим редуцированный граф
//        return buildReducedGraph(components);
//    }
//};


template<class VertexT, class EdgeT>
class TaskTwo {
    Graph<VertexT, EdgeT>* graph;
    vector<VertexT*> result;
    int weight;

    bool notInCluster(VertexT* vertex, vector<VertexT*> cluster) {
        if (!cluster.empty()) {
            for (int i = 0; i < cluster.size(); i++) {
                if (*cluster[i] == *vertex)
                    return false;
            }
        }
        return true;
    }

    vector<VertexT*> findBiggestCluster(vector<vector<VertexT*>> clusters) {
        int index = 0;
        int maxSize = 0;
        for (int i = 0; i < clusters.size(); i++) {
            if (clusters[i].size() > maxSize) {
                maxSize = clusters[i].size();
                index = i;
            }
        }
        if (!clusters.empty())
            return clusters[index];
        else throw exception();
    }

    bool notInClusters(VertexT* vertex, vector<vector<VertexT*>> clusters) {
        if (!clusters.empty()) {
            for (int i = 0; i < clusters.size(); i++) {
                for (int j = 0; j < clusters[i].size(); j++) {
                    if (*clusters[i][j] == *vertex)
                        return false;
                }
            }
        }
        return true;
    }

public:
    TaskTwo(Graph<VertexT, EdgeT>* g, int _weight) {
        Set(g, _weight);
    }

    TaskTwo(const TaskTwo& other) {
        this->result = other.result;
        this->weight = other.weight;
        this->graph = other.graph;
    }

    void Set(Graph<VertexT, EdgeT>* g, int _weight) {
        this->graph = g;
        this->weight = _weight;
        this->result.clear();
        Restart();
    }


    void Restart() {
        vector<vector<VertexT*>> clusters;
        VertexT* startVertex;
        vector<VertexT*> temp;
        for (int i = 0; i < graph->V(); i++) {
            if (notInClusters(graph->getVertex(i), clusters)) {
                startVertex = graph->getVertex(i);
                temp.clear();
                temp.push_back(startVertex);
                for (int j = 0; j < temp.size(); j++) {
                    for (int k = 0; k < graph->V(); k++) {
                        if (notInCluster(graph->getVertex(k), temp)) {
                            if (graph->hasEdge(graph->getIndex(temp[j]), k)) {
                                EdgeT* tmp = graph->getEdge(temp[j], graph->getVertex(k));
                                if (tmp->getWeight() > weight) {
                                    temp.push_back(graph->getVertex(k));
                                }
                            }
                        }
                    }
                }
                clusters.push_back(temp);
            }
        }
        result = findBiggestCluster(clusters);
    }

    vector<VertexT*> Result() {
        return result;
    }

    ~TaskTwo() = default;
};

#endif //AGLRGZ_GRAPH_H