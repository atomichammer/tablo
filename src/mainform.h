#ifndef MAINFORM_H
#define MAINFORM_H

#include <QtWidgets>
//#include "settingsstorage.h"
#include "settingsstorage_sql.h"
#include "settingsdialog.h"
#include "datasender.h"
#include "mylineedit.h"
#include <QDataWidgetMapper>
#include <QTimer>


#include <windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <winuser.h>
#include <dbt.h>	//Need this for definitions of WM_DEVICECHANGE messages

namespace Ui {
    class MainForm;
}

class MainForm : public QMainWindow {
    Q_OBJECT
public:
    MainForm(QWidget *parent = 0);
    ~MainForm();


signals:
    void usbArrived();
public slots:
    void autoResize();
    void callSettingsDialog();
    void restoreSettings();
    void sendData();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainForm *m_ui;
    SettingsStorage_sql *settings;
    DataSender *sender;
    QWidget *dummyWidget;
    QDataWidgetMapper *mapper;
    QTimer *timer;

    void createGridLayout();
    void closeEvent(QCloseEvent *event);
    bool winEvent(MSG *m, long *result);
    void registerUSB();

private slots:
    void on_actionLogWindow_triggered();
    void on_actionSetNight_triggered();
    void on_actionDayMode_triggered();
    void on_actionTurnOff_triggered();
};

#endif // MAINFORM_H
