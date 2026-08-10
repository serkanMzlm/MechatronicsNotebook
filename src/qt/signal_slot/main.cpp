#include <QCoreApplication>

#include "firefox.hpp"
#include "internet_explorer.hpp"
#include "user_interactor.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UserInteractor interactor;
    Firefox firefox;
    InternetExplorer explorer;

    QObject::connect(&interactor, &UserInteractor::phraseTyped, &firefox, &Firefox::browse);
    QObject::connect(&interactor, &UserInteractor::phraseTyped, &explorer, &InternetExplorer::browseRequested);

    interactor.getInput();
    return a.exec();
}
