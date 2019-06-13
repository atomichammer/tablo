#include "dialogdetails.h"
#include "cbdelegate.h"
#include "datasender.h"

#include <QDebug>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QSpinBox>


DialogDetails::DialogDetails(CurrencyNamesValues *currencyNames, DeviceModel *devicesModel, QModelIndex &index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetails)
{
    ui->setupUi(this);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    cbDelegate *delegate = new cbDelegate();
    delegate->setCurrencyList(currencyNames);
    currNames = currencyNames;

    ui->tableView->setItemDelegateForColumn(0, delegate);

    currModel = (devicesModel->getList().at(index.row())).currConfig;
    ui->tableView->setModel(currModel);



    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(devicesModel);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->addMapping(ui->lbAddress, 1, "value");
    mapper->addMapping(ui->lbType, 2, "text");
    mapper->addMapping(ui->leName, 3);
    mapper->addMapping(ui->lbTotalReceived, 5, "text");
    mapper->addMapping(ui->cbSwap, 7);
    mapper->setCurrentModelIndex(index);
    connect(ui->cbSwap,SIGNAL(clicked()), ui->cbSwap, SLOT(setFocus()));


    QModelIndex idx = devicesModel->index(index.row(), 5, QModelIndex());
    actualLength = devicesModel->data(idx, Qt::DisplayRole).toInt();
    idx = devicesModel->index(index.row(), 1, QModelIndex());
    oldAddr = devicesModel->data(idx, Qt::DisplayRole).toInt();
    idx = devicesModel->index(index.row(), 2, QModelIndex());

    //if clock
    if(devicesModel->data(idx, Qt::EditRole) == 3)
    {
        ui->pbAdd->setEnabled(false);
        ui->pbRemove->setEnabled(false);
        ui->tableView->setEnabled(false);
    }

    //if new version
    if(devicesModel->data(idx, Qt::EditRole) != 4)
    {
        ui->frame_2->setVisible(false);
    }

    //if dynamic
    if(devicesModel->data(idx, Qt::EditRole) == 2)
    {
        ui->frBrightness->setVisible(false);
    }

//    QModelIndex idx = devicesModel->index(index.row(), 5, QModelIndex());
//    ui->cbSwap->setChecked(devicesModel->data(idx, Qt::DisplayRole));

    connect(currModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateLength(QModelIndex,QModelIndex)));
    updateLength(QModelIndex(),QModelIndex());

    ui->tableView->resizeColumnsToContents();

    //connect(ui->lbAddress, SIGNAL(valueChanged(int)), this, SLOT(changeAddress()));
    //oldAddr = ui->lbAddress->value();
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateBar()));
    timer.setInterval(1000);
    //get Threshold value
    //getThreshold();
}

DialogDetails::~DialogDetails()
{
    timer.stop();
    delete ui;
}

void DialogDetails::getThreshold()
{
    DataSender *sender = DataSender::Instance();

    if(!sender->open())
    {
        return;
    }

    QByteArray bytes, result;
    //int res = 0;
    result = sender->sendCommand(0x09, oldAddr, bytes, 2);
    if(result.contains(char(0xE0)))
    {
        qDebug() << "THreshold: " << (unsigned char)result[1];
        ui->sbThreshold->setValue(255 - (unsigned char)result[1]);
    }

    sender->close();
}


void DialogDetails::on_pbRemove_clicked()
{
    QModelIndex index;
    index = ui->tableView->currentIndex();
    if(index.isValid())
    {
        currModel->removeRow(index.row());
        updateLength(QModelIndex(),QModelIndex());
    }
}

void DialogDetails::updateLength(QModelIndex,QModelIndex)
{
    length = 0;
    QModelIndex index;
    for(int i = 0; i < currModel->rowCount(); i++)
    {
        index = currModel->index(i,1,QModelIndex());
        length += currModel->data(index).toInt();
    }
    length *= 2;
    ui->lbTotal->setText(QString::number(length));
}

void DialogDetails::on_pbAdd_clicked()
{
    if(currModel->rowCount() < currNames->rowCount(QModelIndex()))
    {
        int row = currModel->rowCount(QModelIndex());
        QStandardItem *item = new QStandardItem();
        item->setData(currModel->rowCount(),Qt::DisplayRole);
        item->setFlags(item->flags() & ~(Qt::ItemIsDropEnabled));
        currModel->setItem(row, 0, item);

        QStandardItem *item0 = new QStandardItem();
        item0->setData(5, Qt::DisplayRole);
        item0->setFlags(item0->flags() & ~(Qt::ItemIsDropEnabled));
        currModel->setItem(row, 1, item0);

    } else
    {
        QMessageBox::critical(this, tr("Error"),
                                        tr("No more currencies in list."),
                                        QMessageBox::Ok,
                                        QMessageBox::Ok);
    }

}

