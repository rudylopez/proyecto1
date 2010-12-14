#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QApplication>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->cont = 0;
    this->data = new Data(5);
    this->btree = new Btree();
    this->mihash = new hashfun();
    mihash->crear(5,"Datos.Inl",11);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->listita = data->agregar();
        QString temp1,temp2;

        for (int i = 0; i<3; i++)
        {
            temp1.setNum(this->listita.at(i).id);
            temp2.setNum(this->listita.at(i).time);

                this->item1 = new QTableWidgetItem();
                this->item2 = new QTableWidgetItem();
                this->item3 = new QTableWidgetItem();
                this->item4 = new QTableWidgetItem();

                this->ui->tableWidget->setItem(i,0,new QTableWidgetItem(temp1));

                this->ui->tableWidget->setItem(i,1,new QTableWidgetItem(temp2));

                this->item1->setText(this->listita.at(i).source);
                this->ui->tableWidget->setItem(i,2,item1);

                this->item2->setText(this->listita.at(i).destination);
                this->ui->tableWidget->setItem(i,3,item2);

                this->item3->setText(this->listita.at(i).protocol);
                this->ui->tableWidget->setItem(i,4,item3);

                this->item4->setText(this->listita.at(i).info);
                this->ui->tableWidget->setItem(i,5,item4);

                this->cont++;
        }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString temp1,temp2;

    registro registritotemp;

    registritotemp.id = this->ui->lineEdit->text().toInt();
    registritotemp.time = this->ui->lineEdit_2->text().toDouble();
    strcpy(registritotemp.source,this->ui->lineEdit_3->text().toStdString().c_str());
    strcpy(registritotemp.destination,this->ui->lineEdit_4->text().toStdString().c_str());
    strcpy(registritotemp.protocol,this->ui->lineEdit_5->text().toStdString().c_str());
    strcpy(registritotemp.info,this->ui->lineEdit_3->text().toStdString().c_str());
    registritotemp.siguiente = -1;

    int x = this->data->agregar(registritotemp);
    //this->btree->agregar(x,registritotemp.id);
        temp1.setNum(registritotemp.id);
        temp2.setNum(registritotemp.time);

            this->item1 = new QTableWidgetItem();
            this->item2 = new QTableWidgetItem();
            this->item3 = new QTableWidgetItem();
            this->item4 = new QTableWidgetItem();

            int fila = cont;

            this->ui->tableWidget->setItem(fila,0,new QTableWidgetItem(temp1));

            this->ui->tableWidget->setItem(fila,1,new QTableWidgetItem(temp2));

            this->item1->setText(registritotemp.source);
            this->ui->tableWidget->setItem(fila,2,item1);

            this->item2->setText(registritotemp.destination);
            this->ui->tableWidget->setItem(fila,3,item2);

            this->item3->setText(registritotemp.protocol);
            this->ui->tableWidget->setItem(fila,4,item3);

            this->item4->setText(registritotemp.info);
            this->ui->tableWidget->setItem(fila,5,item4);

            this->cont++;

}
