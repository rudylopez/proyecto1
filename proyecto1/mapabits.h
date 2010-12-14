#ifndef MAPABITS_H
#define MAPABITS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"

void setbit(int cual,FILE *arch,numero valorBit)
{
    int bloqueBit;
        bloqueBit=cual/(8*1024);
        bloqueBit++;
        cual=cual%(8*1024);
        //ubicarnos en el bloque correcto
        fseek(arch,1024*bloqueBit,SEEK_SET);
        // leer el bloque al cual vamos a setear su bit
        unsigned char *mapaBits;
        mapaBits=(unsigned char *)malloc(1024);
        fread(mapaBits,1,1024,arch);
        fseek(arch,-1024,SEEK_CUR);
        unsigned int casilla=cual/8;
        if(valorBit==1)
        {
            mapaBits[casilla]=mapaBits[casilla] | (1 << (7-cual%8));
        }else{
            mapaBits[casilla] = mapaBits[casilla] & ~(1 << (7-cual%8));
        }
        fwrite(mapaBits,1,1024,arch);
        free(mapaBits);

}

int getBit(int cual,FILE *arch)
{
    int bloqueBit=cual/(1024*8);
    bloqueBit++;
    cual=cual%(1024*8);
    //ubicarnos en el bloque correcto
    fseek(arch,1024*bloqueBit,SEEK_SET);
    // leer el bloque al cual vamos a setear su bit
    unsigned char *mapaBits;
    mapaBits=(unsigned char *)malloc(1024);
    fread(mapaBits,1,1024,arch);
    unsigned int valorBit=mapaBits[cual/8] & (1<<(7-cual%8));
    free(mapaBits);
    if(valorBit != 0)
    {
        return 1;
    }else{
        return 0;
    }
}

int getFreeBit(FILE *arch)
{
    header tmp;
    fseek(arch,0,SEEK_SET);
    fread(&tmp,1,sizeof(header),arch);
    int i;
    for(i=0;i<((1024*8)*tmp.tam_mbits);i++)
    {
        if(getBit(i,arch)==0)
            return i;
    }

}
void imprimirMapa(FILE *arch)
{
    header tmp;
    fseek(arch,0,0);
    fread(&tmp,1,sizeof(header),arch);

    int t;

          for(t=0;t<(1024*8)*tmp.tam_mbits;t++)
          {
                int bit=getBit(t,arch);
                printf("%d",bit);
          }

}

#endif // MAPABITS_H