void DialogDetails::on_pbOk_clicked()
{
    if((length != actualLength) && (actualLength > 0))
    {
        QMessageBox::critical(this, tr("Error"),
                                        tr("Lengths do not match!\nActual length is %1").arg(actualLength),
                                        QMessageBox::Ok,
                                        QMessageBox::Ok);
    }
    else
    {
        this->accept();
        this->close();
    }
}

void DialogDetails::on_pbCancel_clicked()
{
    this->reject();
    this->close();
}

void DialogDetails::changeAddress()
{
    ui->lbAddress->setEnabled(false);
    DataSender *sender = DataSender::Instance();

    if(!sender->open())
    {
        return;
    }

    QByteArray bytes;
    //bytes.append(ui->lbAddress->value());
    qDebug() << "changeAddress";
/*
    if(sender->sendCommand(0x06, oldAddr, bytes).contains(char(0xE0)))
    {
        oldAddr = ui->lbAddress->value();
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        ui->lbAddress->setValue(oldAddr);
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
    */
    sender->close();
    ui->lbAddress->setEnabled(true);
}

void DialogDetails::on_pushButton_clicked()
{
    if(timer.isActive())
    {
        timer.stop();
        ui->pushButton->setText("Start");
    }
    else
    {
        timer.start();
        ui->pushButton->setText("Stop");
    }
}

unsigned char DialogDetails::getLuminosity()
{
    unsigned char res = 255-ui->pbLuminosity->value();
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return res;
    }

    QByteArray bytes, result;
    result = sender->sendCommand(0x08, oldAddr, bytes, 2);
    if(result.contains(char(0xE0)))
    {
        res = result.at(1);
    }

    sender->close();
    return res;
}

void DialogDetails::updateBar()
{
    ui->pbLuminosity->setValue(255 - getLuminosity());
}

void DialogDetails::on_pbSetThreshold_clicked()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }

    QByteArray bytes, result;
    bytes.append(255 - ui->sbThreshold->value());
    result = sender->sendCommand(0x07, oldAddr, bytes, 1);
    if(result.contains(char(0xE0)))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
    sender->close();
}

void DialogDetails::on_pbSetBrighness_clicked()
{
    //0xAA 0x05 0x00 0x00 0x01 0x01

    QByteArray bytes, result;
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }
    //bytes.append(char(0x04)); //fcode
    unsigned short PWM = 1024 - qPow(2, ui->vsDayBright->value());
    qDebug() << "PWM Day: " << PWM;
    qDebug() << ((PWM >> 2) & 0xFF);
    bytes.append((char)(PWM >> 2) & 0xFF );
//    bytes.append( ((PWM & 3) << 4) | 0x0C );
    bytes.append( ((PWM & 3) << 4) | 0x0C);
    PWM = 1024 - qPow(2, ui->vsNightBright->value());
    qDebug() << "PWM Night: " << PWM;
    bytes.append( (PWM >> 2) & 0xFF );
//    bytes.append( ((PWM & 3) << 4) | 0x0D );
    bytes.append( ((PWM & 3) << 4) | 0x0D);
    result = sender->sendCommand(0x04, oldAddr, bytes, 1);
    sender->close();
    qDebug() <<"Brightness: " << bytes;
    if(result.contains(char(0xE0)))
    {
        QMessageBox::information (this, this->windowTitle(), tr("Successful!"));
    } else
    {
        QMessageBox::critical (this, this->windowTitle(), tr("No Answer!"));
    }
}

void DialogDetails::on_pbGetData_clicked()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }

    QByteArray bytes, result;
    //bytes.append();
    result = sender->sendCommand(0x0B, oldAddr, bytes, 1);
    sender->close();
}

void DialogDetails::on_pbSendData_clicked()
{
    DataSender *sender = DataSender::Instance();
    if(!sender->open())
    {
        return;
    }

    QByteArray bytes, result;
    for(int i = 0; i < 51; i++)
    {
        bytes.append(i);
    }
    result = sender->sendCommand(0x03, oldAddr, bytes, 51);
    sender->close();
}
