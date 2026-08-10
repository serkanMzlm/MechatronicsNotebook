#include "mainwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLabel* label = new QLabel("<h1>hello</h1> <font color=blue>world</font>"); //html kodları geçerlidir.
    label->show();
    return a.exec();
}
