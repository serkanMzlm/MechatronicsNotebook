#include "serial_comm.h"

SerialComm::SerialComm(QObject *parent)
    : QObject{parent}
{
    port_ = new QSerialPort(this);
    checkDeviceConnection();
}

SerialComm::~SerialComm()
{
    port_->close();
}

void SerialComm::checkDeviceConnection()
{
    for(const QSerialPortInfo port_list : QSerialPortInfo::availablePorts()){
        bool is_found = ((port_list.manufacturer() == "FTDI") ||
                         (port_list.description() == "STLINK-V3"));
        if(is_found){
            port_->setPortName(port_list.portName());
            port_->setBaudRate(BAUDRATE);
            port_->setDataBits(QSerialPort::Data8);
            while(port_->open(QIODevice::ReadWrite)){ }
            qDebug() << "Port created successfully";
            QThread::sleep(1);
        }
    }
}

void SerialComm::readBytes()
{
}

void SerialComm::writeBytes(uint8_t new_data)
{
    if ( !(port_->isOpen()))
    {
        qDebug() << "Port Closed";
        return ;
    }

    char dataToSend[1] = { static_cast<char>(new_data) };
    qint64 bytesWritten = port_->write(dataToSend, 1);
    if (bytesWritten == -1) {
        qDebug() << "Could not write data to port: " << port_->errorString();
    } else {
        port_->flush();
    }
}

void SerialComm::debugPort(){
    for(const QSerialPortInfo port_list : QSerialPortInfo::availablePorts()){
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
