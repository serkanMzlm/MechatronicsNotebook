#include <QDebug>
#include <QTextStream>

#include "user_interactor.hpp"

UserInteractor::UserInteractor(QObject *parent) : QObject{parent}
{
}

void UserInteractor::getInput()
{
    qDebug() << "Type in your search phrase: ";
    QTextStream s(stdin);
    const auto &phrase = s.readLine();
    if (!phrase.isEmpty())
    {
        emit phraseTyped(phrase);
    }
}
