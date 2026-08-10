#include <QCoreApplication>
#include <QDebug>

#include "agecalc.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AgeCalc tom;
    tom.setAge(29);
    tom.print();
    return a.exec();
}
