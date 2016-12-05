#include "log.h"
#include "logmodel.h"

Log::Log(QWidget *parent):QDialog(parent)
{
    ui.setupUi(this);

    LogModel *logModel = LogModel::Instance();
    ui.listView->setModel(logModel);
    connect(logModel,SIGNAL(itemAdded()), ui.listView, SLOT(scrollToBottom()));
    connect(logModel, SIGNAL(itemAdded()), ui.listView, SLOT(update()));

    connect(ui.pbClear, SIGNAL(clicked()), logModel, SLOT(clear()));
    ui.listView->scrollToBottom();
}
