#include <iostream>
#include "graph.h"

int main() {

   int menu;
   Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>> *graph = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>; ;
   Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator vIt;
   Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator eIt;
   Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator oIt;

   cout << "1. Empty L-graph" << endl;
   cout << "2. graph with V vertexes, no edges, type D, presentation form F" << endl;
   cout << "3. graph with V vertexes, random edges, type D, presentation form F" << endl;
   cout << "4. exit" << endl;
   while (true) {
       cout << "Choose operation: ";
       cin >> menu;
       switch (menu) {
           case 1: {            //Создать пустой L-граф с нулевым числом вершин и ребер
               graph = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>();
               vIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator(*graph);
               eIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator(*graph);
               break;
           };
           case 2: {            //Создать граф с V вершинами, без ребер, типа D, формы представления F
               int tmp1, tmp2, tmp3;
               cout << "Enter number of vertex: ";
               cin >> tmp1;
               cout << "Directed (0 - no, 1 - yes): ";
               cin >> tmp2;
               cout << "Form (0 - list, 1 - matrix): ";
               cin >> tmp3;
               graph = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(tmp1, tmp2, tmp3);
               vIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator(*graph);
               eIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator(*graph);
               break;
           };
           case 3: {            //Создать граф с V вершинами, с E случайными ребрами, типа D , формы F
               int tmp1, tmp2, tmp3, tmp4;
               cout << "Enter number of vertex: ";
               cin >> tmp1;
               cout << "Enter number of edges: ";
               cin >> tmp2;
               cout << "Directed (0 - no, 1 - yes): ";
               cin >> tmp3;
               cout << "Form (0 - list, 1 - matrix): ";
               cin >> tmp4;
               graph = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>(tmp1, tmp2, tmp3, tmp4);
               vIt=new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::VertexIterator(*graph);
               eIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::EdgeIterator(*graph);
               break;
           };
           case 4: {            //выход
               return 0;
           };
           default: {
               cout << "input error" << endl;
               break;
           };
       }
       break;
   }

   while (true) {
       cout << "0. print graph" << endl;
       cout << "1. number of vertexes" << endl;
       cout << "2. number of edges" << endl;
       cout << "3. graph type" << endl;
       cout << "4. graph form" << endl;
       cout << "5. saturation factor" << endl;
       cout << "6. convert to L-form" << endl;
       cout << "7. convert to M-form" << endl;
       cout << "8. insert vertex" << endl;
       cout << "9. remove vertex" << endl;
       cout << "10. insert edge" << endl;
       cout << "11. remove edge" << endl;

       cout << endl << "Operations with vertexes:" << endl;
       cout << "12. read data" << endl;
       cout << "13. write data" << endl;
       cout << "14. Set name" << endl;

       cout << endl << "Operation with edges:" << endl;
       cout << "15. read edge weight" << endl;
       cout << "16. read edge data" << endl;
       cout << "17. write edge data" << endl;
       cout << "18. write edge weight" << endl;

       cout << endl << "Vertex iterator:" << endl;
       cout << "19. beg" << endl;
       cout << "20. end" << endl;
       cout << "21. ++" << endl;
       cout << "22. read current vertex" << endl;
       cout << "23. read current vertex data" << endl;
       cout << "24. write current vertex data" << endl;
       cout << "25. write current vertex name" << endl;

       cout << endl << "Iterator of edges:" << endl;
       cout << "26. beg" << endl;
       cout << "27. end" << endl;
       cout << "28. ++" << endl;
       cout << "29. read edges and weight" << endl;
       cout << "30. read edge's data" << endl;
       cout << "31. write edge's data" << endl;
       cout << "32. write edge's weight" << endl;

       cout << endl << "33. Iterator of output edges" << endl;

       cout << endl << "34. Find biggest cluster";

       cout << endl << "40. exit" << endl;

       cout << endl << "Choose operation: ";
       cin >> menu;
       switch (menu) {
           case 0: {
               graph->print_graph();
               break;
           }
           case 1: {
               cout << endl << graph->V() << endl;
               break;
           }
           case 2: {
               cout << endl << graph->E() << endl;
               break;
           }
           case 3: {
               cout << endl << "(0 - not directed, 1 - directed): " << graph->Directed() << endl;
               break;
           }
           case 4: {
               cout << endl << "(0 - L-graph, 1 - M-graph): " << graph->Dense() << endl;
               break;
           }
           case 5: {
               cout << endl << graph->K() << endl;
               break;
           }
           case 6: {
               graph->ToListGraph();
               break;
           }
           case 7: {
               graph->ToMatrixGraph();
               break;
           }
           case 8: {
               cout << "(with name - 0, without name - 1):";
               int tmp = 0;
               cin >> tmp;
               if (tmp == 0) {
                   cout << "\nEnter name: ";
                   string temp;
                   cin >> temp;
                   graph->InsertV(temp);
               } else {
                   graph->InsertV();
               }
               break;
           }
           case 9: {
               string v1;
               cout << "Enter name of removable vertex: ";
               cin >> v1;
               cout << endl <<  graph->DeleteV(v1) << endl;
               break;
           }
           case 10: {
               cout << "0 - with weight, 1 - without weight: ";
               int tmp = 0;
               cin >> tmp;
               string v1, v2;
               cout << "name of out vertex: ";
               cin >> v1;
               cout << "name of in vertex: ";
               cin >> v2;
               if (tmp == 0) {
                   int _weight;
                   cout << "enter weight: ";
                   cin >> _weight;
                   cout << endl << graph->InsertE(v1,v2,_weight) << endl;
               } else {
                   cout << endl << graph->InsertE(v1, v2) << endl;
               }
               break;
           }

           case 11: {
               string v1, v2;
               cout << "name of out vertex: ";
               cin >> v1;
               cout << "name of in vertex: ";
               cin >> v2;
               cout << endl << graph->DeleteE(v1, v2) << endl;
               break;
           }

               //-------------------------------------------------------
               //Операции над вершинами
           case 12: {            //Прочитать данные о вершине
               int tmp;
               string v;
               cout << "Enter vertex name: ";
               cin >> v;
               try {
                   Vertex<basic_string<char>, int> *ver = graph->getVertexFromName(v);
                   if (ver->isDataSet()) {
                       cout << ver->getData() << endl;
                   } else
                       cout << "data not Set" << endl;
               }
               catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 13: {            //Записать данные о вершине
               int tmp;
               string v;
               cout << "Enter vertex name: ";
               cin >> v;
               cout << "Enter data: ";
               cin >> tmp;
               try {
                   Vertex<basic_string<char>, int> *ver = graph->getVertexFromName(v);
                   ver->setData(tmp);
               }
               catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 14: {            //Записать новое имя вершины
               string v1, v2;
               cout << "Enter old name of vertex: ";
               cin >> v1;
               cout << "Enter new name of vertex: ";
               cin >> v2;
               try {
                   Vertex<basic_string<char>, int> *ver = graph->getVertexFromName(v1);
                   ver->setName(v2);
               }
               catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }

               //-------------------------------------------------------
               //Операции над ребрами
           case 15: {            //Прочитать вес ребра
               int tmp;
               string v1, v2;
               cout << "Enter name of out vertex: ";
               cin >> v1;
               cout << "Enter name of in vertex: ";
               cin >> v2;
               try {
                   Edge<Vertex<string, int>, int, int> *edge1 = graph->getEdge(graph->getVertexFromName(v1),
                                                                               graph->getVertexFromName(v2));
                   if (edge1->isWeightSet())
                       cout << edge1->getWeight() << endl;
                   else
                       cout << "weight is not Set" << endl;
               } catch (exception&) {
                   cout << "Exception!\n";
               }

               break;
           }
           case 16: {            //Прочитать данные о ребре
               int tmp;
               string v1, v2;
               cout << "Enter name of out vertex: ";
               cin >> v1;
               cout << "Enter name of in vertex: ";
               cin >> v2;
               try {
                   Edge<Vertex<string, int>, int, int> *edge1 = graph->getEdge(graph->getVertexFromName(v1),
                                                                               graph->getVertexFromName(v2));
                   if (edge1->isDataSet())
                       cout << edge1->getData() << endl;
                   else
                       cout << "data is not Set" << endl;
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 17: {            //Записать данные о ребре
               int tmp;
               bool tmp1;
               string v1, v2;
               cout << "Enter name of out vertex: ";
               cin >> v1;
               cout << "Enter name of in vertex: ";
               cin >> v2;
               cout << "Enter data: ";
               cin >> tmp;
               try {
                   Edge<Vertex<string, int>, int, int> *edge1 = graph->getEdge(graph->getVertexFromName(v1),
                                                                               graph->getVertexFromName(v2));
                   edge1->setData(tmp);
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 18: {            //Записать вес ребра
               int tmp;
               bool tmp1;
               string v1, v2;
               cout << "Enter out vertex: ";
               cin >> v1;
               cout << "Enter in vertex: ";
               cin >> v2;
               cout << "Enter weight: ";
               cin >> tmp;
               try {
                   Edge<Vertex<string, int>, int, int> *edge1 = graph->getEdge(graph->getVertexFromName(v1),
                                                                               graph->getVertexFromName(v2));
                   edge1->setWeight(tmp);
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }

               //-------------------------------------------------------
               //Итератор вершин
           case 19: {                //Установить итератор вершин на начальную вершину
               vIt = vIt.beg();
               break;
           }
           case 20: {                //Установить итератор вершин на конечную вершину
               vIt = vIt.end();
               cout << endl << "==============================================" << endl;
               break;
           }
           case 21: {                //Переход к следующей позиции
               ++vIt;
               break;
           }
           case 22: {                //Прочитать вершину по текущей позиции итератора
               try {
                   cout << (*vIt)->getName() << endl;
               } catch (exception&) {
                   cout << "Exception";
               }
               break;
           }
           case 23: {                //Прочитать данные вершины по текущей позиции итератора
               try {
                   if ((*vIt)->isDataSet())
                       cout << (*vIt)->getData() << endl;
                   else
                       cout << "data is not Set" << endl;
               } catch (exception&) {
                   cout << "Exception\n";
               }
               break;
           }
           case 24: {                //Записать данные о вершины по текущей позиции итератора
               int tmp;
               cout << "Enter data: ";
               cin >> tmp;
               try {
                   (*vIt)->setData(tmp);
               } catch (exception&) {
                   cout << "Exception\n";
               }
               break;
           }
           case 25: {                //Записать имя вершины по текущей позиции итератора
               string cur;
               cout << "Enter name: ";
               cin >> cur;
               try {
                   (*vIt)->setName(cur);
               } catch (exception&) {
                   cout << "Exception";
               }
               break;
           }

               //-------------------------------------------------------
               //Итератор ребер
           case 26: {                //Установить итератор ребер на начальное ребро
               eIt = eIt.beg();
               break;
           }
           case 27: {                //Установить итератор ребер на конечное ребро
               eIt = eIt.end();
               break;
           }
           case 28: {                //Переход к следующей позиции
               ++eIt;
               break;
           }
           case 29: {                //Прочитать вершины ребра и вес по текущей позиции итератора
               try {
                   cout << (*eIt)->getVertex1()->getName() << " " << (*eIt)->getVertex2()->getName() << " ";
                   if ((*eIt)->isWeightSet())
                       cout << (*eIt)->getWeight() << endl;
                   else
                       cout << "weight is not Set" << endl;
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 30: {                //Прочитать данные ребра по текущей позиции итератора
               try {
                   if ((*eIt)->isDataSet())
                       cout << (*eIt)->getData() << endl;
                   else
                       cout << "data is not Set" << endl;
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }
           case 31: {                //Записать данные о ребре по текущей позиции итератора
               int tmp;
               cout << "Enter data: ";
               cin >> tmp;
               try {
                   (*eIt)->setData(tmp);
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           };
           case 32: {                //Записать вес ребра по текущей позиции итератора
               int tmp;
               cout << "Enter weight: ";
               cin >> tmp;
               try {
                   (*eIt)->setWeight(tmp);
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }

               //-------------------------------------------------------
               //Итератор исходящих ребер
           case 33: {
               int tmp, tmp2;        //узел
               bool ex = false;            //выход
               cout << endl << "Enter vertex: ";
               cin >> tmp;
               oIt = new Graph<Vertex<string, int>, Edge<Vertex<string, int>, int, int>>::OutputEdgeIterator(*graph,
                                                                                                             *graph->getVertex(
                                                                                                                     tmp));

               while (true) {
                   cout << endl << "Iterator of outer edges:" << endl;
                   cout << "1. beg" << endl;
                   cout << "2. end" << endl;
                   cout << "3. ++" << endl;
                   cout << "4. read vertexes and weight" << endl;
                   cout << "5. read data" << endl;
                   cout << "6. write data" << endl;
                   cout << "7. write weight" << endl;
                   cout << "10. back" << endl;
                   cout << endl << "Choose operation: ";
                   cin >> tmp2;
                   switch (tmp2) {
                       case 1: {            //Установить итератор исходящих ребер на начальное ребро
                           oIt = oIt.beg();
                           break;
                       }
                       case 2: {            //Установить итератор исходящих ребер на конечное ребро
                           oIt = oIt.end();
                           break;
                       }
                       case 3: {            //Переход к следующей позиции
                           ++oIt;
                           break;
                       }
                       case 4: {            //Прочитать вершины ребра и вес по текущей позиции итератора
                           try {
                               cout << (*oIt)->getVertex1()->getName() << " " << (*oIt)->getVertex2()->getName() << " ";
                               if ((*oIt)->isWeightSet())
                                   cout << (*oIt)->getWeight() << endl;
                               else
                                   cout << "weight is not Set" << endl;
                           } catch (exception&) {
                               cout << "Exception!\n";
                           }
                           break;
                       }
                       case 5: {            //Прочитать данные ребра по текущей позиции итератора
                           try {
                               if ((*oIt)->isDataSet())
                                   cout << (*oIt)->getData() << endl;
                               else
                                   cout << "data is not Set" << endl;
                           } catch (exception&) {
                               cout << "Exception!\n";
                           }
                           break;
                       }
                       case 6: {            //Записать данные о ребре по текущей позиции итератора
                           int tmp;
                           cout << "Enter data: ";
                           cin >> tmp;
                           try {
                               (*oIt)->setData(tmp);
                           } catch (exception&) {
                               cout << "Exception!\n";
                           }
                           break;
                       }
                       case 7: {            //Записать вес ребра по текущей позиции итератора
                           int tmp;
                           cout << "Enter weight: ";
                           cin >> tmp;
                           try {
                               (*oIt)->setWeight(tmp);
                           } catch (exception&) {
                               cout << "Exception!\n";
                           }
                           break;
                       }
                       case 10: {            //выход
                           ex = true;
                           break;
                       }
                       default: {
                           cout << "Error" << endl;
                       };
                   }
                   if (ex) break;
               }
               break;
           }
           case 34: {
               cout << "Enter weight:";
               int tmp;
               cin >> tmp;
               cout << endl;
               try {
                   TaskTwo<Vertex<string, int>, Edge<Vertex<string, int>, int, int>> taskTwo(graph, tmp);
                   vector<Vertex<string, int>*> result = taskTwo.Result();
                   for (int i = 0; i < result.size(); i++) {
                       cout << result[i]->getName() << " ";
                   }
                   cout << endl;
               } catch (exception&) {
                   cout << "Exception!\n";
               }
               break;
           }

           case 40: {                //выход
               return 0;
           }
           default: {
               cout << "Error" << endl;
           }
               break;
       }

   }
}