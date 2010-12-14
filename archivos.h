#ifndef ARCHIVOS_H
#define ARCHIVOS_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mapabits.h"
#include "types.h"


void imprimirHeader(FILE *arch);
void setbit(int cual, FILE *arch, numero valorBit);
void setTagEntries(char *tipoSong, int inodo, header h, FILE *arch);
void setFileEntries(char *filen, int inodo, header h, FILE *arch);
FE getFileEntrie(FILE *arch);
TE getTagEntry(FILE *arch, header h);
void setHeader(header tmp, FILE *disco)
{
    fseek(disco,0,0);
    fwrite(&tmp,1,1024,disco);
}
header getHeader(FILE *disco)
{
    header tmp;
    fseek(disco,0,0);
    fread(&tmp,1,sizeof(header),disco);
    return tmp;
}
int tamanio_archivo(FILE * origen){
        int tam=0;
        fseek(origen,0,SEEK_END);
        tam = ftell(origen);
        return tam;
}


void imprimirHeader(FILE *arch)
{
    header tmp;
    fseek(arch,0,0);
    fread(&tmp,1,sizeof(header),arch);
    printf("\n cantidad de bloques:, ""%d", tmp.cant_bloques);
    printf("\n Tamano de bits:, ""%d", tmp.tam_mbits);
    printf("\n bloques FE:, ""%d", tmp.cant_b_fileEntries);
    printf("\n bloques TE:, ""%d", tmp.cant_b_tagsEntries);
    printf("\n Tag Entries Usados:, ""%d", tmp.TE_used);
    printf("\n FILE Entries Usados:, ""%d", tmp.FE_used);

}
void setTagEntries(char * tipoSong,int inodo,header h,FILE *arch)
{
    TE tmp,tmp2;
    tmp.pointer=inodo;
    strcpy(tmp.tag,tipoSong);
    fseek(arch,h.TE_POS,0);
    int i;
    int j;
    for(i=0;i<h.cant_b_tagsEntries;i++)
    {
        for(j=0;j<32;j++)
        {
            fread(&tmp2,1,sizeof(TE),arch);
            if(strcmp(tmp2.tag,"~")==0)
            {
                fseek(arch,-sizeof(TE),SEEK_CUR);
                fwrite(&tmp,1,sizeof(TE),arch);
                h.TE_used=h.TE_used+1;
                setHeader(h,arch);
                j=32;
                i=h.cant_b_tagsEntries;
            }
        }
    }

}

TE getTagEntry(FILE *arch,header h)
{
    TE tmp;
    fread(&tmp,1,sizeof(TE),arch);
    return tmp;
}
void crearArchivo(numero cantidadBloques,numero cant_b_FE,numero cant_b_TE, const char *nombre)
{
    FILE *arch=0;
    arch=fopen(nombre,"wb+");
    if(arch==0)
    {
        printf("error");
        exit(2);
    }
    //numero ofset= cantidadBloques*1024-1;
    //printf("%u",ofset);
    char bloque[1024];
    for(int r=0;r<1024;r++)
    {
        bloque[r]='\0';
    }
    int i;
    for(i=0;i<cantidadBloques;i++)
    {
        fwrite(bloque,1,1024,arch);
    }
    // cantidad de bloques del mapa de bits
    numero bloquesMapaBit;
    bloquesMapaBit=cantidadBloques/8192;
    if((cantidadBloques%8192)!=0)
        bloquesMapaBit++;

    //fin calculo de bloques de  mapa de bits

    //creacion del HEADER
    header tmp;
    tmp.cant_bloques=cantidadBloques;
    tmp.cant_b_fileEntries=cant_b_FE;
    tmp.cant_b_tagsEntries=cant_b_TE;
    tmp.FE_used=0;
    tmp.TE_used=0;
    tmp.tam_mbits=bloquesMapaBit;
    tmp.FE_POS=((1+tmp.tam_mbits)*1024);
    tmp.TE_POS=(1+tmp.tam_mbits+tmp.cant_b_fileEntries)*1024;
    setHeader(tmp,arch);
    setbit(0,arch,1);
    setbit(2,arch,1);
    int totalBloquesUsados=bloquesMapaBit+cant_b_FE+cant_b_TE+1;
    int c;
    for(c=0;c<totalBloquesUsados;c++)
    {
        setbit(c,arch,1);
    }

    //llenar TAG entries
    int d;
    int s;
    TE tags;
    tags.pointer-1;
    strcpy(tags.tag,"~");
    fseek(arch,(1+tmp.cant_b_fileEntries+tmp.tam_mbits)*1024,0);
    for(d=0;d<tmp.cant_b_tagsEntries;d++)
    {
        for(s=0;s<32;s++)
        {
            fwrite(&tags,1,sizeof(TE),arch);
        }
    }
    //llenar los file Entries
    FE fileEntries;
    strcpy(fileEntries.filename,"~");
    fileEntries.pointer_INodo=-1;
    fseek(arch,tmp.FE_POS,0);
    for(d=0;d<tmp.cant_b_fileEntries;d++)
    {
        for(s=0;s<1024/sizeof(FE);s++)
        {
            fwrite(&fileEntries,1,sizeof(FE),arch);
        }
    }

    setHeader(tmp,arch);
    imprimirMapa(arch);
    int f=getFreeBit(arch);
    printf("bloque libre %d", f);
    fclose(arch);
    exit(1);

}


