#ifndef _control_h_
#define _control_h_

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QString>

#include "mainwin.h"

class ControlDialog : public QDialog 
{
	Q_OBJECT
	
public:
	ControlDialog(MainWindow *parent);
	~ControlDialog();

	
private:	
	QDialogButtonBox *buttonBox;
	void createLines();
	QLabel *labels;
	QSpinBox *sbAddress;
	QPushButton *pbOn;
	QPushButton *pbOff;

};
#endif
