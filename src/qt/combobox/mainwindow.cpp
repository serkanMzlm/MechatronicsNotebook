#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=0; i<10; i++)
    {
        ui->comboBox->addItem(QString::number(i)+" item");
    }
//    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(qDebug("------")));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboBox_activated(int index)
{
    qDebug("-----");
//    if(ui->comboBox->)
    msgBox->information(this, "box_X", ui->comboBox->currentText());
}

