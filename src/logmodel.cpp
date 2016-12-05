#include "logmodel.h"
#include <Qt>
#include <QDateTime>
#include <QApplication>

LogModel *LogModel ::_self=NULL;

LogModel::LogModel(QObject *parent) :
    QAbstractListModel(parent)
{
//    QIcon *icon = new QIcon(":/16_ok");
     QIcon *icon = new QIcon(":/icons/refresh.png");
    baseIcons.append(icon);
//    icon = new QIcon(":/16_err");
    icon = new QIcon(":/icons/refresh.png");
    baseIcons.append(icon);
//    icon = new QIcon(":/16_inf");
    icon = new QIcon(":/icons/refresh.png");
    baseIcons.append(icon);
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list.count();
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole)
    {
        return list.at(index.row());
    }
    if(role == Qt::DecorationRole)
    {
        return *baseIcons.at(idIcons.at(index.row()));
    }
    if(role == Qt::BackgroundColorRole)
    {
        switch(idIcons.at(index.row()))
        {
            case 0:
            {
                return QColor::fromRgb(230,255,230);
                break;
            }
            case 1:
            {
                return QColor::fromRgb(255,230,230);
                break;
            }
            case 2:
            {
                return QColor::fromRgb(230,230,255);
                break;
            }
            default:
            {
                return QColor(Qt::white);
            }
        }
    }
    return QVariant();
}

void LogModel::addEvent(int iconID, QString text)
{
    QDateTime logTime;
    beginInsertRows(QModelIndex(), list.count(), list.count());
    //beginResetModel();
    idIcons.append(iconID);
    list.append(logTime.currentDateTime().toString("dd.MM.yy|hh:mm:ss - ") + text);
    endInsertRows();
    //endResetModel();
    //emit dataChanged(createIndex(0,0), createIndex(list.count()-1,0));
    emit itemAdded();
    QApplication::processEvents();
}

void LogModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, idIcons.length());
    idIcons.clear();
    list.clear();
    endRemoveRows();
}
