#include <QDir>
#include <QFile>
#include <QDebug>
#include <QBuffer>
#include <QTextStream>
#include <QCoreApplication>

void display(QString title, QByteArray &bytes);
bool makeFile(QString path);
QByteArray getHeader();
bool compressFile(QString originalFile, QString newFile);
bool decompressFile(QString originalFile, QString newFile);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QByteArray data("This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, This is my very long and annoying data, ");

    QByteArray compressed = qCompress(data,9);
    QByteArray decompressed = qUncompress(compressed);

    display("Original", data);
    display("Compressed", compressed);
    display("Decompressed", decompressed);
/////////////////////////////////////////////////////////
    QString originalFile = "orginal.txt";
    QString compressedFile = "compressed.txt";
    QString decompressedFile = "decompressed.txt";

    if(makeFile(originalFile)) {
        qInfo() << "Original created";

        if(compressFile(originalFile, compressedFile)) {
            qInfo() << "File compressed";
            if(decompressFile(compressedFile,decompressedFile)) {
                qInfo() << "File decompressed!";
            } else {
                qInfo() << "Could not decompress file!";
            }
        } else {
            qInfo() << "File not compressed";
        }
    }
    return a.exec();
}

bool makeFile(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::WriteOnly))
    {
        QByteArray data;
        for(int i = 0; i < 1000; i++)
        {
            data.append(QString::number(i).toUtf8() + "\r\n");
        }

        file.write(data);
        file.close();

        return true;
    }

    return false;
}

QByteArray getHeader() {
    QByteArray header;
    header.append("@!~!@");
    return header;
}

bool compressFile(QString originalFile, QString newFile)
{
    QFile ofile(originalFile);
    QFile nfile(newFile);
    QByteArray header = getHeader();

    if(!ofile.open(QIODevice::ReadOnly)) return false;
    if(!nfile.open(QIODevice::WriteOnly)) return  false;
    int size = 1024;

    while (!ofile.atEnd())
    {
        QByteArray buffer = ofile.read(size);
        QByteArray compressed = qCompress(buffer,9);
        nfile.write(header);
        nfile.write(compressed);
    }

    ofile.close();
    nfile.close();

    qInfo() << "Finished compressing";

    return true;
}

bool decompressFile(QString originalFile, QString newFile)
{
    QFile ofile(originalFile);
    QFile nfile(newFile);
    QByteArray header = getHeader();
    int size = 1024;

    if(!ofile.open(QIODevice::ReadOnly)) return false;
    if(!nfile.open(QIODevice::WriteOnly)) return false;


    QByteArray buffer = ofile.peek(size);
    if(!buffer.startsWith(header))
    {
        qCritical() << "We did not create this file!";
        ofile.close();
        nfile.close();
        return false;
    }

    ofile.seek(header.length());

    while (!ofile.atEnd())
    {
        buffer = ofile.peek(size);
        qint64 index = buffer.indexOf(header);
        qDebug() << "Head found at:" << index;

        if(index > -1)
        {
            qint64 maxbytes = index;
            qInfo() << "Reading:" << maxbytes;
            buffer = ofile.read(maxbytes);
            ofile.read(header.length());
        }
        else
        {
            qInfo() << "Read all no header";
            buffer = ofile.readAll();
        }

        QByteArray decompressed = qUncompress(buffer);
        nfile.write(decompressed);
        nfile.flush();
    }

    ofile.close();
    nfile.close();

    return true;

}

void display(QString title, QByteArray &bytes)
{
    qDebug() << "---" << title << "---";
    qDebug() << "Length:" << bytes.size();
    qDebug() << "Plain:" << bytes;
}
