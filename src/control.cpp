#include <QDir>
#include <QGridLayout>
#include <QPushButton>

#include "control.h"

ControlDialog::ControlDialog(MainWindow *parent)
			:QDialog(parent)
{
	createLines();
	setModal(true);
	setSizeGripEnabled(false);
	setFixedSize(minimumSizeHint());
	setWindowTitle(tr("Control"));
	connect(this, SIGNAL(accepted()), this, SLOT(accept()));
}

ControlDialog::~ControlDialog()
{
	delete[] labels;
	delete[] pbOn;
	delete[] pbOff;
	delete[] sbAddress;
}

void ControlDialog::createLines()
{
	QSettings *settings = new QSettings("main.ini", QSettings::IniFormat);
	
	int rows = settings->beginReadArray("lines");

	if (rows == 0)
		rows = 1;

	labels = new QLabel[rows];
	pbOn = new QPushButton[rows];
	pbOff = new QPushButton[rows];

	for (int i = 0; i < rows; i++) {
		labels[i].setText(QString("%1").arg(i+1));
		pbOn[i].setText(tr("On"));
		pbOff[i].setText(tr("Off"));
	}	
	
	sbAddress = new QSpinBox[rows];
	
	for (int i = 0; i < rows; i++) {
		settings->setArrayIndex(i);
		sbAddress[i].setValue(settings->value("value", 0).toInt());
	}
	
	settings->endArray();

	delete settings;
	
	QGridLayout *layout = new QGridLayout;
	QLabel *lbAddress = new QLabel("Address");
	layout->addWidget(lbAddress, 0, 1);
	
	for(int i = 0; i < rows; i++){
		layout->addWidget(&labels[i], i+1, 0);
		layout->addWidget(&sbAddress[i], i+1, 1);
		layout->addWidget(&pbOn[i], i+1, 2);
		layout->addWidget(&pbOff[i], i+1, 3);
	}
	
	setLayout(layout);
}
