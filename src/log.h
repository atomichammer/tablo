#ifndef LOG_H
#define LOG_H

#include "ui_log.h"

class Log : public QDialog {

        Q_OBJECT

public:
        Log( QWidget *parent );
        Ui::logDialog ui;
        void setSource(QStringList *_source) { source = _source; }
private:
        QStringList *source;
};
#endif // LOG_H
