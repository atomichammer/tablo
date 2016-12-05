#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

#include "datasender.h"
#include "logmodel.h"

DataSender *DataSender ::_self=NULL;

DataSender::DataSender()
{
    settings = SettingsStorage_sql::Instance();
    port = new QextSerialPort();
    //port->setBaudRate(BaudRateType(speed));
    port->setBaudRate(BaudRateType(BAUD9600));
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(10);
    //port->setRts(true); // this to set power for optocouplers (obsolete)
}

void DataSender::reverseArray(QByteArray *data)
{
    //sometimes a reversed array is required =/

    int n, m;
    char p;
    n = data->size() - 1;
    m = n / 2 + 1;
    p = 0;

    for(int i=0; i<m; i++)
    {
        p = data->at(i);
        (*data)[i] = data->at(n-i);
        (*data)[n-i] = p;
    }
}

char DataSender::SphericalSenderInVacuum(unsigned char cmd, QByteArray &bytes)
{
    QModelIndex index;
    LogModel *logModel = LogModel::Instance();
    if(settings->getDevices()->rowCount(QModelIndex()) == 0)
    {
        logModel->addEvent(1, tr( "List of devices is empty." ));
        return 0;
    }

    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
//        return 0;
    }

    char result = 1;

    //for each device in the list
    for(int i = 0; i < settings->getDevices()->rowCount(QModelIndex()); i++)
    {
        //if device is disabled
        index = settings->getDevices()->index(i, 0, QModelIndex());
        if(!settings->getDevices()->data(index, Qt::CheckStateRole).toBool())
        {
            continue;
        }

        //getting type address and name of the device
        index = settings->getDevices()->index(i, 2, QModelIndex());
        int type = settings->getDevices()->data(index, Qt::EditRole).toInt();

        index = settings->getDevices()->index(i, 1, QModelIndex());
        int addr = settings->getDevices()->data(index, Qt::DisplayRole).toInt();
        index = settings->getDevices()->index(i, 3, QModelIndex());
        QString name = settings->getDevices()->data(index, Qt::DisplayRole).toString();

        switch(type)
        {
            case(0x00): //old dynamic (legacy)
            {
                if (cmd == Commands(BRIGHT))
                {
                    continue;
                }
                if (cmd == Commands(DATA))
                {
                    bytes = getDynamic_old(i);
                }
                for(int j=0;j<settings->getMaxTries();j++)
                {
                    sendCommand_old( addr, bytes, 1);
                    logModel->addEvent(0, name + tr( " %1 Success." ).arg( addr ));
                }
            } break;
            case(0x04):
            case(0x01): //static display
            {
                if (cmd == Commands(DATA))
                {
                    bytes = getStatic(i);
                    if(bytes.length() == 0)
                    {
                        logModel->addEvent(1, name + tr( " %1 Failed. No currencies added for this display." ).arg( addr ));
                        result = 0;
                        break;
                    }
                }
                for(int j=0;j<settings->getMaxTries();j++)
                {
                    if(sendCommand(cmd, addr, bytes, 1).contains(0xE0))
                    {
                        logModel->addEvent(0, name + tr( " %1 Success." ).arg( addr ));
                        break;
                    }
                    else
                    {
                        logModel->addEvent(1, name + tr(" %1 Failed. Try %2.").arg(addr).arg(j));
                        result = 0;
                    }
                    //Sleep(100);
                }
            }break;

            case(0x02): //dynamic display
            {
            if (cmd == Commands(BRIGHT))
                {
                    //skip brightness settings for dynamic display
                    continue;
                }
                if (cmd == Commands(DATA))
                {
                    bytes = getDynamic(i);
                }
                for(int j=0;j<settings->getMaxTries();j++)
                {
                    if(sendCommand(cmd, addr, bytes, 1).contains(0xE0))
                    {
                        logModel->addEvent(0, name + tr( " %1 Success." ).arg( addr ));
                        break;
                    }
                    else
                    {
                        logModel->addEvent(1, name + tr(" %1 Failed. Try %2.").arg(addr).arg(j));
                        result = 0;
                    }
                    //Sleep(100);
                }
            }break;
            case(0x03): //clock
            {
                if (cmd == Commands(BRIGHT))
                {
                    continue;
                }
                if (cmd == Commands(DATA))
                {
                    bytes = getTimeDate();
                }
                for(int j=0;j<settings->getMaxTries();j++)
                {
                    if(sendCommand(cmd, addr, bytes, 1).contains(0xE0))
                    {
                        logModel->addEvent(0, name + tr( " %1 Success." ).arg( addr ));
                        break;
                    }
                    else
                    {
                        logModel->addEvent(1, name + tr(" %1 Failed. Try %2.").arg(addr).arg(j));
                        result = 0;
                    }
                    //Sleep(100);
                }
            } break;
        }
        //Sleep(100);
    }
    sender->close();
    return result;
}


