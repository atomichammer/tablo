#ifndef CURRENCYNAMESDIALOG_H
#define CURRENCYNAMESDIALOG_H

#include "ui_currencynamesdialog.h"
#include "currencynamesvalues.h"
#include <QDialog>

namespace Ui {
class CurrencyNamesDialog;
}

class CurrencyNamesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CurrencyNamesDialog(CurrencyNamesValues *_model, QWidget *parent = 0);
    ~CurrencyNamesDialog();
    
private slots:
    void on_pbAdd_clicked();
    void on_pbRemove_clicked();

private:
    Ui::CurrencyNamesDialog *ui;
    CurrencyNamesValues *model;
};

#endif // CURRENCYNAMESDIALOG_H
