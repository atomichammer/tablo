#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog {

        Q_OBJECT

public:
        AboutDialog();
        Ui::aboutDialog ui;
};
#endif // ABOUTDIALOG_H
