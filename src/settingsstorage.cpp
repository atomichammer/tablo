#include "settingsstorage.h"
#include "qextserialport.h"

#include <QSettings>
#include <QModelIndex>
#include <QDir>

#include "settings/devicecontroller.h"

SettingsStorage *SettingsStorage ::_self=NULL;

SettingsStorage::SettingsStorage()
{
    ports = new QList<PORT>;
    loadCommunicationsSettings();
    loadGlobalDisplaySettings();

    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);

    iniSettings->beginGroup("StaticSettings");
        expertMode = iniSettings->value("ExpertMode", false).toBool();
    iniSettings->endGroup();

    //Loading devices list
    unsigned char devNumber;

    devices = new DeviceModel();

    QModelIndex index;
    devNumber = iniSettings->beginReadArray("Devices");
    //devNumber = 2;
    if (devNumber != 0)
    {
        QString name;
        for (unsigned char i = 0; i < devNumber; ++i)
        {
            index = devices->index(i, 3, QModelIndex());
            iniSettings->setArrayIndex(i);
            name = iniSettings->value("name", QString("Device_%1").arg(i)).toString();
            devices->setData(index, name, Qt::DisplayRole);
            loadDeviceSettings(name);
        }
    }
    iniSettings->endArray();
    delete iniSettings;
}

void SettingsStorage::loadDeviceSettings(QString deviceName)
{
    DeviceController *devControl = new DeviceController(devices);

    QString iniName = "devices\\" + deviceName + ".ini";
    QSettings *deviceSettings = new QSettings(iniName, QSettings::IniFormat);

    deviceSettings->beginGroup("Parameters");
        bool enabled = deviceSettings->value("enabled", true).toBool();
        int address = deviceSettings->value("address", 0).toInt();
        int type = deviceSettings->value("type", 0).toInt();
        int status = deviceSettings->value("status", 0).toInt();
        int length = deviceSettings->value("length", 0).toInt();
        bool swap = deviceSettings->value("swap", false).toBool();
        QString port = deviceSettings->value("port", 0).toString();

        QModelIndex index = devControl->addDevice(enabled, address, deviceName, type, 0, port);
        devControl->setStatus(&index, status);
        devControl->setLength(&index, length);
        devControl->setSwap(&index, swap);
    deviceSettings->endGroup();

    QStandardItemModel *currConfig;
    currConfig = devices->getList().at(index.row()).currConfig;

    int total = deviceSettings->beginReadArray("currencies");
        for (int i = 0; i < total; i++)
        {
            deviceSettings->setArrayIndex(i);
            int row = currConfig->rowCount(QModelIndex());
            QStandardItem *item = new QStandardItem();
            item->setData(deviceSettings->value("id", i).toInt(), Qt::DisplayRole);
            item->setFlags(item->flags() & ~(Qt::ItemIsDropEnabled));
            currConfig->setItem(row, 0, item);

            QStandardItem *item0 = new QStandardItem();
            item0->setData(deviceSettings->value("length", 5).toInt(), Qt::DisplayRole);
            item0->setFlags(item0->flags() & ~(Qt::ItemIsDropEnabled));
            currConfig->setItem(row, 1, item0);
        }
    deviceSettings->endArray();
}

