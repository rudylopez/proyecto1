#include "data.h"
#include "btree.h"

Data::Data(int cuantos)
{
    binary.open("binary.dat", ios::out | ios::binary);

    this->miheader.libre = 0;
    this->miheader.registros = 0;

    binary.write((char *)&miheader,sizeof(header));

    this->reg.id = 0;
    this->reg.time = 0;
    strcpy(reg.source, " ");
    strcpy(reg.destination, " ");
    strcpy(reg.protocol, " ");
    strcpy(reg.info, " ");
    for (int i = 0; i<cuantos; i++)
    {
    this->reg.siguiente  = i +1 ;

    binary.write((char*)&reg,sizeof(registro));

    //cout << "siguiente: " << reg.siguiente << endl;
    }

    binary.flush();

    binary.close();
}

int Data::agregar(registro registrito)
{
    int pos = 0;

    binary.open("binary.dat", ios::in | ios::out | ios::binary);

    binary.read((char*)&miheader,sizeof(header));

    binary.seekp(miheader.libre*sizeof(registro) + sizeof(header));

    binary.write((char*)&registrito,sizeof(registro));

    pos = miheader.libre;

    binary.seekp(0);
    miheader.registros++;
    miheader.libre++;

    binary.write((char*)&miheader,sizeof(header));

    binary.seekg(pos*sizeof(registro) + sizeof(header));
    binary.read((char*)&registrito,sizeof(registro));

   cout << "Id del registro " << pos << ": " << registrito.id << endl;

    return pos;
}

QList <registro> Data::agregar()
{
    int x = 0;

   file.open("wireshark.txt");

    binary.open("binary.dat",ios::in | ios::out | ios::binary);

    //Leemos el header del wireshark.txt
    file.getline(temp,256);

    int cont = 0;
    int libre = cont*(sizeof(registro)) + sizeof(header);

    while((!file.getline(this->temp,256).eof()))
    {

        cout << "Obteniendo los datos del wireshark.txt" << endl;

        this->key = strtok(this->temp," ");
        this->reg.id = this->key.toInt();
        cout << "ID: " << this->reg.id << endl;

        this->key = strtok(NULL," ");
        this->reg.time = this->key.toDouble();
        cout << "tiempo: " << this->reg.time << endl;

        this->key = strtok(NULL," ");
        strcpy(this->reg.source,this->key.toStdString().c_str());
        cout << "Source: " << this->reg.source << endl;

        this->key = strtok(NULL," ");
        strcpy(this->reg.destination,this->key.toStdString().c_str());
        cout << "Destination: " <<this->reg.destination << endl;

        this->key = strtok(NULL," ");
        strcpy(this->reg.protocol,this->key.toStdString().c_str());
        cout << "Protocolo: " << this->reg.protocol << endl;

        this->key = strtok(NULL,"_");
        strcpy(this->reg.info,this->key.toStdString().c_str());
        cout << "Info: " << this->reg.info << endl;

        libre = cont*(sizeof(registro)) + sizeof(header);

        this->reg.siguiente = -1;
        cout << "Siguiente: " << this->reg.siguiente << endl;

        cout << "el espacio libre es para escribir este estruct es en: ";
        cout << libre << endl;
        cout << "escribimos" << endl;

        binary.seekp(libre);

        x = this->agregar(reg);

        cout << "x: " << x << endl;

        //QString temp = reg.source;
        //this->hash.adicionar(temp,x);

        cout << "id :" << reg.id << endl;

        //btree->agregar(x,reg.id);

        lista.push_back(reg);

        binary.flush();

        //Vamos a leer el struct adentro del archivo binario D:

        binary.seekg(libre);

         binary.read((char*)&reg,sizeof(registro));
         cout << "El registro leido es: " << reg.id<< endl;
         cout << "escribimos contador: " << cont<< endl;

         cont++;

             }

         cout << "Actualizar Header" << endl;
         libre = cont*(sizeof(registro)) + sizeof(header);

         this->miheader.registros = cont;
         this->miheader.libre = libre;

        //cout << miheader.registros << endl;
        //cout << miheader.libre << endl;

        binary.seekp(0);
        binary.write((char*)&miheader, sizeof(header));

        cout << "leamos el header" << endl;

        binary.seekg(0);
        binary.read((char*)&miheader,sizeof(header));

        cout << miheader.registros << endl;
        cout << miheader.libre << endl;


        cout << " " << endl;

        return lista;
}
