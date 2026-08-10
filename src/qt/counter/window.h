#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mythread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class window; }
QT_END_NAMESPACE

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();
    myThread* _mythread;
private:
    Ui::window *ui;
public slots:
    void onNumberChange(int);



private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};
#endif // WINDOW_H