void SettingsStorage::saveDeviceSettings()
{

    QDir dir("devices");
    QFile file;
    QStringList filter;
    filter << "*.ini";
    if (!dir.exists())
    {
        dir.mkdir("devices");
    }
    else
    {
        dir.setNameFilters(filter);
        QStringList files = dir.entryList();

        while(files.count())
        {
            file.setFileName(dir.absolutePath() + "/" + files.takeFirst());
            file.remove();
        }
    }

    int count = devices->rowCount(QModelIndex());
    if(count)
    {
        QModelIndex index;
        for (int i = 0; i < count; ++i)
        {
            index = devices->index(i,3,QModelIndex());
            QString deviceName = devices->data(index, Qt::DisplayRole).toString();
            QString iniName = "devices\\" + deviceName + ".ini";
            QSettings *deviceSettings = new QSettings(iniName, QSettings::IniFormat);
            deviceSettings->beginGroup("Parameters");
                index = devices->index(i,0,QModelIndex());
                deviceSettings->setValue("enabled", devices->data(index,Qt::CheckStateRole).toBool());
                index = devices->index(i,3,QModelIndex());
                deviceSettings->setValue("name", devices->data(index, Qt::DisplayRole).toString());
                index = devices->index(i,1,QModelIndex());
                deviceSettings->setValue("address", devices->data(index, Qt::DisplayRole).toInt());
                index = devices->index(i,4,QModelIndex());
                deviceSettings->setValue("status", devices->data(index, Qt::EditRole).toInt());
                index = devices->index(i,5,QModelIndex());
                deviceSettings->setValue("length", devices->data(index, Qt::EditRole).toInt());
                index = devices->index(i,2,QModelIndex());
                deviceSettings->setValue("type", devices->data(index, Qt::EditRole).toInt());
                index = devices->index(i,6,QModelIndex());
                deviceSettings->setValue("port", devices->data(index, Qt::DisplayRole).toString());
                index = devices->index(i,7,QModelIndex());
                deviceSettings->setValue("swap", devices->data(index, Qt::DisplayRole).toBool());
            deviceSettings->endGroup();

            QStandardItemModel *currConfig;
            currConfig = devices->getList().at(i).currConfig;
            int total = currConfig->rowCount(QModelIndex());

            deviceSettings->beginWriteArray("currencies");
            for (int j = 0; j < total; ++j)
            {
                deviceSettings->setArrayIndex(j);
                index = currConfig->index(j, 0, QModelIndex());
                deviceSettings->setValue("id", currConfig->data(index, Qt::DisplayRole).toInt());
                index = currConfig->index(j, 1, QModelIndex());
                deviceSettings->setValue("length", currConfig->data(index, Qt::DisplayRole).toInt());
            }
            deviceSettings->endArray();
            deviceSettings->sync();

            delete deviceSettings;
        }
    }
}

void SettingsStorage::loadCommunicationsSettings()
{
    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);

    iniSettings->beginGroup("StaticSettings");
            portName = iniSettings->value("port", "COM1").toString();
            portSpeed = iniSettings->value("speed", BAUD9600).toInt();
    iniSettings->endGroup();

    iniSettings->beginGroup("Tranmission");
        timeout = iniSettings->value("timeout", 1).toInt();
        maxTries = iniSettings->value("maxTries", 1).toInt();
        transControlEnabled = iniSettings->value("transControl", false).toBool();
    iniSettings->endGroup();

    delete iniSettings;
}

void SettingsStorage::loadGlobalDisplaySettings()
{
    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);

    currencyNameValues = new CurrencyNamesValues();

    //reading currency names
    int rows = iniSettings->beginReadArray("names");
        if (rows == 0) rows = 3;
        for (int i = 0; i < rows; i++)
        {
            iniSettings->setArrayIndex(i);
            currencyNameValues->appendRow(iniSettings->value("name", "USD").toString() ,"","");
        }
    iniSettings->endArray();

    //reading currency values
    unsigned char j = 0;
    QModelIndex index;
    iniSettings->beginReadArray("values");
        for (int i = 0; i < rows*2;)
        {
            iniSettings->setArrayIndex(i);
            index = currencyNameValues->index(j, 2, QModelIndex());
            currencyNameValues->setData(index, iniSettings->value("value", i).toString(), Qt::EditRole);
            i++;
            iniSettings->setArrayIndex(i);
            index = currencyNameValues->index(j, 3, QModelIndex());
            currencyNameValues->setData(index, iniSettings->value("value", i).toString(), Qt::EditRole);
            i++;
            j++;
        }
    iniSettings->endArray();

    iniSettings->beginGroup("Timer");
            timerEnabled = iniSettings->value("enabled", false).toBool();
            iniSettings->beginGroup("Day");
                    dayHour = iniSettings->value("hours", 8).toInt();
                    dayMinute = iniSettings->value("minutes", 0).toInt();
            iniSettings->endGroup();
            iniSettings->beginGroup("Night");
                    nightHour = iniSettings->value("hours", 20).toInt();
                    nightMinute = iniSettings->value("minutes", 0).toInt();
            iniSettings->endGroup();
    iniSettings->endGroup();

    iniSettings->beginGroup("Brightness");
            dayBrightness = iniSettings->value( "day", 100 ).toInt();
            nightBrightness = iniSettings->value( "night", 15 ).toInt();
    iniSettings->endGroup();

    delete iniSettings;
}

