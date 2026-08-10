#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "rosnode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class window; }
QT_END_NAMESPACE

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();
    rosNode* rosnode;

public slots:
    void labelChanged(int);

private slots:
    void on_start_button_clicked();

    void on_stop_button_clicked();

    void on_reset_button_clicked();

    void on_up_button_clicked();

    void on_down_button_clicked();

    void on_left_button_clicked();

    void on_right_button_clicked();

    void on_cancel_button_clicked();

    void on_setLine_editingFinished();

private:
    Ui::window *ui;
};
#endif // WINDOW_H
