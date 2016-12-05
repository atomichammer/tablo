#ifndef _settings_h_
#define _settings_h_

#include "mainform.h"
//#include "settingsstorage.h"
#include "settingsstorage_sql.h"
#include "ui_settings.h"
#include <QString>
#include <QMenu>
#include <QShortcut>

class MainForm;
class SettingsDialog : public QDialog 
{
	Q_OBJECT
	
public:
    SettingsDialog(MainForm *parent);
    //Timer settings

//    unsigned int getDayHour() { return ui.sbDayHour->value(); }
//    unsigned int getDayMinute() { return ui.sbDayMinute->value(); }
//    unsigned int getNightHour() { return ui.sbNightHour->value(); }
//    unsigned int getNightMinute() { return ui.sbNightMinute->value(); }
//    bool isTimerEnabled() { return ui.checkBox->isChecked(); }

    //Port settings
    QString getPortName() { return ui.cbPort->currentText(); }
    //unsigned int getPortSpeed() { return ui.cbSpeed->currentIndex(); }
    //unsigned int getTimeout() { return ui.sbTimeout->value(); }
    unsigned int getMaxTries() { return ui.sbTries->value(); }
    //bool isTransControlEnabled() { return ui.checkBoxControl->isChecked(); }

    //Brightness settings
    //unsigned int getDayBrightness() { return ui.vsDayBright->value(); }
    //unsigned int getNightBrightness() { return ui.vsNightBright->value(); }

    //Scan Tab


    struct Data {
        bool enabled;
        unsigned char address;
        QString name;
        unsigned char status;
    };

private:
        Ui::settingsDialog ui;
        SettingsStorage_sql *settings;
        QShortcut *shortcut;
        QAction *actDay;
        QAction *actNight;
        QAction *actOff;
        QAction *actRefresh;
        bool scanning;
        void abortScan();

        void createActions();
public slots:
        void startScan();
        void checkStatus();

private slots:
        void on_pbScan_clicked();
        void on_pbForce_clicked();
        void on_pbChange_clicked();

        void addRow();
        void removeRow();
        void EnumComm();
        void portChange(int id);

        void on_pbDetails_clicked();
        void showDialog(QModelIndex index);
        void onPBEditClicked();
        void toggleAdvancedMode();
        void showContextMenu(QPoint point);
        void sendAct();
        void on_pbReset_clicked();
};

#endif

