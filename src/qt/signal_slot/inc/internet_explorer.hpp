#ifndef __INTERNET_EXPLORER_HPP__
#define __INTERNET_EXPLORER_HPP__

#include <QObject>
#include <QTimer>

class InternetExplorer: public QObject
{
    Q_OBJECT
public:
    explicit InternetExplorer(QObject* parent = nullptr);
    ~InternetExplorer() = default;

public slots:
    void browse();

signals:
    void browseRequested(const QString &phrase);

private:
    QTimer m_timer;
};

#endif
