#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QList>
#include <QTableWidgetItem>
#include <QString>
#include "data.h"
#include "btree.h"
#include "hashfun.h"
using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int agregarRegistro();

    QList <registro> listita;
    QTableWidgetItem *item1,*item2,*item3,*item4;
    Data *data;
    Btree *btree;
    hashfun * mihash;
    int cont;


private:
    Ui::MainWindow *ui;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
