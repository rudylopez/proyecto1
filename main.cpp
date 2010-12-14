#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "archivos.h"

int main(int argc, char *argv[])
{


    if(argc>2)
    {
        if(strcmp(argv[1],"-c")==0)
        {
            if(argc==6)
            {
                numero tambloqueAr=atol(argv[2]);
                numero tamFE=atol(argv[3]);
                numero tamTE=atol(argv[4]);
                char *nombre=argv[5];
                crearArchivo(tambloqueAr,tamFE,tamTE,nombre);


            }else{

                printf("ERROR \n");
                exit(1);
            }

        }else if(argc==4 && strcmp(argv[1],"-a")==0)
        {
            if(strcmp(argv[1],"-a")==0)
            {
                agregar(argv[2],argv[3],"dragonball","dibujos animados");
                listarTagEntries("rudy.dat");
            }else{
                printf("error");
                exit(2);
            }
        }else if(argc==5){

            if(strcmp(argv[1],"-e")==0)
            {
                extraer(argv[2],argv[3],argv[4]);

            }else{
                printf("error");
                exit(2);
            }
        }else if (argc==4 && strcmp(argv[1],"-q")==0)
        {
            if(strcmp(argv[1],"-q")==0)
            {
               printf("%d", retornarINodo(argv[2],argv[3]));
            }
        }else if(argc==4 && strcmp(argv[1],"-d")==0)
        {
            eliminar(argv[2],argv[3]);
        }

        else
        {

            printf("error \n");
            exit(1);
        }


    }else{
        printf("error \n");
        exit(1);
    }



    return 0;
}
