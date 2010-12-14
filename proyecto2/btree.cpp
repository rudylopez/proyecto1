#include "btree.h"
#include "data.h"


Btree::Btree()
{
    arbolBData.open("ArbolB.dat", ios::out | ios::binary);
    //binaryData.open("binary.dat",ios::in | ios::out| ios::binary);
    miheaderB.registros = 0;
    miheaderB.raiz = -1;
    arbolBData.write((char*)&miheaderB,sizeof(miheaderB));
    this->sigLibre = sizeof(headerB);
    this->raiz = -1;

    createData();
    arbolBData.flush();
    arbolBData.close();

}

void Btree::createData()
{
    BTree btree;
    for(int i = 0; i < 100; i++)
        {
            arbolBData.write((char *)&btree,sizeof(BTree));

        }

}

/*void Btree::crearArbol()
{
    int tInicial, tFinal;
    cout << "---------------------------" << endl;
    cout << "Creando arbol." <<endl;
    tInicial = clock();
    binaryData.read((char*)&miheader,sizeof(header));
    int cant = miheader.registros;

    for (int i = 0; i<cant; i++)
    {
            binaryData.read((char*)&registrito,sizeof(registro));
            agregar(i,registrito.id);
    }
    tFinal = clock();
    cout << " Terminado."<<endl;
    cout << "---------------------------" << endl;
    cout << "Tiempo tomado: " << tFinal - tInicial << " ms."<< endl;
    cout << "---------------------------" << endl;
}
*/

void Btree::agregar(int rrn, int codigo)
{

    fstream prueba;
    headerB mio;
    //arbolBData.open("ArbolB.dat", ios::binary | ios::in | ios::out);
    prueba.open("ArbolB.dat", ios::binary | ios::in | ios::out);
    prueba.read((char*)&mio, sizeof(headerB));
    //arbolBData.read((char*)&miheaderB,sizeof(headerB));


    cout << "Raiz"<< mio.raiz<< endl;

    if(mio.raiz == -1)
    {
            cout<<"llega hasta aqui"<<endl;

            //arbolBData.seekg(0);
            prueba.seekg(0);
            prueba.read((char*)&mio, sizeof(headerB));
            //arbolBData.read((char *)&miheaderB,sizeof(headerB));

            //miheaderB.raiz = 0;
            //mio.raiz=0;
            //agregarLista();

            //arbolBData.seekg(miheaderB.raiz*sizeof(BTree) + sizeof(headerB));
            prueba.seekg(mio.raiz*sizeof(BTree)+sizeof(headerB));
            //arbolBData.read((char*)&btree,sizeof(BTree));
            prueba.read((char*)&btree,sizeof(BTree));
            btree.data[0] = codigo;
            btree.padre = -1;
            btree.rrn[0] = rrn;
            //arbolBData.seekg(miheaderB.raiz*sizeof(BTree) + sizeof(headerB));
            prueba.seekg(mio.raiz*sizeof(BTree)+sizeof(headerB));
            //arbolBData.write((char*)&btree,sizeof(BTree));
            prueba.write((char*)&btree, sizeof(BTree));
            //this->agregarLista();
    }
    else
    {
            agregarRec(miheaderB.raiz, codigo, rrn, -1, -1,false);
            this->agregarLista();
    }

    arbolBData.seekp(0);
    miheaderB.registros++;
    miheader.libre = miheaderB.registros*sizeof(Btree) + sizeof(miheaderB);

    arbolBData.write((char *)&miheaderB,sizeof(headerB));

    arbolBData.close();
}

void Btree::buscar(int codigo)
{
    int tInicial, tFinal;
    arbolBData.seekg(0);
    arbolBData.read((char*)&miheaderB,sizeof(headerB));
    cout << "---------------------------" << endl;
    cout << "Buscando codigo: " << codigo <<endl;
    tInicial = clock();
    buscarAux(miheaderB.raiz, codigo);
    tFinal = clock();
    cout << "---------------------------" << endl;
    cout << "Tiempo de busqueda: " << tFinal - tInicial << " ms." << endl;
    cout << "---------------------------" << endl;
}

