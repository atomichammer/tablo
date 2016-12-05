#include "settings/devicemodel.h"
//#include "settingsstorage.h"
#include "settingsstorage_sql.h"
#include "currencynamesvalues.h"
#include <QDebug>

DeviceModel::DeviceModel(QObject *parent) : QAbstractTableModel(parent)
{
     currencyNames = new QStringListModel;
     currRows = 0;
     //settings = SettingsStorage::Instance();
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return list.count();
}

bool DeviceModel::setData( const QModelIndex & index, const QVariant & value, int role)
{
    Q_UNUSED(role)
    if (index.isValid())
    {
        switch(index.column())
        {
            case 0:
                {
                    list[index.row()].enabled = value.toBool();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 1:
                {
                    list[index.row()].address = value.toChar().toLatin1();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 2:
                {
                    list[index.row()].type = value.toInt();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 3:
                {
                    list[index.row()].name = value.toString();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 4:
                {
                    list[index.row()].status = value.toInt();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 5:
                {
                    list[index.row()].length = value.toInt();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 6:
                {
                    list[index.row()].portName = value.toString();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            case 7:
                {
                    list[index.row()].swap = value.toBool();
                    emit dataChanged(index, index);
                    emit changed();
                    return true;
                }
            default: return false;
        }
    }
    return false;
}

QVariant DeviceModel::data ( const QModelIndex & index, int role ) const
{
    if (index.isValid())
    {
        if(role == Qt::ToolTipRole)
        {
            QString currency;
            QModelIndex idx;
            SettingsStorage_sql *settings = SettingsStorage_sql::Instance();
            CurrencyNamesValues *model = settings->getCurrencyNameValuesModel();
            int id;
            currency.append(tr("Port: ") + list[index.row()].portName + "\n");
            for(int i = 0; i < list[index.row()].currConfig->rowCount(QModelIndex()); i++)
            {
                idx = list[index.row()].currConfig->index(i, 0, QModelIndex());
                id = list[index.row()].currConfig->data(idx, Qt::DisplayRole).toInt();
                idx = model->index(id, 1, QModelIndex());
                currency.append(model->data(idx, Qt::DisplayRole).toString() + ": ");
                idx = list[index.row()].currConfig->index(i, 1, QModelIndex());
                currency.append(list[index.row()].currConfig->data(idx, Qt::DisplayRole).toString() + "\n");
            }
            currency.remove(currency.length()-1, 1);
            return currency;
        }
        switch(index.column())
        {
            case 0:
                {
                    if(role == Qt::CheckStateRole)
                        return list[index.row()].enabled ? Qt::Checked : Qt::Unchecked;
                    break;
                }
            case 1:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                        return list[index.row()].address;
                    break;
                }
            case 2:
                {
                    if(role == Qt::DisplayRole)
                        switch(list[index.row()].type)
                        {
                            case 0x04:
                            case 0x01: return "Static";
                            case 0x02: return "Dynamic";
                            case 0x03: return "Clock";
                            default : return "Unknown";
                        }
                    if(role == Qt::EditRole)
                    {
                        return list[index.row()].type;
                    }
                    break;

                }
            case 3:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole){
                        return list[index.row()].name;
                    }
                    break;
                }
            case 4:
                {
                    if(role == Qt::DisplayRole)
                    {
                        switch(list[index.row()].status)
                        {
                            case 0x00: return tr("Offline");
                            case 0x01: return tr("Day");
                            case 0x02: return tr("Night");
                            case 0x03: return tr("Off");
                            default : return tr("Unknown");
                        }
                        break;
                    }

                    if(role == Qt::BackgroundColorRole)
                    {
                        switch(list[index.row()].status)
                        {
                            case OFFLINE: return QVariant(QColor(Qt::red));
                            case DAY: return QVariant(QColor(Qt::green));
                            case NIGHT: return QVariant(QColor(Qt::green));
                            case OFF: return QVariant(QColor(Qt::green));
                            default : return QVariant(QColor(Qt::yellow));
                        }
                        break;
                    }

                    if(role == Qt::EditRole)
                    {

                        return list[index.row()].status;
                    }
                    break;
                }
                case 5:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                    {
                        return list[index.row()].length;
                    }
                    break;
                }
                case 6:
                    {
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                        {
                            return list[index.row()].portName;
                        }
                        break;
                    }
                case 7:
                        if(role == Qt::DisplayRole || role == Qt::EditRole)
                        {
                            return list[index.row()].swap;
                        }
                        if(role == Qt::CheckStateRole)
                        {
                            return list[index.row()].swap ? Qt::Checked : Qt::Unchecked;
                        }
                        break;
                default: return QVariant();
        }
    }
    return QVariant();
}

bool DeviceModel::insertRows ( int row, int count, const QModelIndex & parent)
{

    DEVICE dev;
    dev.enabled = true;
    dev.address = 0xFF;
    dev.name = "Display";
    dev.type = 0;
    dev.status = 0;
    dev.portName = "COM1";
    dev.swap = false;

    dev.currConfig = new QStandardItemModel();
    int cnt = 5;
    for (int i = 0; i < currRows; i++)
    {
        QStandardItem *item0 = new QStandardItem();
        item0->setData(i,Qt::DisplayRole);
        item0->setFlags(item0->flags() & ~(Qt::ItemIsDropEnabled));
        dev.currConfig->setItem(i, 0, item0);
        QStandardItem *item1 = new QStandardItem();
        if(i > 1) cnt = 4;
        item1->setData(cnt, Qt::DisplayRole);
        item1->setFlags(item1->flags() & ~(Qt::ItemIsDropEnabled));
        dev.currConfig->setItem(i, 1, item1);
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        list.insert(row, dev);
    }

    endInsertRows();

    emit dataChanged(parent, parent);
    emit changed();
    return true;
}

bool DeviceModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);
    if(row > list.count())
        return false;
    if(row+count > list.count())
    {
        count = list.count() - row;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i=0;i<count;++i)
    {
        list.removeAt(row);
    }
    endRemoveRows();
    emit dataChanged(parent, parent);
    return true;
}

void DeviceModel::clear(const QModelIndex & parent)
{
    Q_UNUSED(parent);

    if(list.count() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, list.count() - 1);
        list.clear();
        endRemoveRows();
        emit dataChanged(parent, parent);
    }
}

void DeviceModel::appendRow()
{
    insertRows(list.length(), 1, QModelIndex());
}

QList<DEVICE> DeviceModel::getList()
{
    return list;
}

 QVariant DeviceModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         switch (section) {
             case 0:
                 return tr("E");

             case 1:
                 return tr("Address");

             case 2:
                 return tr("Type");

             case 3:
                 return tr("Name");

             case 4:
                 return tr("Status");

             case 5:
                 return tr("Digits");

             default:
                 return QVariant();
         }
     }
     return QVariant();
 }

 DEVICE DeviceModel::getDevice(int row)
 {
     return list.at(row);
 }

Qt::ItemFlags DeviceModel::flags(const QModelIndex &index) const
 {
     Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

     if (index.isValid())
     {
         switch(index.column())
         {
             case 0:
                 return defaultFlags | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
             /*case 3:
                 return defaultFlags  | Qt::ItemIsEditable;*/
             default:
                 return defaultFlags;
         }
     }
     return defaultFlags;
 }