void SettingsStorage::saveDevices()
{
    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);


    unsigned char rows = currencyNameValues->rowCount(QModelIndex());
    QModelIndex index;

    iniSettings->beginWriteArray("Devices");
    int total = devices->rowCount(QModelIndex());
    for (int i = 0; i < total; ++i)
    {
        iniSettings->setArrayIndex(i);
        index = devices->index(i, 3, QModelIndex());
        iniSettings->setValue("name", devices->data(index, Qt::DisplayRole).toString());
    }
    iniSettings->endArray();

    iniSettings->beginWriteArray("names");
    for (unsigned int i = 0; i < rows; ++i)
    {
        index = currencyNameValues->index(i, 1, QModelIndex());
        iniSettings->setArrayIndex(i);
        iniSettings->setValue("name", currencyNameValues->data(index, Qt::DisplayRole).toString());
    }
    iniSettings->endArray();

    iniSettings->beginGroup("Timer");
            iniSettings->setValue("enabled", isTimerEnabled());
            iniSettings->beginGroup("Day");
                    iniSettings->setValue("hours", getDayHour());
                    iniSettings->setValue("minutes", getDayMinute());
            iniSettings->endGroup();
            iniSettings->beginGroup("Night");
                    iniSettings->setValue("hours", getNightHour());
                    iniSettings->setValue("minutes", getNightMinute());
            iniSettings->endGroup();
    iniSettings->endGroup();

    iniSettings->beginGroup("Brightness");
            iniSettings->setValue( "day", getDayBrightness() );
            iniSettings->setValue( "night", getNightBrightness() );
    iniSettings->endGroup();
    iniSettings->sync();

    delete iniSettings;
    saveDeviceSettings();
}

void SettingsStorage::saveSettings()
{

    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);

    unsigned char rows = currencyNameValues->rowCount(QModelIndex());
    QModelIndex index;


    iniSettings->beginGroup("StaticSettings");
        iniSettings->setValue("port", getPortName());
        iniSettings->setValue("speed", getPortSpeed());
        iniSettings->setValue("ExpertMode", QVariant(0));
    iniSettings->endGroup();

    iniSettings->beginGroup("Tranmission");
        iniSettings->setValue("timeout", getTimeout());
        iniSettings->setValue("maxTries", getMaxTries());
        iniSettings->setValue("transControl", isTransControlEnabled());
    iniSettings->endGroup();

    unsigned char j = 0;
    iniSettings->beginWriteArray("values");
    for (unsigned int i = 0; i < rows*2;)
    {
            iniSettings->setArrayIndex(i);
            index = currencyNameValues->index(j, 2, QModelIndex());
            iniSettings->setValue("value", currencyNameValues->data(index, Qt::DisplayRole).toString());
            i++;
            iniSettings->setArrayIndex(i);
            index = currencyNameValues->index(j, 3, QModelIndex());
            iniSettings->setValue("value", currencyNameValues->data(index, Qt::DisplayRole).toString());
            i++;
            j++;
    }
    iniSettings->endArray();
    iniSettings->sync();

    delete iniSettings;
}
