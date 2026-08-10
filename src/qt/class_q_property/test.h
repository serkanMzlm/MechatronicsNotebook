#ifndef TEST_H
#define TEST_H

#include <QObject>

class Test : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged FINAL)
public:
    explicit Test(QObject *parent = nullptr);

    QString message() const;
    void setMessage(const QString &newMessage);

signals:
    void messageChanged(QString message);
private:
    QString m_message;
};

#endif // TEST_H
