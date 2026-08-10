#include <QCoreApplication>
#include <QDebug>
#include <QLoggingCategory>

// Bu ifade, "network" adında bir log kategorisi tanımlar.
Q_DECLARE_LOGGING_CATEGORY(network);
//Bu ifade, network adındaki log kategorisini tanımlar ve etkinleştirir.
//İlk parametre, kategorinin adını belirtir.
Q_LOGGING_CATEGORY(network,"network");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInfo() << "test";

    qInfo(network) << "test";
    qWarning(network) << "test";

    //turn it off
    QLoggingCategory::setFilterRules("network.debug=false");

    //Will not log
    qDebug(network) << "kitty";

    if(!network().isDebugEnabled())
    {
        QLoggingCategory::setFilterRules("network.debug=true");
        qDebug(network) << "We turned it back on!";
    }

    qDebug(network) << "yes";
    return a.exec();
}
