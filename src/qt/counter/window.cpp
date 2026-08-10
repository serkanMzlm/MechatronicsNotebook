#include "window.h"
#include "./ui_window.h"

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
{
    _mythread = new myThread(this);
    ui->setupUi(this);
    connect(_mythread, SIGNAL(valueChange(int)), this, SLOT(onNumberChange(int)));
}

window::~window()
{
    delete ui;
}


void window::onNumberChange(int Number)
{
    ui->label->setText(QString::number(Number));
}

void window::on_pushButton_clicked()
{
    _mythread->start();

    if( _mythread->stop)
    {

      _mythread->stop=false;
       ui->pushButton->setText("start");

    }

   //Start
}


void window::on_pushButton_2_clicked()
{
    _mythread->stop = true;
    ui->pushButton->setText("pause");
    //Stop
}