QByteArray DataSender::sendCommand(unsigned char opcode, unsigned char address, QByteArray data, int requiredLength)
{
    data.prepend(opcode);
    data.prepend(data.size() + 4); //size of a packet

    unsigned short crc = Crc16(data.data(), data.size());
    data.append(crc & 0x00FF);
    data.append(crc >> 8);
    data.prepend(address);  //address
    data.prepend(char(0xAA));  //start byte
//    data.chop(1);

    QByteArray b;
    b = sendPacket(data, requiredLength);
//    unsigned short CRC2;
//    CRC2 = b.at(7);
//    CRC2 <<= 8;
//    CRC2 |= (unsigned char)b.at(6);
//    crc = Crc16(b.data(), b.size() - 2);
//    qDebug() << QString::number(CRC2, 16) << " : " << QString::number(crc, 16);
    return b;
}

QByteArray DataSender::sendCommand8(unsigned char opcode, unsigned char address, QByteArray data, int requiredLength)
{
    data.prepend(opcode);
    data.prepend(data.size() + 4); //size of a packet
    data.prepend(address);  //address
    unsigned short crc = Crc16(data.data(), data.size());
    data.append(crc & 0x00FF);
    data.append(crc >> 8);
    data.prepend(char(0xAA));  //start byte
    //qDebug() << data.toHex();
    return sendPacket(data, requiredLength);
}

void DataSender::sendCommand_old(unsigned char address, QByteArray data, int requiredLength)
{
    data.prepend(data.size() + 1); //size of a packet

    data.prepend(address);  //address
    data.prepend(char(0xAA));  //start byte
    /*
    for (int i = 0; i < data.count(); ++i)
    {
        qDebug() << data.at(i) + " ";
    }
    */
    sendPacket(data, requiredLength);
}

QByteArray DataSender::getTimeDate()
{
    QByteArray data;
    QStringList list;
    QDateTime *dateTime = new QDateTime();

    QString date = dateTime->currentDateTime().toString("yy.MM.dd.hh.mm.ss");
    list = date.split(".");

    for (int i = 0; i<list.size(); i++)
    {
        data.append(list[i].toInt());
    }

    return data;
}

QByteArray DataSender::getStatic( int num )
{
    SettingsStorage_sql *settings = SettingsStorage_sql::Instance();
    QStringList data, result;
    QModelIndex index;
    int dataIndex = 0;

    QStandardItemModel *deviceConfig = settings->getDevices()->getList().at(num).currConfig;

    data = settings->getCurrencyNameValuesModel()->valuesList();
    int total = deviceConfig->rowCount(QModelIndex());
    //qDebug() << "Total: " << total;
    int curID;
    int length;
    int i = 0;
    while (i < total)
    {
        //get currency id
        index = deviceConfig->index(i, 0, QModelIndex());
        curID = deviceConfig->data(index, Qt::DisplayRole).toInt();

        index = deviceConfig->index(i, 1, QModelIndex());
        length = deviceConfig->data(index, Qt::DisplayRole).toInt();

        index = settings->getDevices()->index(num, 7, QModelIndex());
        if(settings->getDevices()->data(index, Qt::DisplayRole).toBool() == true)
        {
            dataIndex = curID*2 + 1;
        }else
        {
            dataIndex = curID*2;
        }

        QString value = data.at(dataIndex);

        if(value.contains(",")) length = length+1;

        QString str = value;

        if(value.length() < length)
        {
            str = value.rightJustified(length, QChar(0x0A), true);

        } else if (value.length() > length)
        {
            int len = value.length() - length;
            str = value.remove(value.length()-len, len);
        }

        result.append(str);

        index = deviceConfig->index(i, 1, QModelIndex());
        length = deviceConfig->data(index, Qt::DisplayRole).toInt();

        index = settings->getDevices()->index(num, 7, QModelIndex());
        settings->getDevices()->data(index, Qt::DisplayRole).toBool();
        if(settings->getDevices()->data(index, Qt::DisplayRole).toBool() == true)
        {
            dataIndex = curID*2;
        }else
        {
            dataIndex = curID*2 + 1;
        }

        value = data.at(dataIndex);
        str = value;
        //qDebug() << curID << " = " << value;

        if(value.contains(",")) length = length+1;

        if(value.length() < length)
        {
            str = value.rightJustified(length, QChar(0x0A), true);

        } else if (value.length() > length)
        {
            int len = value.length() - length;
            str = value.remove(value.length()-len, len);
        }

        result.append(str);

        i++;
    }

//    for (i = 0; i < result.count(); ++i)
//    {
//        qDebug() << result.at(i).toLatin1().toHex();
//    }

    QByteArray bytes;

    bytes = result.join('0').toLatin1();

    for( int i=0; i<bytes.size(); )
    {
        bytes[i] = bytes.at(i) & 0x0F;

        if ( bytes.at(i) == 0xC || bytes.at(i) == 0xE )
        {
            bytes[i-1] = bytes.at(i-1) | 0x10;
            bytes.remove(i, 1);
            continue;
        }
        i++;
    }
    return bytes;
}

