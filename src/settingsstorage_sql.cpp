#include "settingsstorage_sql.h"


#include <QSettings>
#include <QModelIndex>
#include <QDir>
#include <QMessageBox>
#include <QTime>
#include <QtSerialPort/QtSerialPort>
#include "devicecontroller.h"

SettingsStorage_sql *SettingsStorage_sql ::_self=NULL;

SettingsStorage_sql::SettingsStorage_sql()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("main.sqlite");
//    sdb.setDatabaseName(":memory:");
    if (!sdb.open()) {
          QMessageBox::critical(0, "SQLite Error", "Error opening database.\n" + sdb.lastError().text());
    }

    checkTables();

    ports = new QList<PORT>;
    portName = "COM1";
    loadCommunicationsSettings();
    loadGlobalDisplaySettings();
    loadDevices();
}

void SettingsStorage_sql::checkTables()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS devices ("
                   "number integer PRIMARY KEY NOT NULL, "
                   "enabled BOOLEAN, "
                   "address integer, "
                   "type VARCHAR(16), "
                   "length integer, "
                   "name VARCHAR(16), "
                   "swap BOOLEAN"
                   ");");
    query.exec();

    query.prepare("CREATE TABLE IF NOT EXISTS settings ("
                   "port VARCHAR(7), "
                   "speed integer, "
                   "timeout integer, "
                   "maxtries integer"
                   ");");
    query.exec();

    query.prepare("CREATE TABLE IF NOT EXISTS curr_order ("
                   "number INTEGER, "
                   "id INTEGER, "
                   "length INTEGER"
                   ");");
    query.exec();

    query.prepare("CREATE TABLE IF NOT EXISTS currencies ("
                   "names VARCHAR(4), "
                   "val_buy VARCHAR(10), "
                   "val_sell VARCHAR(10)"
                   ");");
    query.exec();
}

void SettingsStorage_sql::loadDevices()
{
    QSqlQuery query, subquery;

    //Loading devices list
    devices = new DeviceModel();

    qDebug() << "Currencies: " << currencyNameValues->rowCount(QModelIndex());

    DeviceController *devControl = new DeviceController(devices);
    QModelIndex index;

    query.prepare("SELECT number, enabled, address, type, length, name, swap FROM devices");
    query.exec();

    while(query.next())
    {
        int number = query.value(0).toInt();
        bool enabled = query.value(1).toBool();
        int address = query.value(2).toInt();
        int type = query.value(3).toInt();
        int length = query.value(4).toInt();
        QString name = query.value(5).toString();
        bool swap = query.value(6).toBool();
        index = devControl->addDevice(enabled, address, name, type, 0, getPortName());

        devControl->setStatus(&index, 1);
        devControl->setLength(&index, length);
        devControl->setSwap(&index, swap);
        //load currencies order

        subquery.prepare("SELECT id, length FROM curr_order WHERE number=:number");
        subquery.bindValue(":number", number);
        subquery.exec();

        //config, where currency id and lengths are stored
        QStandardItemModel *currConfig;
        currConfig = devices->getList().at(number).currConfig;

        int i = 0;

        while(subquery.next())
        {
            int row = currConfig->rowCount(QModelIndex());
            QStandardItem *item = new QStandardItem();
            item->setData(subquery.value(0).toInt(), Qt::DisplayRole);
            item->setFlags(item->flags() & ~(Qt::ItemIsDropEnabled));
            currConfig->setItem(row, 0, item);

            QStandardItem *item0 = new QStandardItem();
            item0->setData(subquery.value(1).toInt(), Qt::DisplayRole);
            item0->setFlags(item0->flags() & ~(Qt::ItemIsDropEnabled));
            currConfig->setItem(row, 1, item0);
            i++;
        }
    }
}

