#ifndef TYPES_H
#define TYPES_H
typedef unsigned int numero;
typedef struct{
    int cant_bloques;
    int cant_b_fileEntries;
    int cant_b_tagsEntries;
    int FE_used;
    int TE_used;
    int tam_mbits;
    int FE_POS;
    int TE_POS;
    int FE_LIBRE;
    int TE_libre;


}header;

typedef struct
{
    unsigned int size_byte;
    unsigned int pointer_directo[12];
    unsigned int pointer_indirecto;
    unsigned int pointer_indirecto_doble;
    unsigned int pointer_al_file_entry;
    char tags[250];
    char tags_cancion[128];
    unsigned int num_bloques;
}inodo;

typedef struct{
    char filename[60];// tenemos que decir cuantos bloques para FE y TE
    int pointer_INodo;//si esta vacio caracter ~
}FE;
typedef struct{
    char tag [28];//si esta vacio caracter ~
    int pointer;// HACE REFERENCIA A LA LISTA INVERTIDA DE TAGE
}TE;

#endif // TYPES_H
