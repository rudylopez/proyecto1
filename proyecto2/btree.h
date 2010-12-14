#ifndef BTREE_H
#define BTREE_H

#define M 4

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <data.h>
using namespace std;

typedef struct estructuraArbolB{
        estructuraArbolB(){
                for(int i = 0; i < M; i++){
                        data[i] = -1;
                        rrn[i] = apuntadores[i] = -1;
                }
                apuntadores[M] = -1;
        }
        int padre;
        int data[4];
        int apuntadores[4+1];
        int rrn[4];
}BTree;

typedef struct {
        int rrn, ptr;
        int data;
        int firstRRN;
        void addDatos(int data,int rrn, int ptr)
        {
                this->data = data;
                this->rrn = rrn;
                this->ptr = ptr;
        }
}nodo;

typedef struct{
    int registros;
    int raiz;
}headerB;

class Data;

class Btree
{
public:
    Btree();
    void createData();
    //void crearArbol();
    void agregar(int m,int x);
    void buscar(int codigo);
    void buscarAux(int pos, int codigo);
    void buscarBin(int pos, int codigo);

    void corregirPadres(int padre, int pos);
    void agregarRec(int pos,int codigo,int rrn, int ptr1, int ptr2, bool force);
    void promote(BTree *btree, int pos, nodo *n);
    void correrValores(BTree *btree, int newVal, int posVal, nodo *n, int rrn, int ptr1, int ptr2);
    int agregarLista();

    fstream binaryData;
    fstream arbolBData;
    BTree btree;
    int raiz;
    registro registrito;
    header miheader;
    headerB miheaderB;
    int sigLibre;
    ~Btree();
};

#endif // BTREE_H
