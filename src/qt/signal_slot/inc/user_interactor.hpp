#ifndef __USER_INTERACTOR_HPP__
#define __USER_INTERACTOR_HPP__

#include <QObject>

class UserInteractor: public QObject
{
    Q_OBJECT
public:
    explicit UserInteractor(QObject *parent = nullptr);
    ~UserInteractor() = default;

    void getInput();

signals:
    void phraseTyped(const QString &phrase);
};

#endif
