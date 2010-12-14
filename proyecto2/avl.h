#ifndef AVL_H
#define AVL_H

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
using namespace std;

typedef struct
{
    int posicion;
    int padre;
    int izquierdo;
    int derecho;
    int altura;
}Nodo;

class AVL
{
public:
    AVL();
    Nodo nodo;
    AVL(int pos, int p);
    void insertar(int pos);
    int buscar(char protocolo[20]);
    void balancear(int n,fstream *f);
    void rotacionSimple(int n,bool izq,fstream*f);
    void rotacionDoble(int n,bool izq,fstream *f);
    void actualizarAltura(int n,fstream *f);
    int altura(int n,fstream *f);

private:
    int raiz;
    int mayor(int a, int b);
    void print(int n, fstream *f);
    void insert(int n, int pos, fstream *f);
    int obtenerRaiz(fstream *f);
    void actualizarRaiz(int v,fstream *f);
    int search(int n,fstream *f);

};

#endif // AVL_H
