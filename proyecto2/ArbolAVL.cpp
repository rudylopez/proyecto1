/*#include <iostream>
using namespace std;

template<class TIPO>class ArbolAVL;
template<class TIPO>
class Nodo{
    friend class ArbolAVL<TIPO>;
    public:
        Nodo();
        Nodo(TIPO d,int pos, int p);
        TIPO dato;
        int posicion;
        int padre;
        int izquierdo;
        int derecho;
        int altura;
};
template <class TIPO>
Nodo<TIPO>::Nodo(){
    padre=-1;
    izquierdo=-1;
    derecho=-1;
    altura=-1;
}
template <class TIPO>
Nodo<TIPO>::Nodo(TIPO d,int pos,int p){
    dato=d;
    posicion=pos;
    padre=p;
    izquierdo=-1;
    derecho=-1;
    altura=0;
}
template <class TIPO>
class ArbolAVL{
    private:
        int raiz;
        int mayor(int a, int b);
        void print(int n, FILE *f);
        void insert(int n, TIPO d,int pos, FILE *f);
        int obtenerRaiz(FILE *f);
        void actualizarRaiz(int v,FILE *f);
        int search(int n,TIPO d, FILE *f);
    public:
        ArbolAVL();
        ~ArbolAVL();
        void insertar(TIPO d, int pos);
        int buscar(TIPO d);
        void imprimir();
        void balancear(int n,FILE *f);
        void rotacionSimple(int n,bool izq,FILE *f);
        void rotacionDoble(int n,bool izq,FILE *f);
        void actualizarAltura(int n,FILE *f);
        int altura(int n,FILE *f);
};
template<class TIPO>
ArbolAVL<TIPO>::ArbolAVL(){

}
template<class TIPO>
ArbolAVL<TIPO>::~ArbolAVL(){

}
template<class TIPO>
int ArbolAVL<TIPO>::obtenerRaiz(FILE *f){
    fseek(f,0,SEEK_SET);
    fread(&raiz,sizeof(int),1,f);
    return raiz;
}
template<class TIPO>
void ArbolAVL<TIPO>::actualizarRaiz(int v,FILE *f){
    raiz=v;
    fseek(f,0,SEEK_SET);
    fwrite(&raiz,sizeof(int),1,f);
}
template<class TIPO>
void ArbolAVL<TIPO>::insertar(TIPO d,int pos){
    FILE *archivo;
    archivo=fopen("PrimaryIndex.dat","rb+");
    if(archivo==NULL){archivo=fopen("PrimaryIndex.dat","ab");}
    fseek(archivo,0,SEEK_END);
    if(ftell(archivo)==0){
        Nodo<TIPO> tmp(d,pos,-1);raiz=4;
        actualizarRaiz(4,archivo);
        fwrite(&tmp,sizeof(Nodo<TIPO>),1,archivo);
    }
    else{
        insert(obtenerRaiz(archivo),d,pos,archivo);
    }
    fclose(archivo);
}
template<class TIPO>
void ArbolAVL<TIPO>::insert(int n, TIPO d,int pos,FILE *f){
    cout<<"ERROR"<<endl;
    Nodo<TIPO> buffer;
    fseek(f,n,SEEK_SET);
    fread(&buffer,sizeof(buffer),1,f);
    if(d<buffer.dato){
        if(buffer.izquierdo==-1){
            fseek(f,0,SEEK_END);buffer.izquierdo=ftell(f);
            Nodo<TIPO> tmp(d,pos,n);fwrite(&tmp,sizeof(Nodo<TIPO>),1,f);
            fseek(f,n,SEEK_SET);fwrite(&buffer,sizeof(Nodo<TIPO>),1,f);
        }
        else{
            insert(buffer.izquierdo,d,pos,f);
        }
    }
    else{
        if(d>buffer.dato){
            if(buffer.derecho==-1){
                fseek(f,0,SEEK_END);buffer.derecho=ftell(f);
                Nodo<TIPO> tmp(d,pos,n);fwrite(&tmp,sizeof(Nodo<TIPO>),1,f);
                fseek(f,n,SEEK_SET);fwrite(&buffer,sizeof(Nodo<TIPO>),1,f);
            }
            else{
                insert(buffer.derecho,d,pos,f);
            }
        }
    }
    balancear(n,f);
    actualizarAltura(n,f);
}
template<class TIPO>
void ArbolAVL<TIPO>::imprimir(){
    FILE *archivo;
    archivo=fopen("PrimaryIndex.dat","rb+");
    if(archivo==NULL){archivo=fopen("PrimaryIndex.dat","ab");}
    fseek(archivo,0,SEEK_END);
    if(ftell(archivo)==0){
        cout<<"Arbol Vacio"<<endl;
    }
    else{
        print(obtenerRaiz(archivo),archivo);
    }
    fclose(archivo);
}
template<class TIPO>
void ArbolAVL<TIPO>::print(int n, FILE *f){
    if(n!=-1){
        Nodo<TIPO> buffer;
        fseek(f,n,SEEK_SET);
        fread(&buffer,sizeof(Nodo<TIPO>),1,f);
        print(buffer.izquierdo,f);
        cout<<buffer.dato<<"    "<<buffer.posicion<<endl;
        print(buffer.derecho,f);
    }
}
template<class TIPO>
int ArbolAVL<TIPO>::mayor(int a, int b){
    if(a>b){
        return a;
    }
    return b;
}
template<class TIPO>
void ArbolAVL<TIPO>::balancear(int n,FILE *f){
    if(n!=-1){
        Nodo<TIPO> buffer;
        fseek(f,n,SEEK_SET);
        fread(&buffer,sizeof(Nodo<TIPO>),1,f);
        if((altura(buffer.izquierdo,f)-altura(buffer.derecho,f))==2){
            fseek(f,buffer.izquierdo,SEEK_SET);
            fread(&buffer,sizeof(Nodo<TIPO>),1,f);
            if(altura(buffer.izquierdo,f)>=altura(buffer.derecho,f)){
                rotacionSimple(n,true,f);
            }
            else{
                rotacionDoble(n,true,f);
            }
        }
        else{
            if((altura(buffer.derecho,f)-altura(buffer.izquierdo,f))==2){
                fseek(f,buffer.derecho,SEEK_SET);
                fread(&buffer,sizeof(Nodo<TIPO>),1,f);
                if(altura(buffer.derecho,f)>=altura(buffer.izquierdo,f)){
                    rotacionSimple(n, false,f);
                }
                else{
                    rotacionDoble(n, false,f);
                }
            }
        }
    }
}
template<class TIPO>
void ArbolAVL<TIPO>::rotacionSimple(int n, bool izq,FILE *f){
    int x,y;
    Nodo<TIPO> tmp,buffer,padre;
    fseek(f,n,SEEK_SET);fread(&buffer,sizeof(Nodo<TIPO>),1,f);
    if(izq){
        x=buffer.izquierdo;
        y=buffer.padre;
        fseek(f,x,SEEK_SET);fread(&tmp,sizeof(Nodo<TIPO>),1,f);
        buffer.izquierdo=tmp.derecho;buffer.padre=x;
        tmp.derecho=n;tmp.padre=y;
        if(y!=-1){
            fseek(f,y,SEEK_SET);fread(&padre,sizeof(Nodo<TIPO>),1,f);
            if(padre.izquierdo==n){
                padre.izquierdo=x;
            }
            else{
                padre.derecho=x;
            }
            fseek(f,y,SEEK_SET);fwrite(&padre,sizeof(Nodo<TIPO>),1,f);
        }
        fseek(f,x,SEEK_SET);fwrite(&tmp,sizeof(Nodo<TIPO>),1,f);
    }
    else{
        x=buffer.derecho;
        y=buffer.padre;
        fseek(f,x,SEEK_SET);fread(&tmp,sizeof(Nodo<TIPO>),1,f);
        buffer.derecho=tmp.izquierdo;buffer.padre=x;
        tmp.izquierdo=n;tmp.padre=y;
        if(y!=-1){
            fseek(f,y,SEEK_SET);fread(&padre,sizeof(Nodo<TIPO>),1,f);
            if(padre.izquierdo==n){
                padre.izquierdo=x;
            }
            else{
                padre.derecho=x;
            }
            fseek(f,y,SEEK_SET);fwrite(&padre,sizeof(Nodo<TIPO>),1,f);
        }
        fseek(f,x,SEEK_SET);fwrite(&tmp,sizeof(Nodo<TIPO>),1,f);
    }
    fseek(f,n,SEEK_SET);
    fwrite(&buffer,sizeof(Nodo<TIPO>),1,f);
    actualizarAltura(n,f);
    actualizarAltura(x,f);
    if(obtenerRaiz(f)==n){actualizarRaiz(x,f);}
}
template<class TIPO>
void ArbolAVL<TIPO>::rotacionDoble(int n, bool izq, FILE *f){
    Nodo<TIPO> buffer;
    fseek(f,n,SEEK_SET);
    fread(&buffer,sizeof(Nodo<TIPO>),1,f);
    if(izq){
        rotacionSimple(buffer.izquierdo,false,f);
        rotacionSimple(n,true,f);
    }
    else{
        rotacionSimple(buffer.derecho,true,f);
        rotacionSimple(n,false,f);
    }
}
template<class TIPO>
void ArbolAVL<TIPO>::actualizarAltura(int n, FILE *f){
    if(n!=-1){
        Nodo<TIPO> buffer;
        fseek(f,n,SEEK_SET);
        fread(&buffer,sizeof(Nodo<TIPO>),1,f);
        buffer.altura = mayor(altura(buffer.izquierdo,f),altura(buffer.derecho,f)) + 1;
        fseek(f,n,SEEK_SET);
        fwrite(&buffer,sizeof(Nodo<TIPO>),1,f);
    }
}
template<class TIPO>
int ArbolAVL<TIPO>::altura(int n,FILE *f){
    Nodo<TIPO> buffer;
    if(n==-1){
        return -1;
    }
    else{
        fseek(f,n,SEEK_SET);
        fread(&buffer,sizeof(Nodo<TIPO>),1,f);
        return buffer.altura;
    }
}
template<class TIPO>
int ArbolAVL<TIPO>::buscar(TIPO d){
    FILE *archivo;
    archivo=fopen("PrimaryIndex.dat","rb+");
    if(archivo==NULL){archivo=fopen("PrimaryIndex.dat","ab");}
    fseek(archivo,0,SEEK_END);
    if(ftell(archivo)==0){
        return -1;
    }
    else{
        return search(obtenerRaiz(archivo),d,archivo);
    }
    fclose(archivo);
}
template<class TIPO>
int ArbolAVL<TIPO>::search(int n,TIPO d, FILE *f){
    if(n!=-1){
        Nodo<TIPO> buffer;
        fseek(f,n,SEEK_SET);
        fread(&buffer,sizeof(Nodo<TIPO>),1,f);
        if(d<buffer.dato){
            return search(buffer.izquierdo,d,f);
        }
        else{
            if(d>buffer.dato){
                return search(buffer.derecho,d,f);
            }
            else{
                return buffer.posicion;
            }
        }
    }
    return -1;
}
int main(){
   ArbolAVL<long> ArbolInt;
   ArbolInt.insertar(1,0);
   ArbolInt.insertar(2,8);
   ArbolInt.insertar(3,9);
   ArbolInt.insertar(4,11);
   ArbolInt.insertar(5,45);
   ArbolInt.insertar(6,34);
   ArbolInt.insertar(7,67);
   ArbolInt.insertar(8,98);
   ArbolInt.insertar(9,23);
   ArbolInt.insertar(10,34);
   ArbolInt.insertar(11,22);
   ArbolInt.insertar(12,13);
   ArbolInt.insertar(13,23);
   ArbolInt.insertar(14,45);
   ArbolInt.insertar(15,67);
   ArbolInt.insertar(16,78);
   ArbolInt.insertar(17,89);
   ArbolInt.insertar(18,11);
   ArbolInt.insertar(19,12);
   ArbolInt.insertar(20,13);
   ArbolInt.imprimir();
   cout<<endl;
   cout<<"Dato: 20  Posicion: "<<ArbolInt.buscar(20)<<endl;
   return 0;
}
*/
