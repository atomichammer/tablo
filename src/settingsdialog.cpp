#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QToolBar>
#include <QVBoxLayout>

#include "settingsdialog.h"
#include "settings/devicemodel.h"
#include "settings/devicecontroller.h"
#include "settings/dialogdetails.h"
#include "settings/currencynamesdialog.h"
#include "datasender.h"
#include "port.h"
#include "ftwrapper.h"
#include "actionbutton.h"

#define SCAN_LIMIT  255


SettingsDialog::SettingsDialog(MainForm *parent)
			:QDialog(parent)
{
    ui.setupUi(this);
    settings = SettingsStorage_sql::Instance();
    ui.expertWidget->setVisible(0);

    EnumComm();

    QSizePolicy sp = ui.cbPort->view()->sizePolicy();
    sp.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    ui.cbPort->view()->setSizePolicy(sp);

    shortcut = new QShortcut(QKeySequence("Ctrl+K"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(toggleAdvancedMode()));

    ui.sbTries->setValue(settings->getMaxTries());
    ui.sbTimeout->setValue(settings->getTimeout());
    ui.lbScan->setText(tr("Address: %1/%2").arg(0).arg(SCAN_LIMIT));
    ui.progressBar->setMaximum(SCAN_LIMIT);


    //setting view for the devices model
    ui.tvDevices->setModel(settings->getDevices());
    ui.tvDevices->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tvDevices->resizeColumnsToContents();
    ui.tvDevices->setToolTip(tr("List of all found devices"));
    ui.tvDevices->hideColumn(6);
    ui.tvDevices->hideColumn(7);
    ui.tvDevices->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui.tvDevices, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui.pbUpdate, SIGNAL(clicked()), this, SLOT(EnumComm()));
    connect(ui.pbCurrencies, SIGNAL(clicked()), this, SLOT(onPBEditClicked()));
    connect(ui.cbPort, SIGNAL(currentIndexChanged(int)), this, SLOT(portChange(int)));

    scanning = false;

    createActions();

   // this->adjustSize();
    this->resize(minimumSizeHint());
}

void SettingsDialog::createActions()
{
    actDay = new QAction(QIcon(":/icons/day.png"), tr("Set Day"), this);
    actNight = new QAction(QIcon(":/icons/night.png"), tr("Set Night"), this);
    actOff = new QAction(QIcon(":/icons/exit.png"), tr("Turn Off"), this);
    actRefresh = new QAction(QIcon(":/icons/refresh.png"), tr("Refresh\nstate"), this);
    actSetTime = new QAction(QIcon(":/icons/refresh.png"), tr("Set Time"), this);

    actDay->setObjectName("Day");
    actNight->setObjectName("Night");
    actOff->setObjectName("Off");
    actRefresh->setObjectName("Refresh");
    actSetTime->setObjectName("SetTime");

    actDay->setToolTip(tr("Set day mode for the selected device"));
    actNight->setToolTip(tr("Set night mode for the selected device"));
    actOff->setToolTip(tr("Turn off the selected device"));
    actRefresh->setToolTip(tr("Refresh state of all devices"));
    actSetTime->setToolTip(tr("Set current time"));

    connect(actDay, SIGNAL(triggered()), this, SLOT(sendAct()));
    connect(actNight, SIGNAL(triggered()), this, SLOT(sendAct()));
    connect(actOff, SIGNAL(triggered()), this, SLOT(sendAct()));
    connect(actRefresh, SIGNAL(triggered()),this, SLOT(checkStatus()));
    connect(actSetTime, SIGNAL(triggered()), this, SLOT(actSendTime()));

    QVBoxLayout *vLayout = new QVBoxLayout();
    ActionButton *pbDay = new ActionButton();
    pbDay->setAction(actDay);
    ActionButton *pbNight = new ActionButton();
    pbNight->setAction(actNight);
    ActionButton *pbOff = new ActionButton();
    pbOff->setAction(actOff);
    ActionButton *pbRefresh = new ActionButton();
    pbRefresh->setAction(actRefresh);
    ActionButton *pbSetTime = new ActionButton();
    pbSetTime->setAction(actSetTime);

    vLayout->addWidget(pbDay);
    vLayout->addWidget(pbNight);
    vLayout->addWidget(pbOff);
    vLayout->addWidget(pbRefresh);
    vLayout->addWidget(pbSetTime);
    vLayout->addStretch();
    vLayout->setMargin(0);

    ui.widget_2->setLayout(vLayout);
}

void SettingsDialog::actSendTime()
{
    DataSender *dataSender = DataSender::Instance();
    if(!dataSender->open())
    {
        return;
    }
    QByteArray param;

    QModelIndex index = ui.tvDevices->currentIndex();

    int row = index.row();

    if(!index.isValid())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No devices selected!"));
        return;
    }

    unsigned int adr;
    index = settings->getDevices()->index(row, 1, QModelIndex());
    adr = settings->getDevices()->data(index, Qt::DisplayRole).toInt();

    param = getTimeDate();

    if(dataSender->sendCommand(0x0C, adr, param, 1).contains(0xE0))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
    dataSender->close();
}

