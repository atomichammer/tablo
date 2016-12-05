#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>
#include <QVariant>
#include <QIcon>

class LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    static LogModel* _self;
protected:
    LogModel(QObject *parent = 0);
public:
    static LogModel* Instance()
    {
        if(!_self) _self = new LogModel();
        return _self;
    }
    int rowCount(const QModelIndex & parent = QModelIndex() ) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole ) const;
    
    void addEvent(int iconID, QString text);
public slots:
    void clear();
signals:
    void itemAdded();
//    void dataChanged(QModelIndex, QModelIndex);
public slots:

private:
    QStringList list;
    QList<int> idIcons;
    QList<QIcon*> baseIcons;
    
};

#endif // LOGMODEL_H
