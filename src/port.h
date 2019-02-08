#ifndef _PORT_H_
#define _PORT_H_

#include <QString>
#include <QByteArray>
#include <QThread>
#include <QMutex>
#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
//#include "qextserialport.h"

class PortManager : public QObject
{
    Q_OBJECT
protected:

public:
    PortManager();
    //methods
    void setPortName(QString name);
    QByteArray sendPacket(QByteArray packet);
    QByteArray receiveAnswer();
    bool open();
    void close();
    bool isOpen() { return port->isOpen(); }

    //fields
    QSerialPort *port;
signals:
    void timeoutOccured(const QByteArray &);
};

#endif // PORT_H
