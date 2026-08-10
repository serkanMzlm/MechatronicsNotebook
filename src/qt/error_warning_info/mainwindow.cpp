#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
     QMessageBox::information(this, "info", "deneem");
}


void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::warning(this, "warning", "deneme");
}



void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox::critical(this,"critical","deneme");
}


void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::question(this, "question", "deneme");
}

