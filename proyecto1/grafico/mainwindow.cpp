#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox msg;

    argumentos.clear();
    this->argumentos << "-c" << this->ui->Crear_Tam->text() << this->ui->Crear_FE->text() << this->ui->Crear_TE->text() << this->ui->crear_nombre->text();
    this->Proceso.start("./mfs",argumentos);

    msg.setText("Hecho");
    msg.exec();

}




void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox msg;

    argumentos.clear();
    this->argumentos << "-a" << this->ui->Agregar_nombre->text()<<this->ui->Agregar_disco->text();
    this->Proceso.start("./mfs",argumentos);

    msg.setText("Hecho");
    msg.exec();
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox msg;

    argumentos.clear();
    this->argumentos << "-e" << this->ui->Ruta->text() << this->ui->ex_nombre->text() << this->ui->ex_disco->text();
    this->Proceso.start("./mfs",argumentos);

    msg.setText("Hecho");
    msg.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox msg;

    argumentos.clear();
    this->argumentos << "-d" << this->ui->el_nombre->text() << this->ui->el_disco->text();
    this->Proceso.start("./mfs",argumentos);

    msg.setText("Hecho");
    msg.exec();
}
