#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H

#include <QObject>
#include <QDateTime>
#include <QStringList>

class LogContainer : public QObject
{
Q_OBJECT
public:
    static LogContainer* _self;
protected:
    LogContainer(QObject *parent = 0);
public:
    static LogContainer* Instance()
    {
        if(!_self) _self = new LogContainer();
        return _self;
    }
    QStringList data;
    void append(QString str);
public slots:
    void clear();

private:
    QDateTime logTime;

};

#endif // LOGCONTAINER_H