void Btree::buscarAux(int pos, int codigo)
{
    arbolBData.seekg(pos*sizeof(BTree) + sizeof(headerB));
    BTree btree;
    arbolBData.read((char*)&btree,sizeof(BTree));

    for(int i = 0; i < M; i++)
    {
            if(btree.data[i] == codigo)
        {
                    cout << "Codigo: " << codigo << endl;
                    binaryData.seekg(codigo*sizeof(BTree) + sizeof(headerB));
                    binaryData.read((char *)&registrito,sizeof(registro));
                    cout << "Id: " << registrito.id << endl;
                    cout << "Info: " << registrito.info << endl;
                    return;
        }
            else if(btree.data[i] > codigo)
            {
                    buscarAux(btree.apuntadores[i], codigo);
                    return;
            }
            else if(btree.data[i] == -1)
            {
                    if(btree.apuntadores[i] == -1)
                {
                            cout << "No se encontro codigo " << codigo << endl;
                            return;
                }
                    buscarAux(btree.apuntadores[i], codigo);
                    return;
            }
    }

    if(btree.apuntadores[M] != -1)
        {
            buscarAux(btree.apuntadores[M], codigo);
        }
    else
        {
            cout << "No se encontro codigo " << codigo << "."<< endl;
        }
}

void Btree::buscarBin(int pos, int codigo)
{
    arbolBData.seekg(pos*sizeof(BTree) + sizeof(headerB));
    BTree btree;
    arbolBData.read((char*)&btree,sizeof(BTree));
    int bajo= 0, alto = M-1, medio = alto/2;

    while(bajo <= alto)
    {
            if(btree.data[medio] == codigo)
                {
                    cout << "Codigo: " << codigo;
                    binaryData.seekg(codigo*sizeof(registro) + sizeof(header));
                    binaryData.read((char*)&registrito,sizeof(registro));
                    cout << "Id: " << registrito.id << endl;
                    cout << "Info: " << registrito.info << endl;
                    return;
                }
            else if(btree.data[medio] == -1 || btree.data[medio] > codigo)
                    alto = medio-1;
            else
            {
                    bajo = medio+1;
            medio = (alto+bajo)/2;
            }
    }
    if(codigo < btree.data[medio])
        {
            if(btree.apuntadores[medio] != -1)
                {
                    buscarBin(btree.apuntadores[medio], codigo);
                    return;
                }
        }
    else if(btree.data[medio] != -1)
        {
            if(btree.apuntadores[medio+1] != -1)
                {
                    buscarBin(btree.apuntadores[medio+1], codigo);
                    return;
                }
        }
    cout << "No se encontro " << codigo << "." << endl;
}

void Btree::corregirPadres(int padre, int pos)
{
    arbolBData.seekg(pos*sizeof(BTree) + sizeof(headerB));
    BTree btree;
    arbolBData.read((char*)&btree,sizeof(BTree));
    if(btree.padre != padre){
            arbolBData.seekg(pos*sizeof(BTree) + sizeof(headerB));
            btree.padre = padre;
            arbolBData.write((char*)&btree,sizeof(BTree));
    }
}

