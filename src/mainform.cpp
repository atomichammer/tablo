#include "mainform.h"
#include "settingsdialog.h"
#include "port.h"
#include "log.h"
#include "ui_mainform.h"
#include "settings/displayeditfilter.h"
#include "maskdelegate.h"
#include "logmodel.h"

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainForm)
{
    m_ui->setupUi(this);
    connect(m_ui->pbSend, SIGNAL(clicked()), this, SLOT(sendData()));
    //connect(m_ui->pbSend, SIGNAL(clicked()), this, SLOT(saveSettings()));

    setWindowFlags(windowFlags()^Qt::WindowMaximizeButtonHint);
    settings = SettingsStorage_sql::Instance();
    sender = DataSender::Instance();
    restoreSettings();
    createGridLayout();

    connect(m_ui->actionSettings, SIGNAL(triggered()), this, SLOT(callSettingsDialog()));
    connect(settings->getCurrencyNameValuesModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(autoResize()));
    LogModel *logModel = LogModel::Instance();
    logModel->addEvent(2, "Programm Started");
    //registerUSB();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));
    timer->setInterval(2000);
//    timer->start();
}

MainForm::~MainForm()
{
    delete m_ui;
}

void MainForm::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainForm::createGridLayout()
{
    //creating logo
    if(QFile::exists("logo.png"))
    {
        QPixmap pixmap("logo.png");
        m_ui->logo->setPixmap(pixmap);
    }

    MaskDelegate *delegate = new MaskDelegate(this);
    //connect(m_ui->tableView, SIGNAL(), this, SLOT(sendData()));

    m_ui->tableView->setItemDelegateForColumn(0, delegate);
    m_ui->tableView->setItemDelegateForColumn(1, delegate);

    DisplayEditFilter *filteredModel = new DisplayEditFilter();
    filteredModel->setSourceModel(settings->getCurrencyNameValuesModel());
    connect(filteredModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(autoResize()));

    m_ui->tableView->setModel(filteredModel);
    m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //qApp->setStyleSheet("QHeaderView::section {color: black;padding: 4px;border: 1px solid #6c6c6c;}");

    autoResize();
}

void MainForm::autoResize()
{
    m_ui->tableView->setFixedHeight( m_ui->tableView->rowHeight(0) * (m_ui->tableView->model()->rowCount(QModelIndex()) + 1)-3);
    this->resize(this->minimumSize());
}

void MainForm::callSettingsDialog()
{
    SettingsDialog *settingsDialog = new SettingsDialog(this);
    connect(this, SIGNAL(usbArrived()), settingsDialog, SLOT(EnumComm()));

    if(settingsDialog->exec() == QDialog::Accepted)
    {
//        settings->setDayHour(settingsDialog->getDayHour());
//        settings->setDayMinute(settingsDialog->getDayMinute());
//        settings->setNightHour(settingsDialog->getNightHour());
//        settings->setNightMinute(settingsDialog->getNightMinute());
//        settings->setDayBrightness(settingsDialog->getDayBrightness());
//        settings->setNightBrightness(settingsDialog->getNightBrightness());
//        settings->setTimerEnabled(settingsDialog->isTimerEnabled());
        settings->setMaxTries(settingsDialog->getMaxTries());
        settings->saveDevices();
        delete settingsDialog;
    }
}

void MainForm::sendData()
{
    Log *logWindow = new Log(this);
    logWindow->show();

    m_ui->pbSend->setEnabled(false);
    QApplication::processEvents();
    QByteArray param;
    char result;
    result = sender->SphericalSenderInVacuum(3, param);
    if(result != 0)
    {
        //Sleep(500);
        logWindow->close();
        delete logWindow;
    }

    m_ui->pbSend->setEnabled(true);
}

void MainForm::restoreSettings()
{
    QSettings *settings = new QSettings("main.ini", QSettings::IniFormat);
    settings->beginGroup("MainWindow");
            move(settings->value("pos", QPoint(200, 200)).toPoint());
    settings->endGroup();
    delete settings;
}

void MainForm::closeEvent(QCloseEvent *event)
{
    QSettings *iniSettings = new QSettings("main.ini", QSettings::IniFormat);

    iniSettings->beginGroup("MainWindow");
        iniSettings->setValue("pos", pos());
    iniSettings->endGroup();
    delete iniSettings;

    settings->saveSettings();
    QMainWindow::closeEvent(event);
}

void MainForm::on_actionTurnOff_triggered()
{
    Log *logWindow = new Log(this);
    logWindow->show();
    QByteArray param;
    char result;
    param.append(8);
    result = sender->SphericalSenderInVacuum(1, param);
    if(result != 0)
    {
        //Sleep(500);
        logWindow->close();
        delete logWindow;
    }
}

void MainForm::on_actionDayMode_triggered()
{
    Log *logWindow = new Log(this);
    logWindow->show();
    QByteArray param;
    char result;
    param.append(1);
    result = sender->SphericalSenderInVacuum(1, param);
    if(result != 0)
    {
        //Sleep(500);
        logWindow->close();
        delete logWindow;
    }
}

void MainForm::on_actionSetNight_triggered()
{
    Log *logWindow = new Log(this);
    logWindow->show();
    QByteArray param;
    char result;
    param.append(4);
    result = sender->SphericalSenderInVacuum(1, param);
    if(result != 0)
    {
        //Sleep(500);
        logWindow->close();
        delete logWindow;
    }
}

void MainForm::on_actionLogWindow_triggered()
{
    Log *logWindow = new Log(this);
    logWindow->show();
}
/*
void MainForm::registerUSB()
{
    //Globally Unique Identifier (GUID). Windows uses GUIDs to identify things.
    GUID InterfaceClassGuid = { 0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED };
    //Globally Unique Identifier (GUID) for USB peripheral devices

    //Register for WM_DEVICECHANGE notifications:
    DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;

    ZeroMemory( &MyDeviceBroadcastHeader, sizeof(MyDeviceBroadcastHeader) );
    MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
    MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;

    RegisterDeviceNotification((HWND)this->winId(), &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);
}

bool MainForm::winEvent(MSG *m, long *result)
{
    Q_UNUSED(result)
    if(m->message == WM_POWERBROADCAST)
    {
        //qDebug() << "PowerEvent";
        switch(m->wParam)
        {
            case PBT_APMPOWERSTATUSCHANGE: qDebug() << "WM_APMPOWERSTATUSCHANGE"; break;
            case PBT_APMRESUMEAUTOMATIC: qDebug() << "PBT_APMRESUMEAUTOMATIC"; break;
            case PBT_APMRESUMESUSPEND:  qDebug() << "openPort()"; break;
            case PBT_APMSUSPEND: qDebug() << "closePort()"; break;
            //case PBT_APMPOWERSTATUSCHANGE: qDebug() << "PBT_APMPOWERSTATUSCHANGE"; break;
            default: break;
        }
        return true;
    }

    if(m->message == WM_DEVICECHANGE)
     {
         PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)m->lParam;

         switch(m->wParam)
         {
             case DBT_DEVICEARRIVAL:
             case DBT_DEVICEREMOVEPENDING:
             case DBT_DEVICEREMOVECOMPLETE:
             case DBT_CONFIGCHANGED:
             {
                 if(lpdb -> dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                 {
//                     PDEV_BROADCAST_DEVICEINTERFACE lpdbv = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
//                     QString vidpid = QString::fromStdWString(lpdbv->dbcc_name);
                     emit usbArrived();
                 }
                 break;
            }
         }
         return true;
     }
    return false;
}
*/
