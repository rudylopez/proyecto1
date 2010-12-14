#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <QList>
#include <QString>
#include <hashfun.h>
using namespace std;

typedef struct
{
    int id;
    double time;
    char source[16];
    char destination[16];
    char protocol[20];
    char info[256];
    int siguiente;
}registro;

typedef struct
{
    int registros;
    int libre;
}header;

class Btree;

class Data
{
public:    
    registro reg;
    header miheader;
    hashfun hash;
    fstream binary;
    fstream file;
    char temp[256];
    QString key;
    QList <registro> lista;
    Btree *btree;
    QList <registro> agregar();
    Data(int cuantos);
    int agregar(registro registrito);
    };

#endif // DATA_H