void Btree::agregarRec(int pos, int codigo, int rrn, int ptr1, int ptr2, bool force)
{
    BTree btree;
    arbolBData.seekg(pos*sizeof(BTree) + sizeof(headerB));
    arbolBData.read((char*)&btree,sizeof(BTree));
    nodo n;

    if(force == true)
        {
            for(int i = 0; i < M; i++)
                {
                    if(codigo < btree.data[i] || btree.data[i] == -1)
                        {
                            correrValores(&btree, codigo, i, &n, rrn, ptr1, ptr2);
                            if(n.rrn != -1)
                                {
                                    promote(&btree, pos, &n);
                                    arbolBData.seekg(pos);
                                    arbolBData.write((char *)&btree,sizeof(BTree));
                                    return;
                                }
                        }
                }
            correrValores(&btree, codigo, M, &n, rrn, ptr1, ptr2);
            promote(&btree, pos, &n);
            arbolBData.seekg(pos);
            arbolBData.write((char*)&btree,sizeof(BTree));
        }

    else
        {
            for(int i = 0; i < M; i++)
                {
                    if(codigo < btree.data[i] || btree.data[i] == -1)

                        {
                            if(btree.apuntadores[i] == -1)
                                {
                                    correrValores(&btree, codigo, i, &n, rrn, ptr1, ptr2);
                                    if(n.rrn != -1)
                                        {
                                            promote(&btree, pos, &n);
                                            arbolBData.seekg(pos);
                                            arbolBData.write((char*)&btree,sizeof(BTree));
                                        }
                                }
                            else
                            {
                                    agregarRec(btree.apuntadores[i],codigo, rrn, ptr1, ptr2, force);
                                    return;
                            }
                        }
                }
            if( btree.apuntadores[M] == -1)
                {
                    correrValores(&btree, codigo, M, &n, rrn, ptr1, ptr2);
                    if(n.rrn != -1)
                        {
                            promote(&btree, pos, &n);
                            arbolBData.seekg(pos);
                            arbolBData.write((char*)&btree,sizeof(BTree));
                        }
                }
            else
                {
                    agregarRec(btree.apuntadores[M],codigo, rrn, ptr1, ptr2, force);
                }
        }
}

void Btree::promote(BTree *btree, int pos, nodo *n)
{
    arbolBData.seekg(0);
    arbolBData.read((char*)&miheaderB,sizeof(miheaderB));

    if(btree->padre == -1)
    {
            BTree nuevaRaiz;
            int posRaiz = agregarLista();
            int posHijo2 = agregarLista();
            miheaderB.raiz = posRaiz;
            arbolBData.seekg(miheaderB.raiz*sizeof(BTree) + sizeof(headerB));
            nuevaRaiz.apuntadores[0] = pos;
            nuevaRaiz.rrn[0] = btree->rrn[M/2];
            nuevaRaiz.data[0] = btree->data[M/2];
            nuevaRaiz.apuntadores[1] = posHijo2;
            nuevaRaiz.padre = -1;
            arbolBData.write((char*)&nuevaRaiz,sizeof(BTree));
            nuevaRaiz.padre = posRaiz;
            btree->padre = posRaiz;
            miheaderB.raiz = posRaiz;
            arbolBData.seekp(0);
            arbolBData.write((char*)&miheaderB,sizeof(miheaderB));

                    for(int i = 1; i < M/2; i++)

                        {
                        nuevaRaiz.data[i-1] = btree->data[M/2+ i];
                        nuevaRaiz.rrn[i-1] = btree->rrn[M/2 + i];
                        btree->data[M/2+i] = -1;
                        btree->rrn[M/2+i] = -1;
                        nuevaRaiz.apuntadores[i-1] = btree->apuntadores[M/2+i];
                        btree->apuntadores[M/2+i] = -1;
                        }
            nuevaRaiz.apuntadores[M/2-1] = btree->apuntadores[M];
            nuevaRaiz.apuntadores[M/2] = n->ptr;
            nuevaRaiz.data[M/2-1] = n->data;
            nuevaRaiz.rrn[M/2-1] = n->rrn;
            btree->apuntadores[M] = -1;
            btree->data[M/2] = -1;
            btree->rrn[M/2] = -1;
            for(int i = 0; i <= M/2; i++)
                {
                 if(btree->apuntadores[i] != -1)
                        {
                            corregirPadres(pos, btree->apuntadores[i]);
                        }
                    if(nuevaRaiz.apuntadores[i] != -1)
                        {
                            corregirPadres(posHijo2, nuevaRaiz.apuntadores[i]);
                        }
                }
            arbolBData.seekg(posHijo2*sizeof(BTree) + sizeof(headerB));
            arbolBData.write((char*)&nuevaRaiz,sizeof(BTree));
    }

    else
    {
            int posHijo2 = agregarLista();
            int newRrn = btree->rrn[M/2];
            int newData = btree->data[M/2];
            int newPadre = btree->padre;
            BTree nuevoHijo;
            nuevoHijo.padre = newPadre;
            for(int i = 1; i < M/2; i++)
                {
                    nuevoHijo.data[i-1] = btree->data[M/2+i];
                    nuevoHijo.rrn[i-1] = btree->rrn[M/2 + i];
                    btree->data[M/2+i] = -1;
                    btree->rrn[M/2+i] = -1;
                    nuevoHijo.apuntadores[i-1] = btree->apuntadores[M/2 + i];
                    btree->apuntadores[M/2+i] = -1;
                }
            nuevoHijo.apuntadores[M/2-1] = btree->apuntadores[M];
            btree->apuntadores[M] = -1;
            nuevoHijo.apuntadores[M/2] = n->ptr;
            nuevoHijo.data[M/2-1] = n->data;
            nuevoHijo.rrn[M/2-1] = n->rrn;
            btree->data[M/2] = -1;
            btree->rrn[M/2] = -1;
            for(int i = 0; i <= M/2; i++)
                {
                    if(btree->apuntadores[i] != -1)
                        {
                            corregirPadres(pos, btree->apuntadores[i]);
                        }
                    if(nuevoHijo.apuntadores[i] != -1)
                        {
                            corregirPadres(posHijo2, nuevoHijo.apuntadores[i]);
                        }
                }
            arbolBData.seekg(posHijo2*sizeof(BTree) + sizeof(headerB));
            arbolBData.write((char *)&nuevoHijo,sizeof(BTree));
            agregarRec(newPadre,newData, newRrn, pos, posHijo2, true);
    }
}