void SettingsDialog::showContextMenu(QPoint point)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(actDay);
    menu->addAction(actNight);
    menu->addAction(actOff);

    menu->popup(ui.tvDevices->viewport()->mapToGlobal(point));
}

void SettingsDialog::sendAct()
{
    DataSender *dataSender = DataSender::Instance();
    if(!dataSender->open())
    {
        return;
    }
    QByteArray param;

    if(sender()->objectName() == "Day")
        param.append(1);
    if(sender()->objectName() == "Night")
        param.append(4);
    if(sender()->objectName() == "Off")
        param.append(8);

    QModelIndex index = ui.tvDevices->currentIndex();

    int row = index.row();

    if(!index.isValid())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No devices selected!"));
        return;
    }

    unsigned int adr;
    index = settings->getDevices()->index(row, 1, QModelIndex());
    adr = settings->getDevices()->data(index, Qt::DisplayRole).toInt();

    if(dataSender->sendCommand(1, adr, param, 1).contains(0xE0))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
    dataSender->close();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    if(scanning)
    {
        abortScan();
    }
    QDialog::closeEvent(event);
}


void SettingsDialog::toggleAdvancedMode()
{
    if(ui.expertWidget->isVisible())
    {
        ui.expertWidget->setVisible(0);
        disconnect(ui.tvDevices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDialog(QModelIndex)));
    }
    else
    {
        ui.expertWidget->setVisible(1);
        connect(ui.tvDevices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showDialog(QModelIndex)));
    }
    this->resize(minimumSizeHint());
}

void SettingsDialog::portChange(int id)
{
    if(id >= 0)
    {
        settings->setPortName(settings->getPorts()->at(id).name);
        qDebug() << "ID: " << id;
    }
}

void SettingsDialog::checkStatus()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }
    DeviceModel *devices = settings->getDevices();
    DeviceController *devControl = new DeviceController(devices);

    QModelIndex index;
    int address;
    int start;
    QByteArray bytes, data;

    for(int i = 0; i < devices->rowCount(QModelIndex()); i++)
    {
        data.clear();
        index = devices->index(i, 1, QModelIndex());
        address = devices->data(index, Qt::DisplayRole).toInt();
        bytes = sender->sendCommand(5, address, data, 6);

        if(bytes.size() > 0 && bytes.contains(char(0xE0)))
        {
            start = bytes.indexOf(char(0xE0), 0);
            data = bytes.mid(start, bytes.at(start+1));

            devControl->setStatus(&index, (int)data.at(4));
            if(data.length() > 6)
            {
                devControl->setLength(&index, (int)data.at(5));
            } else
            {
                devControl->setLength(&index, 0);
            }
        }
    }
    sender->close();
    delete devControl;
}

void SettingsDialog::startScan()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }
    settings->getDevices()->setCurrencyRows(settings->getCurrencyNameValuesModel()->rowCount(QModelIndex()));
    //0xAA 0x05 0x00 0x00 0x01 0x01
    QByteArray bytes, data;
    unsigned char start;
    ui.pbScan->setText(tr("Stop"));
    //clear model
    settings->getDevices()->clear(QModelIndex());
    ui.tvDevices->reset();

    ui.progressBar->setValue(0);
    ui.lbScan->setText(tr("Address: %1/%2").arg(0).arg(SCAN_LIMIT));

    scanning = true;

    DeviceController *devControl = new DeviceController(settings->getDevices());
    QString name;

    for(int i = 0; i < SCAN_LIMIT; i++)
    {
        if(!scanning)
            break;
        data.clear();
        bytes = sender->sendCommand(5, i, data, 6);
        QApplication::processEvents();

        //qDebug()<< i << ")" << bytes.toHex();
        ui.progressBar->setValue(i);
        ui.lbScan->setText(tr("Address: %1/%2").arg(i).arg(SCAN_LIMIT));
        if(bytes.size() > 0 && bytes.contains(char(0xE0)))
        {
            start = bytes.indexOf(char(0xE0), 0);
            data = bytes.mid(start, bytes.at(start+1));

            name = QString("Display_%1").arg(QString::number(data.at(2)));
            QModelIndex index = devControl->addDevice(true, data.at(2), name, data.at(3), data.at(4), settings->getPortName());
            if(data.length() > 6)
            {
                devControl->setLength(&index, (int)data.at(5));
            } else
            {
                devControl->setLength(&index, 0);
            }
        }
    }

    for(int i = 0; i < settings->getDevices()->rowCount(QModelIndex()); i++)
    {
        QModelIndex index = settings->getDevices()->index(i, 0, QModelIndex());
        showDialog(index);
    }

    abortScan();
    delete devControl;
}

void SettingsDialog::abortScan()
{
    DataSender *sender = DataSender::Instance();
    sender->close();
    ui.pbScan->setText(tr("Scan"));
    ui.progressBar->setValue(0);
    ui.lbScan->setText(tr("Address: %1/%2").arg(0).arg(SCAN_LIMIT));
    scanning = false;
}

void SettingsDialog::addRow()
{
    this->adjustSize();
    //unsigned char row;
    //row = settings->dataModel->rowCount();
}