//sets y gets


void setFileEntries(char *filen,int inodo,header h,FILE *arch)
{
    FE tmp,tmp2;
    strcpy(tmp.filename,filen);
    tmp.pointer_INodo=inodo;

    fseek(arch,h.FE_POS,0);
    int i;
    int j;
    for(i=0;i<h.cant_b_fileEntries;i++)
    {
        for(j=0;j<1024/h.cant_b_fileEntries;j++)
        {
            fread(&tmp2,1,sizeof(FE),arch);
              if(strcmp(tmp2.filename,"~")==0)
            {
                fseek(arch,-sizeof(FE),SEEK_CUR);
                fwrite(&tmp,1,sizeof(FE),arch);
                h.FE_used=h.FE_used+1;
                setHeader(h,arch);
                j=1024/h.cant_b_fileEntries;
                i=h.cant_b_fileEntries;
            }
        }
    }

}
FE getFileEntrie(FILE *arch)
{
    FE tmp;
    fread(&tmp,1,sizeof(FE),arch);
    return tmp;
}
// fin de los tags files  y el crear


void leerBloque(int pos, void *buffer, FILE *arch)
{
    fseek(arch,pos*1024,0);
    fread(buffer,1,1024,arch);
}

int pos_libres(FILE *arch){

 header tempH=getHeader(arch);
 int mbits;
                mbits=tempH.cant_bloques/(8*1024);
                if(tempH.cant_bloques%(8192)!=0){
                        mbits++;
                }
        int m;
        m=getFreeBit(arch);
        setbit(m,arch,1);
        return m;
}
void escribirBloque(int pos,void *buffer,FILE *arch,int size=1024)
{
    fseek(arch,pos*1024,0);
    fwrite(buffer,1,size,arch);
}

int getBloque(inodo &nodo, int bloque, FILE *arch)
{
    int free,free2;
    if(bloque<=11)
    {
        printf("\n yes");
        if(nodo.pointer_directo[bloque]==-1)
        {   free=pos_libres(arch);
             nodo.pointer_directo[bloque]=free;
            return free;
        }else
        {
            return nodo.pointer_directo[bloque];
        }
    }else if(bloque<=267)
    {

        int buffer[256];

        bloque=bloque-12;
        if(nodo.pointer_indirecto==-1)
        {
            free=pos_libres(arch);
             printf("\n free: %d",free);
            nodo.pointer_indirecto=free;
            for(int i=0;i<256;i++){
                buffer[i]=-1;
            }

            escribirBloque(free,(char *)buffer,arch);
        }
        leerBloque(nodo.pointer_indirecto,(char *)buffer,arch);
        int blq=buffer[bloque];
        if(blq==-1)
        {
            blq=pos_libres(arch);
            buffer[bloque]=blq;
            escribirBloque(nodo.pointer_indirecto,(char*)buffer,arch);
        }

        return blq;
    }else if(bloque<=65084)
    {
        int buffer1[256];
        int buffer2[256];

        if(nodo.pointer_indirecto_doble==-1)
        {
            free=pos_libres(arch);
            printf("\n DFree: %d", free);
            nodo.pointer_indirecto_doble=free;
            int n;
            for(n=0;n<256;n++){buffer1[n]=-1;}
            escribirBloque(free,(char *)buffer1,arch);
        }
        leerBloque(nodo.pointer_indirecto_doble,(char *)buffer1,arch);
        int bloque1=(bloque-268)/256;
        if(buffer1[bloque1]==-1)
        {
              free=pos_libres(arch);
              printf("\n DFree: %d", free);
              buffer1[bloque1]=free;
              int n;
              for(n=0;n<256;n++){buffer2[n]=-1;}
              escribirBloque(free,(char *)buffer2,arch);
        }
        leerBloque(buffer1[bloque1],(char *)buffer2,arch);
         bloque=(bloque-268)%256;
        int blq=buffer2[bloque];
        if(blq==-1)
        {
            blq=pos_libres(arch);
            buffer2[bloque]=blq;
            escribirBloque(buffer2[bloque],(char*)buffer2,arch);
        }


        return blq;

    }

}

