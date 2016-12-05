#ifndef DIALOGDETAILS_H
#define DIALOGDETAILS_H

#include "ui_dialogdetails.h"
#include "devicemodel.h"
#include "currencynamesvalues.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QTimer>

namespace Ui {
class DialogDetails;
}

class DialogDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogDetails(CurrencyNamesValues *currencyNames, DeviceModel *model, QModelIndex &index, QWidget *parent = 0);
    ~DialogDetails();
    
private slots:
    void on_pbRemove_clicked();
    void updateLength(QModelIndex,QModelIndex);
    void on_pbAdd_clicked();

    void on_pbOk_clicked();

    void on_pbCancel_clicked();
    void changeAddress();

    void on_pushButton_clicked();
    void updateBar();

    void on_pbSetThreshold_clicked();

    void on_pbSetBrighness_clicked();

private:
    Ui::DialogDetails *ui;
    QStandardItemModel *currModel;
    QStandardItemModel tempModel;
    CurrencyNamesValues *currNames;
    int length;
    int actualLength;
    int oldAddr;
    QTimer timer;
    unsigned char getLuminosity();

    void getThreshold();
};

#endif // DIALOGDETAILS_H
