#include "serial_comm.h"

SerialComm::SerialComm(QObject *parent)
    : QObject{parent}
{
    port_ = new QSerialPort(this);
    checkDeviceConnection();
    debugPort();
    connect(port_, &QSerialPort::readyRead, this, &SerialComm::readBytes);
}

SerialComm::~SerialComm()
{
    port_->close();
}

void SerialComm::checkDeviceConnection()
{
    for(const QSerialPortInfo& port_list : QSerialPortInfo::availablePorts()){
        bool is_found = ((port_list.manufacturer() == "FTDI") ||
                         (port_list.description() == "STLINK-V3") ||
                         (port_list.description() == "USB-SERIAL CH340") ||
                         (port_list.manufacturer() == "STMicroelectronics"));
        if(is_found){
            port_->setPortName(port_list.portName());
            port_->setBaudRate(BAUDRATE);
            port_->setDataBits(QSerialPort::Data8);
            while(port_->open(QIODevice::ReadWrite)){ }
            qDebug() << "Port created successfully";
            setInfo("Port created successfully");
            QThread::sleep(1);
        }
    }
}

void SerialComm::readBytes()
{
    while(port_->bytesAvailable()){
        uint8_t cur_byte;
        port_->read((char*)&cur_byte, 1);
        qDebug() << cur_byte;
        if(cur_byte == 70)
        {
            setInfo("Enter a Number");
            start_test = true;
        }
    }
}

void SerialComm::writeBytes(uint8_t new_data)
{
    if ( !(port_->isOpen()))
    {
        qDebug() << "Port Closed";
        setError("Port Closed");
        return ;
    }

    if(!start_test && new_data !=  0x11)
    {
        setError("Device Not Ready");
        return;
    }
    setError("");
    QString new_data_str = QString::fromStdString(std::to_string(static_cast<int>(new_data)));
    char dataToSend[1] = { static_cast<char>(new_data) };
    qint64 bytesWritten = port_->write(dataToSend, 1);
    if (bytesWritten == -1) {
        qDebug() << "Could not write data to port: " << port_->errorString();
    } else {
        port_->flush();
    }
}

void SerialComm::debugPort(){
    for(const auto &port_list : QSerialPortInfo::availablePorts()){
        if(port_list.description() == "") {
            continue;
        }
        qDebug() << "-----------------------------------------------------";
        qDebug() << "portName:          " << port_list.portName();
        qDebug() << "systemLocation:    " << port_list.systemLocation();
        qDebug() << "description:       " << port_list.description();
        qDebug() << "manufacturer:      " << port_list.manufacturer();
        qDebug() << "serialNumber:      " << port_list.serialNumber();
        qDebug() << "vendorIdentifier:  " << port_list.vendorIdentifier();
        qDebug() << "productIdentifier: " << port_list.productIdentifier();
    }
}

QString SerialComm::error() const
{
    return m_error;
}

void SerialComm::setError(const QString &newError)
{
    m_error = newError;
    emit errorChanged();
}

QString SerialComm::info() const
{
    return m_info;
}

void SerialComm::setInfo(const QString &newInfo)
{
    m_info = newInfo;
    emit infoChanged();
}