void Btree::correrValores(BTree *btree, int newVal, int posVal, nodo *n, int rrn, int ptr1, int ptr2)
{
    if(posVal == 0)
    {
            n->addDatos(btree->data[M-1], btree->rrn[M-1], btree->apuntadores[M]);
            btree->apuntadores[M] = btree->apuntadores[M-1];
            for(int i = M-1; i > 0; i--)
            {
                    btree->data[i] = btree->data[i-1];
                    btree->rrn[i] = btree->rrn[i-1];
                    btree->apuntadores[i] = btree->apuntadores[i-1];
            }
            btree->data[0] = newVal;
            btree->rrn[0] = rrn;
            btree->apuntadores[0] = ptr1;
            btree->apuntadores[1] = ptr2;
    }
    else if(posVal == M-1)
    {
            n->addDatos(btree->data[posVal], btree->rrn[posVal], btree->apuntadores[M]);
            btree->data[posVal] = newVal;
            btree->rrn[posVal] = rrn;
            if(ptr1 != -1)
                    btree->apuntadores[posVal] = ptr1;
            if(ptr2 != -1)
                    btree->apuntadores[M] = ptr2;
    }
    else if(posVal == M)
    {
            btree->apuntadores[M] = ptr1;
            n->addDatos(newVal, rrn, ptr2);
    }
    else
    {
            n->addDatos(btree->data[M-1], btree->rrn[M-1], btree->apuntadores[M]);
            btree->apuntadores[M] = btree->apuntadores[M-1];
            for(int i = M-1; i >posVal; i--)
            {
                    btree->data[i] = btree->data[i-1];
                    btree->rrn[i] = btree->rrn[i-1];
                    btree->apuntadores[i] = btree->apuntadores[i-1];
            }
            btree->data[posVal] = newVal;
            btree->rrn[posVal] = rrn;
            if(ptr1 != -1)
                    btree->apuntadores[posVal] = ptr1;
            if(ptr2 != -1)
                    btree->apuntadores[posVal+1] = ptr2;
    }
}

int Btree::agregarLista()
{

    sigLibre += sizeof(BTree);
    int retVal = sigLibre;
    return retVal;
}

Btree::~Btree()
{
    arbolBData.seekg(0, ios::beg);
    arbolBData.write((char *)&miheaderB,sizeof(miheaderB));
    arbolBData.close();
    binaryData.close();
}
