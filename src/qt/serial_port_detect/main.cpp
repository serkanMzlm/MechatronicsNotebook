#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    if (ports.isEmpty()) {
        qDebug() << "Hiçbir seri port bulunamadı.";
    } else {
        foreach (const QSerialPortInfo &portInfo, ports) {
            qDebug() << "-----------------------------------------------------";
            qDebug() << "Port Name:          " << portInfo.portName();
            qDebug() << "System Location:    " << portInfo.systemLocation();
            qDebug() << "Description:        " << (portInfo.description().isEmpty() ? "Boş port" : portInfo.description());
            qDebug() << "Manufacturer:       " << (portInfo.manufacturer().isEmpty() ? "Bilinmiyor" : portInfo.manufacturer());
            qDebug() << "Serial Number:      " << (portInfo.serialNumber().isEmpty() ? "N/A" : portInfo.serialNumber());
            qDebug() << "Vendor Identifier:  " << (portInfo.hasVendorIdentifier() ? QString::number(portInfo.vendorIdentifier(), 16) : "N/A");
            qDebug() << "Product Identifier: " << (portInfo.hasProductIdentifier() ? QString::number(portInfo.productIdentifier(), 16) : "N/A");
            qDebug() << "Busy:               " << (portInfo.isBusy() ? "Yes" : "No");
        }
    }
    return a.exec();
}
