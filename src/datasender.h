#ifndef DATASENDER_H
#define DATASENDER_H

#include <QtCore>
#include <QByteArray>
#include <QByteArray>
#include <QStringList>
#include <QTcpSocket>
#include <QtSerialPort/QSerialPort>
//#include "qextserialport.h"
//#include "settingsstorage.h"
#include "settingsstorage_sql.h"

class DataSender
{
    static DataSender* _self;
    Q_DECLARE_TR_FUNCTIONS(DataSender)
protected:
    DataSender();
public:
    static DataSender* Instance()
    {
        if(!_self) _self = new DataSender();
        return _self;
    }

    bool open();
    void close();
    bool isOpen() { return port->isOpen(); }
    QByteArray receiveData();
    QByteArray sendCommand(unsigned char opcode, unsigned char address, QByteArray data, int requiredLength);
    void  sendCommand_old(unsigned char address, QByteArray data, int requiredLength);
    unsigned short Crc16(char *pcBlock, unsigned short len);
    char SphericalSenderInVacuum(unsigned char cmd, QByteArray &bytes);

    QByteArray getStatic( int num );
    QByteArray getDynamic( int num );
    QByteArray getDynamic_old( int num );
    QByteArray getTimeDate();
    void reverseArray(QByteArray *data);
    enum Commands {RSRVD0, SWITCH, RSRVD1, DATA, BRIGHT, INFO, NEWADDR};
    enum ComData {RSRVD2, DAY, RSRVD3, RSRVD4, NIGHT, RSRVD5, RSRVD6, RSRVD7, OFF};
    QByteArray sendPacket(QByteArray packet, int requiredLength);

    QByteArray sendCommand8(unsigned char opcode, unsigned char address, QByteArray data, int requiredLength);
private:
    QSerialPort *port;
    QStringList log;
    SettingsStorage_sql *settings;
    QTcpSocket *socket;
};

#endif // DATASENDER_H
