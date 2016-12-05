#include <QDebug>
#include "logcontainer.h"


LogContainer *LogContainer ::_self=NULL;

LogContainer::LogContainer(QObject *parent) :
    QObject(parent)
{
}

void LogContainer::append(QString str)
{
    data.append(logTime.currentDateTime().toString("dd.MM.yy|hh:mm:ss - ") + str);
}

void LogContainer::clear()
{
    data.clear();
    append(tr("Log cleared."));
}