void SettingsDialog::removeRow()
{
    //settings->dataModel->removeRow(ui.tvLayout->currentIndex().row());
}

void SettingsDialog::on_pbChange_clicked()
{
    DataSender *sender = DataSender::Instance();

    if(!sender->open())
    {
        return;
    }

    QModelIndex index = ui.tvDevices->currentIndex();

    int row = index.row();

    if(!index.isValid())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No devices selected!"));
        return;
    }

    unsigned int oldAdr;
    index = settings->getDevices()->index(row, 1, QModelIndex());
    oldAdr = settings->getDevices()->data(index, Qt::DisplayRole).toInt();


    QByteArray bytes;
    bytes.append(ui.sbNewAddr->value());

    if(sender->sendCommand(0x06, oldAdr, bytes, 1).contains(char(0xE0)))
    {
        settings->getDevices()->setData(index, ui.sbNewAddr->value(), Qt::DisplayRole);
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
    sender->close();
}

void SettingsDialog::on_pbForce_clicked()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }
    QMessageBox msgBox(this);
    msgBox.setText(tr("Attention!"));
    msgBox.setInformativeText(tr("You must have only one device connected!\nIf unsure - press \"No.\"\nContinue?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Critical);
    int ret = msgBox.exec();

    if(ret == QMessageBox::No)
    {
        return;
    }

    QByteArray bytes;
    bytes.append(ui.sbNewAddr->value());
    //sender->sendCommand(6, 0xFF, bytes);

    if(sender->sendCommand(6, 0xFF, bytes, 1).contains(char(0xE0)))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
        //settings->devices->setData(index, ui.sbNewAddr->value(), Qt::DisplayRole);
    }
    sender->close();
}

void SettingsDialog::EnumComm()
{
    /*
    FTWrapper wrapper;
    if(wrapper.isSuccessful())
    {
        wrapper.scanPorts();
    }
    */

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    if(ports.count() > 0)
    {
        ui.cbPort->clear();
        settings->clearPorts();
    }
//
    /*
    if(wrapper.getPorts().count() > 0)
    {
        foreach(PORT port, wrapper.getPorts())
        {
            settings->addPort(port);
            qDebug() << port.description;
        }
    }
    */
//
    qDebug() << "List of ports:";
    bool flag = false;
    foreach (QSerialPortInfo info, ports)
    {
        if(info.portName() != "")
        {
            PORT port;
            port.description = info.description();
            port.name = info.portName();
            flag = false;
            foreach(PORT baseport, *settings->getPorts())
            {
                if(baseport.name == port.name)
                {
                    flag = true;
                    break;
                }
            }

            if(!flag) settings->addPort(port);
        }
    }
//
    int i = 0;
    int id = 0;
    foreach(PORT port, *settings->getPorts())
    {
        ui.cbPort->addItem(port.name + ": " + port.description);
        if(port.name == settings->getPortName())
        {
            id = i;
        }
        i++;
    }
//
    if (id > ui.cbPort->count()) id = 0;
    ui.cbPort->setCurrentIndex(id);
    ui.cbPort->setToolTip(tr("List of available ports"));
}


void SettingsDialog::on_pbScan_clicked()
{
    if(scanning)
    {
        scanning = false;
    }
    else
    {
        startScan();
    }
}

void SettingsDialog::showDialog(QModelIndex index)
{
    DialogDetails *dialog = new DialogDetails(settings->getCurrencyNameValuesModel(), settings->getDevices(), index, this);
    if(dialog->exec())
    {
        delete dialog;
        ui.tvDevices->resizeColumnsToContents();
    }
}

void SettingsDialog::onPBEditClicked()
{
    CurrencyNamesDialog *dialog = new CurrencyNamesDialog(settings->getCurrencyNameValuesModel(), this);

    if(dialog->exec())
    {
        delete dialog;
    }
}

void SettingsDialog::on_pbDetails_clicked()
{
    if(ui.tvDevices->currentIndex().isValid())
    {
        showDialog(ui.tvDevices->currentIndex());
    }
}

void SettingsDialog::on_pbReset_clicked()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }
    QByteArray bytes;

    QModelIndex index = ui.tvDevices->currentIndex();

    int row = index.row();

    if(!index.isValid())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No devices selected!"));
        return;
    }

    unsigned int oldAdr;
    index = settings->getDevices()->index(row, 1, QModelIndex());
    oldAdr = settings->getDevices()->data(index, Qt::DisplayRole).toInt();

    if(sender->sendCommand(0x0A, oldAdr, bytes, 1).contains(char(0xE0)))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    }
    sender->close();
}

void SettingsDialog::on_sbTimeout_valueChanged(int arg1)
{
    settings->setTimeout(arg1);
}

QByteArray SettingsDialog::getTimeDate()
{
    QByteArray data;
    QStringList list;
    QDateTime *dateTime = new QDateTime();

    QString date = dateTime->currentDateTime().toString("yy.MM.dd.hh.mm.ss");
    list = date.split(".");

    for (int i = 0; i<list.size(); i++)
    {
        data.append(list[i].toInt());
    }

    return data;
}
