#ifndef SETTINGSSTORAGE_SQL_H
#define SETTINGSSTORAGE_SQL_H

#include <QByteArray>
#include <QStringList>
#include <QString>
#include <QStandardItemModel>
#include "devicemodel.h"
#include <QDebug>
#include <QtSql>
#include "currencynamesvalues.h"
//#include "ftwrapper.h"

typedef struct{
    QString description;
    QString name;
} PORT;

class SettingsStorage_sql : public QObject
{
    Q_OBJECT
    static SettingsStorage_sql* _self;
protected:
    SettingsStorage_sql();
public:
    static SettingsStorage_sql* Instance()
    {
        if(!_self) _self = new SettingsStorage_sql();
        return _self;
    }

    void saveSettings();

    //Port methods
    void setPortName(QString value) { portName = value; }
    void setPortSpeed(unsigned int value) { portSpeed = value; }
    void setTimeout(unsigned int value) { timeout = value; }
    void setMaxTries(unsigned char value) { maxTries = value; }

    QString getPortName() { return portName; }
    unsigned int getPortSpeed() { return portSpeed; }
    unsigned int getTimeout() { return timeout; }
    unsigned char getMaxTries() { return maxTries; }

    //Brightness methods
    void setDayBrightness(unsigned int value) { dayBrightness = value;  }
    void setNightBrightness(unsigned int value) { nightBrightness = value; }

    unsigned char getDayBrightness() { return dayBrightness; }
    unsigned char getNightBrightness() { return nightBrightness; }

    //model
    DeviceModel *getDevices() { return devices; }
    CurrencyNamesValues *getCurrencyNameValuesModel() { return currencyNameValues; }

    void loadGlobalDisplaySettings();
    void loadCommunicationsSettings();
    void loadDeviceSettings(QString deviceName);
//    void saveDeviceSettings();
    void addPort(PORT port){ ports->append(port); }
    void addPort(QList<PORT> port){ ports->append(port); }
    void clearPorts(){ ports->clear(); }
    void removePort(int i){ if(i < ports->length()) ports->removeAt(i); }
    QList<PORT> *getPorts() { return ports; }
    bool expertMode;
    void saveDevices();
private:
    QSqlDatabase sdb;
    //models
    DeviceModel *devices;
    CurrencyNamesValues *currencyNameValues;

    //Timer settings
    unsigned char dayHour;
    unsigned char dayMinute;
    unsigned char nightHour;
    unsigned char nightMinute;
    bool timerEnabled;

    //Port settings
    QString portName;
    unsigned int portSpeed;
    unsigned int timeout;
    unsigned char maxTries;

    //Brightness settings
    unsigned char dayBrightness;
    unsigned char nightBrightness;
    //
    QList<PORT> *ports;
    void checkTables();
    void loadDevices();
};

#endif // DATASTORAGE_SQL_H
