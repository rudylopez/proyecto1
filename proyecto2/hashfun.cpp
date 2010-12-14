#include <iostream>
#include <fstream>
#include <cstring>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>

#include "hashfun.h"
using namespace std;

hashfun::hashfun()
{
    this->nombre="mihash.dat";
    this->colisiones=50;
    ofstream file;
    file.open(this->nombre.toStdString().c_str(),ios::binary | ios::out);
}

unsigned int hashfun:: hash(QString query,int sizetable)
{
    unsigned int hashval =0;

    for(int i= 0; i<query.size();i++)
    {
        hashval= 37 * hashval + query.at(i).toAscii();

    }
    return  hashval % sizetable;
}
void hashfun::crear(int size, QString nombre, int colisiones)
{
    this->nombre=nombre;
    this->colisiones=colisiones;
    this->sizetable=size;
    ofstream file;
    file.open(this->nombre.toStdString().c_str(),ios::binary | ios::out | ios::in);
    file.write(reinterpret_cast<char *>(&size),
                            sizeof(int));
    file.write(reinterpret_cast<char *>(&colisiones),
                            sizeof(int));
    //file.write(reinterpret_cast<char *>(&size),
    //                        sizeof(int));
    int Arreglo[this->colisiones];
    for (int i=0; i<this->colisiones; i++)
    {
        Arreglo[i]=0;

    }
    for (int i=0; i<size; i++)
    {
        file.write(reinterpret_cast<char *>(&Arreglo),
                                sizeof(int)*this->colisiones);

    }


       file.close();

       //ifstream fentrada(this->nombre.toStdString().c_str(),
         //    ios::in | ios::binary);

          //fentrada.read(reinterpret_cast<char *>(&Arreglo),
          //   sizeof(int)*this->colisiones);
          //cout << Arreglo[0] << endl;
          //cout << Arreglo[1] << endl;
          //cout << Arreglo[2] << endl;
          //system("PAUSE");

}
void hashfun::adicionar(QString ip, int id)
{

    int pos=0;
    pos=hash(ip,this->sizetable);


    FILE * archivo;
    char nombrechar[this->nombre.size()];

    strcpy(nombrechar, this->nombre.toStdString().c_str());

    archivo = fopen( nombrechar, "rb+" );

    fseek(archivo,sizeof(int)*2+sizeof(int)*this->colisiones*pos,0);
    int Arreglo[this->colisiones];
    fread(&Arreglo,sizeof(int)*this->colisiones,1,archivo);
    //cout<<Arreglo[0]<<endl;
    //cout<<Arreglo[1]<<endl;
    //cout<<Arreglo[2]<<endl;
    //system("PAUSE");


    if(Arreglo[0]!=this->colisiones){
        Arreglo[Arreglo[0]+1]=id;
        Arreglo[0]=Arreglo[0]+1;
        fseek(archivo,sizeof(int)*2+sizeof(int)*this->colisiones*pos,0);
        fwrite(&Arreglo,sizeof(int)*this->colisiones,1,archivo);
    }


    //fseek(archivo,sizeof(int)*2+sizeof(int)*this->colisiones*pos,0);

    //fread(&Arreglo,sizeof(int)*this->colisiones,1,archivo);
    //cout<<Arreglo[0]<<endl;
    //cout<<Arreglo[1]<<endl;
    //cout<<Arreglo[2]<<endl;

    system("PAUSE");


       fclose(archivo);


}
QList <int> hashfun::devolverlista(QString dato)
{
    /*
     1. Aplicarle la funcion hash al dato.
     2. Ir a la posicion que devuelve la funcion hash en el archivo.
     3. Leer el arreglo de esa posicion.
     4. Leer la posicion 0 para saber cuantod id hay guardados.
     5. Hacer un for, he ir guardando cada id en el QList.
     6. Devolver el QList.


     */
    QList<int> milista;

    int pos=0;
    pos=hash(dato,this->sizetable);
    cout<<pos<<endl;

    FILE * archivo;
    char nombrechar[this->nombre.size()];

    strcpy(nombrechar, this->nombre.toStdString().c_str());

    archivo = fopen( nombrechar, "r+" );
    //

    fseek(archivo,sizeof(int)*2+sizeof(int)*this->colisiones*pos,0);
    int Arreglo[this->colisiones];
    fread(&Arreglo,sizeof(int)*this->colisiones,1,archivo);
    for (int i=0; i<Arreglo[0]+1;i++)
    {
        milista.append(Arreglo[i]);
    }
    return milista;
}
