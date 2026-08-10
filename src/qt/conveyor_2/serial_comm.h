#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>

#define BAUDRATE 115200

class SerialComm : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged FINAL)
    Q_PROPERTY(QString info READ info WRITE setInfo NOTIFY infoChanged FINAL)
private:
    QSerialPort *port_;
    QString m_error = "";
    QString m_info = "";
    bool start_test = false;

public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();
    void checkDeviceConnection();
    void readBytes();
    Q_INVOKABLE void writeBytes(uint8_t new_data = 0x11);
    Q_INVOKABLE void debugPort();
    QString error() const;
    void setError(const QString &newError);

    QString info() const;
    void setInfo(const QString &newInfo);

signals:
    void errorChanged();
    void infoChanged();
};

#endif // SERIAL_COMM_H