QByteArray DataSender::getDynamic( int num )
{
    SettingsStorage_sql *settings = SettingsStorage_sql::Instance();
    QStringList data, result;
    QModelIndex index;

    QStandardItemModel *deviceConfig = settings->getDevices()->getList().at(num).currConfig;

    data = settings->getCurrencyNameValuesModel()->valuesList();
    int total = deviceConfig->rowCount(QModelIndex());
    int curID;
    int length;
    int i = 0;
    while (i < total)
    {
        //get currency id
        index = deviceConfig->index(i, 0, QModelIndex());
        curID = deviceConfig->data(index, Qt::DisplayRole).toInt();

        index = deviceConfig->index(i, 1, QModelIndex());
        length = deviceConfig->data(index, Qt::DisplayRole).toInt();

        QString value = data.at(curID*2);
        QString str = value;

        //incrementing maxLength for dot
        if(value.contains(",")) length = length+1;

        //check limits
        if (value.length() > length)
        {
            int len = value.length() - length;
            str = value.remove(str.length()-len, len);
        }

        length = 5;
        if(value.contains(",")) length = length+1;

        if(value.length() < length)
        {
            str = value.rightJustified(length, QChar(0x0A), true);

        }

        result.append(str);

        value = data.at(curID*2 + 1);
        str = value;

        index = deviceConfig->index(i, 1, QModelIndex());
        length = deviceConfig->data(index, Qt::DisplayRole).toInt();

        if(value.contains(",")) length = length+1;

        if (value.length() > length)
        {
            int len = value.length() - length;
            str = value.remove(value.length()-len, len);
        }

        length = 5;
        if(value.contains(",")) length = length+1;

        if(value.length() < length)
        {
            str = value.rightJustified(length, QChar(0x0A), true);
        }
        i++;
        result.append(str);
    }

//    qDebug() << result;
//    for (i = 0; i < result.count(); ++i)
//    {
//        qDebug() << result.at(i).toLatin1().toHex();
//    }


    // modern variant
    /*
    QDateTime *dateTime = new QDateTime();
    data.prepend(dateTime->currentDateTime().toString("0dd,MM0HH,mm"));
    delete dateTime;
    */
    QByteArray bytes;

    bytes = result.join('0').toLatin1();

    for( int i=0; i<bytes.size(); )
    {
        bytes[i] = bytes.at(i) & 0x0F;

        if ( bytes.at(i) == 0xC || bytes.at(i) == 0xE )
        {
            bytes[i-1] = bytes.at(i-1) | 0x10;
            bytes.remove(i, 1);
            continue;
        }
        i++;
    }

    //int MinLowPos = bytes.count();

    // align to x10
    const int inLineCount = 10;
    int count = ((bytes.size() / inLineCount) + 1) * inLineCount - bytes.size();
    if(count < inLineCount)
        count = ((bytes.size() / inLineCount) + 1) * inLineCount - bytes.size();
    else
        count = (bytes.size() / inLineCount) * inLineCount - bytes.size();

    for(int i = 0; i < count; i++)
    {
        bytes.append(0x0A);
    }


//    bytes.prepend(MinLowPos);

    return bytes;

}

