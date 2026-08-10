#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QTextStream>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);
    static bool logging;
    static QString filename;
    static void attach();
    static void handler(QtMsgType type, const QMessageLogContext &context,
                                                            const QString &msg);
signals:
};

#endif // LOGGER_H
