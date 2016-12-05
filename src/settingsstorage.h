#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H

#include <QByteArray>
#include <QStringList>
#include <QString>
#include <QStandardItemModel>
#include "settings/devicemodel.h"
#include <QDebug>
#include "settings/currencynamesvalues.h"
#include "ftwrapper.h"

class SettingsStorage : public QObject
{
    Q_OBJECT
    static SettingsStorage* _self;
protected:
    SettingsStorage();
public:
    static SettingsStorage* Instance()
    {
        if(!_self) _self = new SettingsStorage();
        return _self;
    }
    void saveSettings();

    //GUI methods

    //Timer methods
    void setDayHour(unsigned char value) { dayHour = value; }
    void setDayMinute(unsigned char value){dayMinute = value;}
    void setNightHour(unsigned char value){nightHour = value;}
    void setNightMinute(unsigned char value){nightMinute = value;}
    void setTimerEnabled(bool value){timerEnabled = value;}

    unsigned char getDayHour() { return dayHour; }
    unsigned char getDayMinute() { return dayMinute; }
    unsigned char getNightHour() { return nightHour; }
    unsigned char getNightMinute() { return nightMinute; }
    bool isTimerEnabled() { return timerEnabled; }

    //Port methods
    void setPortName(QString value) { portName = value; }
    void setPortSpeed(unsigned int value) { portSpeed = value; }
    void setTimeout(unsigned int value) { timeout = value; }
    void setMaxTries(unsigned char value) { maxTries = value; }
    void setTransControlEnabled( bool value ) { transControlEnabled = value; }

    QString getPortName() { return portName; }
    unsigned int getPortSpeed() { return portSpeed; }
    unsigned int getTimeout() { return timeout; }
    unsigned char getMaxTries() { return maxTries; }
    bool isTransControlEnabled() { return transControlEnabled; }

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
    void saveDeviceSettings();
    void addPort(PORT port){ ports->append(port); }
    void addPort(QList<PORT> port){ ports->append(port); }
    void clearPorts(){ ports->clear(); }
    void removePort(int i){ if(i < ports->length()) ports->removeAt(i); }
    QList<PORT> *getPorts() { return ports; }
    bool expertMode;
    void saveDevices();
private:
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
    bool transControlEnabled;

    //Brightness settings
    unsigned char dayBrightness;
    unsigned char nightBrightness;
    //
    QList<PORT> *ports;
};

#endif // SETTINGSSTORAGE_H
