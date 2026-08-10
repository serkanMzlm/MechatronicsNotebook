#include "window.h"
#include "./ui_window.h"

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
{
    rosnode = new rosNode(this);
    ui->setupUi(this);
    rosnode->start();

    connect(rosnode, SIGNAL(timerChanged(int)), this, SLOT(labelChanged(int)));
    // connect(ui->reset_button, &QPushButton::clicked, this, [&]() { rosnode->resetService(); });
}

window::~window()
{
    delete ui;
}


void window::labelChanged(int value)
{
    ui->getValue->setText(QString::number(value));
}

void window::on_start_button_clicked()
{
    // rosnode->value = true;

    ui->getLine->setText("<b>Started...\n</b>");
}

void window::on_stop_button_clicked()
{
    if(rosnode->stop==true)
    {
        qDebug("---------------------------------");
        rosnode->stop = false;
        ui->getLine->setText("<b>Stoped...</b>");
    }

}

void window::on_reset_button_clicked()
{

    if(rosnode->value != 0)
    {
        qDebug("reset....");


        // rosnode->_thread.push_back(std::thread(std::bind(&rosNode::resetClientCallback, this)));
        QMessageBox::information(this, "Reset", "values ​​are reset");
        // rosnode->value = 0;
        ui->getLine->setText("<b>Reset...</b>");
        ui->getValue->setText(QString::number(0));
        rosnode->resetService();
        rosnode->stop = false;
    }
}

void window::on_up_button_clicked()
{

    rosnode->statusPublisher(ui->up_button->text().toStdString());
}

void window::on_down_button_clicked()
{
    rosnode->statusPublisher(ui->down_button->text().toStdString());
}

void window::on_left_button_clicked()
{
    rosnode->statusPublisher(ui->left_button->text().toStdString());
}

void window::on_right_button_clicked()
{
    rosnode->statusPublisher(ui->right_button->text().toStdString());
}

void window::on_cancel_button_clicked()
{
    QMessageBox::warning(this, "EXİT", "The data will be deleted and the application will be closed.");
}

void window::on_setLine_editingFinished()
{
    rosnode->textPublisher(ui->setLine->displayText().toStdString());
    ui->setLine->clear();

}
