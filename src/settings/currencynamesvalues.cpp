#include "settings/currencynamesvalues.h"
#include <QStringList>
#include <QDebug>

CurrencyNamesValues::CurrencyNamesValues(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int CurrencyNamesValues::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return currencyList.count();
}

QStringList CurrencyNamesValues::valuesList()
{
    QStringList valuesList;
    int total = currencyList.count();
    for (int i = 0; i < total; ++i)
    {
        valuesList.append(currencyList.at(i).buy);
        valuesList.append(currencyList.at(i).sell);
    }
    return valuesList;
}

bool CurrencyNamesValues::setData( const QModelIndex & index, const QVariant & value, int role)
{
    Q_UNUSED(role)
    if (index.isValid())
    {
        switch(index.column())
        {
            case 0:
                {
                    currencyList[index.row()].id = value.toInt();
                    emit dataChanged(index, index);
                    //emit changed(index);
                    return true;
                }
            case 1:
                {
                    currencyList[index.row()].name = value.toString();
                    emit dataChanged(index, index);
                    //emit changed(index);
                    return true;
                }
            case 2:
                {
                    currencyList[index.row()].buy = value.toString();
                    emit dataChanged(index, index);
                    //emit changed(index);
                    return true;
                }
            case 3:
                {
                    currencyList[index.row()].sell = value.toString();
                    emit dataChanged(index, index);
                    //emit changed(index);
                    return true;
                }

            default: return false;
        }
    }
    return false;
}

QVariant CurrencyNamesValues::data ( const QModelIndex & index, int role ) const
{
    if (index.isValid())
    {
        switch(index.column())
        {
            case 0:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                        return currencyList[index.row()].id;
                    break;
                }
            case 1:
                {
                    if(role == Qt::EditRole || role == Qt::DisplayRole)
                        return currencyList[index.row()].name;
                    break;
                }
            case 2:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                    {
                        return currencyList[index.row()].buy;
                    }
                    break;
                }
            case 3:
                {
                    if(role == Qt::DisplayRole || role == Qt::EditRole)
                    {
                        return currencyList[index.row()].sell;
                    }
                    break;
                }

            default: return QVariant();
        }
    }
    return QVariant();
}

bool CurrencyNamesValues::insertRows ( int row, int count, const QModelIndex & parent)
{
    CURR item;
    item.name = "";
    item.buy = "";
    item.sell = "";

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i)
    {
        currencyList.insert(row, item);
    }

    endInsertRows();
    qDebug() << ("Row inserted");
    emit dataChanged(parent, parent);
    //emit changed();
    return true;
}

bool CurrencyNamesValues::removeRows(int row, int count, const QModelIndex & parent)
{
    qDebug() << "RemoveRows";
    Q_UNUSED(parent);
    if(row > rowCount(QModelIndex()))
        return false;
    if(row+count > rowCount(QModelIndex()))
    {
        count = rowCount(QModelIndex()) - row;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0;i < count; ++i)
    {
        currencyList.removeAt(row);
    }
    endRemoveRows();
    emit dataChanged(parent, parent);
    return true;
}

void CurrencyNamesValues::clear(const QModelIndex & parent)
{
    Q_UNUSED(parent);

    if(rowCount(QModelIndex()) > 0)
    {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()) - 1);
        currencyList.clear();
        endRemoveRows();
        emit dataChanged(parent, parent);
    }
}

void CurrencyNamesValues::appendRow(QString name, QString buy, QString sell)
{
    CURR item;
    item.id = currencyList.count();
    item.name = name;
    item.buy = buy;
    item.sell = sell;

    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    currencyList.append(item);
    endInsertRows();
}

CURR CurrencyNamesValues::getRow(int index)
{
    return currencyList.at(index);
}

QStringList CurrencyNamesValues::getCurrencyNamesList()
{
    QStringList list;
    for (int i = 0; i < rowCount(QModelIndex()); ++i)
    {
        list.append(currencyList.at(i).name);
    }
    return list;
}

 QVariant CurrencyNamesValues::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         switch (section) {
             case 0:
                 return tr("ID");
                 break;
             case 1:
                 return tr("Name");
                 break;

             case 2:
                 return tr("Buy");
                 break;

             case 3:
                 return tr("Sell");

             default:
                 return QVariant();
         }
     }

     if (orientation == Qt::Vertical)
     {
         return QVariant(currencyList.at(section).name);
     }

     return QVariant();
 }

 Qt::DropActions CurrencyNamesValues::supportedDropActions() const
 {
     return Qt::MoveAction | Qt::CopyAction;
 }


Qt::ItemFlags CurrencyNamesValues::flags(const QModelIndex &index) const
 {
     Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

     if (index.isValid())
     {
         return defaultFlags | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
     }
     return defaultFlags | Qt::ItemIsDropEnabled;
 }

