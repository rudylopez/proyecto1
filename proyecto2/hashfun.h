#ifndef HASHFUN_H
#define HASHFUN_H
#include <QString>
#include <QList>
#include <string>

class hashfun
{
public:
    hashfun();
    QString nombre;
    int colisiones;
    int sizetable;
   unsigned int hash(QString query,int sizetable);
   void crear(int size, QString nombre, int colisiones);
   void adicionar(QString dato, int id);
   QList <int> devolverlista(QString dato);
};

#endif // HASHFUN_H
