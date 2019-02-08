#include <QTime>
#include <QMessageBox>
#include "port.h"
#include "settingsstorage.h"

PortManager::PortManager()
{
    port = new QSerialPort();
    //port->setBaudRate(BaudRateType(speed));
//    port->setBaudRate(BaudRateType(BAUD9600));
//    port->setFlowControl(FLOW_OFF);
//    port->setParity(PAR_NONE);
//    port->setDataBits(DATA_8);
//    port->setStopBits(STOP_1);
//    port->setTimeout(10);
    //port->setRts(true); // this to set power for optocouplers (obsolete)
}

bool PortManager::open()
{
    this->close();
    SettingsStorage *settings = SettingsStorage::Instance();
    port->setPortName("\\\\.\\" + settings->getPortName());
    if(port->open(QIODevice::ReadWrite/* | QIODevice::Unbuffered*/))
    {
        qDebug() << "Port opened " << settings->getPortName();
        return true;
    }
    QMessageBox::critical(0, tr("Error"), tr("Port open error: %1").arg(settings->getPortName()));
    return false;
}

void PortManager::close()
{
    if(port->isOpen()) port->close();
    qDebug() << "Port closed.";
}

QByteArray PortManager::sendPacket(QByteArray packet)
{
    this->open();

    port->flush();
    port->write(packet);
    while(port->bytesToWrite());
    qDebug() << "Data written.";
//
    unsigned int numBytes = 0;
    QByteArray dataReceived;
    QTime timer(0,0,0,0);
    timer.restart();
    while(dataReceived.length() < 7)
    {
        numBytes = port->bytesAvailable();
        if (numBytes > 0)
        {
            if (numBytes > 255) numBytes = 255;
            dataReceived.append(port->read(numBytes));
        }
        if(timer.elapsed() > 500)
        {
            break;
        }
    }
    qDebug() << dataReceived.toHex();
    this->close();
    return dataReceived;
}


QByteArray PortManager::receiveAnswer()
{
//    unsigned int numBytes = 0;
//    QByteArray dataReceived;
//    QTime timer(0,0,0,0);
//    timer.restart();
//    while(dataReceived.length() < 7)
//    {
//        numBytes = port->bytesAvailable();
//        if (numBytes > 0)
//        {
//            if (numBytes > 255) numBytes = 255;
//            dataReceived.append(port->read(numBytes));
//        }
//        if(timer.elapsed() > 500)
//        {
//            break;
//        }
//    }
//    qDebug() << dataReceived.toHex();
//    return dataReceived;
}