void SettingsStorage_sql::loadCommunicationsSettings()
{
    QSqlQuery query;

    query.prepare("SELECT port, maxtries, timeout FROM settings");
    query.exec();

    if(query.next())
    {
        portName = query.value(0).toString();
        maxTries = query.value(1).toInt();
        timeout = query.value(2).toInt();
    }
    query.finish();
}

void SettingsStorage_sql::loadGlobalDisplaySettings()
{
    QSqlQuery query;
    currencyNameValues = new CurrencyNamesValues();

    query.prepare("SELECT names, val_buy, val_sell FROM currencies");
    query.exec();

    while(query.next())
    {
        currencyNameValues->appendRow(query.value(0).toString(),
                                      query.value(1).toString(),
                                      query.value(2).toString());
    }
    query.finish();
}

void SettingsStorage_sql::saveDevices()
{
    QTime timer(0,0,0,0);
    timer.restart();

    QSqlQuery query, subquery;

    query.prepare("DELETE FROM devices");
    query.exec();

    query.prepare("DELETE FROM curr_order");
    query.exec();
    //qDebug() << timer.elapsed();

    QModelIndex index;
    QVariantList numbers;
    QVariantList ids;
    QVariantList lengths;

    int count = devices->rowCount(QModelIndex());
    if(count)
    {
        sdb.transaction();

        for (int i = 0; i < count; ++i)
        {
            DEVICE dev = devices->getDevice(i);

            query.prepare("INSERT into devices (number, enabled, address, type, length, name, swap)"
                          "VALUES(:number, :enabled, :address, :type, :length, :name, :swap)");
            query.bindValue(":number", i);
            query.bindValue(":enabled", dev.enabled);
            query.bindValue(":address", dev.address);
            query.bindValue(":type", dev.type);
            query.bindValue(":length", dev.length);
            query.bindValue(":name", dev.name);
            query.bindValue(":swap", dev.swap);
            query.exec();

            QStandardItemModel *currConfig;
            currConfig = devices->getList().at(i).currConfig;
            int total = currConfig->rowCount(QModelIndex());
            qDebug() << "Rowcount: " << total;

            numbers.clear();
            ids.clear();
            lengths.clear();

            subquery.prepare("INSERT into curr_order VALUES (?, ?, ?)");
            for(int j = 0; j < total; j++)
            {
                numbers.append(i);
                index = currConfig->index(j, 0, QModelIndex());
                ids.append(currConfig->data(index, Qt::DisplayRole).toInt());
                index = currConfig->index(j, 1, QModelIndex());
                lengths.append(currConfig->data(index, Qt::DisplayRole).toInt());
            }
            subquery.addBindValue(numbers);
            subquery.addBindValue(ids);
            subquery.addBindValue(lengths);
            subquery.execBatch();
        }
        sdb.commit();
    }
    qDebug() << timer.elapsed();
}

void SettingsStorage_sql::saveSettings()
{
    QTime timer(0,0,0,0);
    timer.restart();
    QSqlQuery query;
    sdb.transaction();
    query.prepare("DELETE FROM settings");
    query.exec();

    query.prepare("INSERT into settings (port, maxtries, timeout) VALUES (:port, :maxtries, :timeout)");
    query.bindValue(":port", getPortName());
    query.bindValue(":maxtries", getMaxTries());
    query.bindValue(":timeout", getTimeout());
    query.exec();

    query.prepare("DELETE FROM currencies");
    query.exec();

    unsigned char rows = currencyNameValues->rowCount(QModelIndex());

    QVariantList names;
    QVariantList buy;
    QVariantList sell;

    CURR item;
    query.prepare("INSERT into currencies VALUES (?, ?, ?)");
    for (unsigned int i = 0; i < rows; i++)
    {
        item = currencyNameValues->getRow(i);
        names.append(item.name);
        buy.append(item.buy);
        sell.append(item.sell);
    }
    query.addBindValue(names);
    query.addBindValue(buy);
    query.addBindValue(sell);
    query.execBatch();
    sdb.commit();
    qDebug() << timer.elapsed();
}
