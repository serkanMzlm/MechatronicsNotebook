#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "new_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->plainTextEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionnew_window_triggered()
{
    newWindow = new new_window(this);
    newWindow->show();


}



