#ifndef DIALOGDEVCONFIG_H
#define DIALOGDEVCONFIG_H

#include <QDialog>

namespace Ui {
class DialogDevConfig;
}

class DialogDevConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogDevConfig(QWidget *parent = 0);
    ~DialogDevConfig();
    
private:
    Ui::DialogDevConfig *ui;
};

#endif // DIALOGDEVCONFIG_H