QByteArray DataSender::getDynamic_old( int num )
{
    SettingsStorage_sql *settings = SettingsStorage_sql::Instance();
    QStringList data, result;
    QModelIndex index;

    QStandardItemModel *deviceConfig = settings->getDevices()->getList().at(num).currConfig;

    data = settings->getCurrencyNameValuesModel()->valuesList();
    int total = deviceConfig->rowCount(QModelIndex());
    int curID;
    int length;
    int i = 0;
    while (i < total)
    {
        //get currency id
        index = deviceConfig->index(i, 0, QModelIndex());
        curID = deviceConfig->data(index, Qt::DisplayRole).toInt();

        length = 5;

        QString value = data.at(curID*2);
//        qDebug() << curID << " = " << value;

        if(value.contains(",")) length = length+1;

        if(value.length() < length)
        {
            result.append(value.rightJustified(length, QChar(0x0A), true));
        } else if (value.length() > length)
        {
            int len = value.length() - length;
            QString str = value;
            result.append(str.remove(str.length()-len, len));
        }

        length = 5;
        value = data.at(curID*2 + 1);
        //qDebug() << curID << " = " << value;

        if(value.contains(",")) length = length+1;

        if(value.length() < length)
        {
            result.append(value.rightJustified(length, QChar(0x0A), true));
        } else if (value.length() > length)
        {
            int len = value.length() - length;
            result.append(value.remove(value.length()-len, len));
        }
        i++;
    }

//    for (i = 0; i < result.count(); ++i)
//    {
//        qDebug() << result.at(i).toLatin1().toHex();
//    }

    //old variant (irken)
    QDateTime *dateTime = new QDateTime();
    data.append(dateTime->currentDateTime().toString("dd,MMHH,mm"));
    delete dateTime;

    QByteArray bytes;

    bytes = result.join('0').toLatin1();

    //convert to bcd with dot in higher nibble
    //one digit per byte (in lower nibble)
    for( int i=0; i<bytes.size(); )
    {
        bytes[i] = bytes.at(i) & 0x0F;

        if ( bytes.at(i) == 0xC || bytes.at(i) == 0xE )
        {
            bytes[i-1] = bytes.at(i-1) | 0x10;
            bytes.remove(i, 1);
            continue;
        }
        i++;
    }

    int MinLowPos = bytes.count();

    // align to x8
    const int inLineCount = 8;
    int count = ((bytes.size() / inLineCount) + 1) * inLineCount - bytes.size();
    if(count < inLineCount)
    {
        count = ((bytes.size() / inLineCount) + 1) * inLineCount - bytes.size();
    }
    else
    {
        count = (bytes.size() / inLineCount) * inLineCount - bytes.size();
    }

    for(int i=0; i<count; i++)
    {
        bytes.append(0x0A);
    }

    bytes.prepend(MinLowPos);

    return bytes;

}


/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021   x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
unsigned short DataSender::Crc16(char *pcBlock, unsigned short len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }

    return crc;
}

bool DataSender::open()
{
    LogModel *logModel = LogModel::Instance();

    this->close();
    SettingsStorage_sql *settings = SettingsStorage_sql::Instance();
    port->setPortName("\\\\.\\" + settings->getPortName());
    if(port->open(QIODevice::ReadWrite/* | QIODevice::Unbuffered*/))
    {
        logModel->addEvent(2, tr("Port %1 opened.").arg(settings->getPortName()));
        //qDebug() << "Port opened " << settings->getPortName();
        return true;
    }
    QMessageBox::critical(0, tr("Error"), tr("Port open error: %1").arg(settings->getPortName()));
    logModel->addEvent(1, tr("Port open error: %1").arg(settings->getPortName()));
    return false;
}

void DataSender::close()
{
    LogModel *logModel = LogModel::Instance();
    if(port->isOpen())
    {
        port->close();
        logModel->addEvent(2, tr("Port %1 closed.").arg(settings->getPortName()));
    }
}

QByteArray DataSender::sendPacket(QByteArray packet, int requiredLength)
{
    Q_UNUSED(requiredLength)
    port->flush();
    port->write(packet);
    while(port->bytesToWrite());
//
    unsigned int numBytes = 0;
    QByteArray dataReceived;
    QTime timer(0,0,0,0);
    timer.restart();
    while(dataReceived.length() < 8)
    {
        numBytes = port->bytesAvailable();
        if (numBytes > 0)
        {
            if (numBytes > 255) numBytes = 255;
            dataReceived.append(port->read(numBytes));
//            qDebug() << "Time: " << timer.elapsed();
        }
        if(timer.elapsed() > 800)
        {
            break;
        }
    }
/*
    qDebug() << "R: " << dataReceived.toHex();

//    if(packet.at(1) == 0x00)
    {
        socket = new QTcpSocket();
        socket->connectToHost(QHostAddress("127.0.0.1"), 9876);
//        while(socket->state() != QTcpSocket::ConnectedState);
//        if(socket->state() == QTcpSocket::ConnectedState)
//        {
            socket->write(packet, packet.length());
            qDebug() << "socket";
//        }
//        else
//        {
//            QMessageBox::critical(0, tr("Error"), tr("Socket open error"));
//        }
        socket->disconnectFromHost();
        socket->deleteLater();
    }
*/
    qDebug() << dataReceived.toHex();
    return dataReceived;
}