void agregar(char *orgen, char * destino,char * nombre, char *tipo)
{
    FILE *arch=0;
    FILE *origen=0;
    header tmp;
    inodo info;
    int n;
    for(n=0;n<12;n++){info.pointer_directo[n]=-1;}
    info.pointer_indirecto=-1;
    info.pointer_indirecto_doble=-1;
    FE fe;
    TE te;
    int pinodo, pfe;
    if((origen=fopen(orgen,"rb+"))==NULL)
    {
        printf("error");
        exit(2);
    }

    if((arch=fopen(destino,"rb+"))==NULL)
    {
        printf("error");
        exit(2);
    }
    tmp=getHeader(arch);
    int bitLibre=pos_libres(arch);
    pinodo=bitLibre*1024;
    setTagEntries(tipo,bitLibre,tmp,arch);
    setFileEntries(nombre,bitLibre,tmp,arch);
    fseek(origen,0,SEEK_END);
    info.size_byte=ftell(origen);
    int inf;
    inf=info.size_byte/1024;
    if(info.size_byte%1024!=0){
    inf++;
    }
    info.num_bloques=inf;
    int cont=0;
     int leido;
     char buffer[1024];
     printf("tiene %d bytes",info.num_bloques);
     fseek(origen,0,SEEK_SET);
     int bloque;
     while (cont<(int(info.num_bloques-1))){
        fread(buffer,1,1024,origen);
        bloque=getBloque(info,cont,arch);
        cont++;
        fseek(arch,bloque*1024,0);
            fwrite(buffer,1024,1,arch);
            printf("cntador %d ",bloque);
    }
     leido=cont*1024;
     int falta;
     falta=info.size_byte-leido;
      char buffer2[falta];
      bloque=getBloque(info,cont,arch);

      fseek(arch,bloque*1024,0);
     fwrite(buffer2,falta,1,arch);
     fseek(arch,pinodo,0);
     fwrite(&info,1,sizeof(inodo),arch);
     fclose(origen);


}


void extraer(char *nombre,char *origen, char *destino)
{
FILE *arch,*dest;
arch=fopen(origen,"rb+");
dest=fopen(destino,"wb+");
if(arch==NULL)
{
    printf("error");
    exit(3);
}
FE name;
inodo info;
header tmp=getHeader(arch);
fseek(arch,tmp.FE_POS,0);
int x;
x=0;
while(strcmp(nombre,name.filename)!=0 || x<tmp.FE_used)
{
    name=getFileEntrie(arch);
    x++;
}
fseek(arch,name.pointer_INodo*1024,0);
fread(&info,1,sizeof(inodo),arch);
char buffer[1024];
 int pos;
for(x=0;x<(info.num_bloques-1);x++){
    pos=getBloque(info,x,arch);
    fseek(arch,pos*1024,0);
    fread(&buffer,1024,1,arch);
    fwrite(&buffer,1024,1,dest);
}
int falta;
falta=(int)info.size_byte-(x*1024);
char buffer2[falta];
pos=getBloque(info,x,arch);
fseek(arch,pos*1024,0);
fread(&buffer2,falta,1,arch);
fwrite(&buffer2,falta,1,dest);
fclose(arch);
fclose(dest);}
int retornarINodo(char *cancion,char * archivo)
{
    FILE *arch;
    if((arch=fopen(archivo,"rb+"))==NULL)
    {
        printf("error");
        exit(4);
    }
    FE name;
    header tmp=getHeader(arch);
    fseek(arch,tmp.FE_POS,0);
    int x;
    x=0;
    while(strcmp(cancion,name.filename)!=0 || x<tmp.FE_used)
    {
        name=getFileEntrie(arch);

        x++;
    }
    if((strcmp(cancion,name.filename)==0))
    {
        return name.pointer_INodo;
    }else{
        return -1;
    }

}

void eliminar(char* nombre,char* disco){
    FILE *arch;
    arch=fopen(disco,"rb+");
        if(arch==NULL)
    {
        printf("error");
        exit(3);
    }
    FE name;
    inodo info;
    header tmp=getHeader(arch);
    fseek(arch,tmp.FE_POS,0);
    int x;
    x=0;
    while(strcmp(nombre,name.filename)!=0 || x<tmp.cant_b_fileEntries)
    {
        name=getFileEntrie(arch);
        x++;
    }
    fseek(arch,name.pointer_INodo*1024,0);
    fread(&info,1,sizeof(inodo),arch);
    int pos;
    for(x=0;x<(info.num_bloques);x++){

        pos=getBloque(info,x,arch);
        setbit(pos,arch,0);
    }
    for(x=0;x<12;x++){
        setbit(info.pointer_directo[x],arch,0);
    }
    setbit(info.pointer_indirecto,arch,0);
    setbit(info.pointer_indirecto_doble,arch,0);
    imprimirMapa(arch);
    fclose(arch);

}

void listarTagEntries(char *origen)
{
    FILE *arch;
    arch=fopen(origen,"rb+");
    TE te;
    if(arch==NULL)
    {
        printf("error");
        exit(5);
    }
    int x=0;
    int enc=0;
    header tmp=getHeader(arch);
    fseek(arch,tmp.TE_POS,0);
    do{
        te=getTagEntry(arch,tmp);
        if(strcmp(te.tag,"~")!=0)
        {
            printf(te.tag,",");
            enc++;
        }
        x++;
    }while(x=tmp.cant_b_tagsEntries*1024 || enc==tmp.TE_used);


}


#endif // ARCHIVOS_H
