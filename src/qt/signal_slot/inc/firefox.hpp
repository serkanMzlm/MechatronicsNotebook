#ifndef __FIREFOX_HPP__
#define __FIREFOX_HPP__

#include <QObject>

class Firefox : public QObject
{
    Q_OBJECT
public:
    explicit Firefox(QObject *parent = nullptr);
    ~Firefox() = default;
public slots:
    void browse(const QString &phrase);
};

#endif
