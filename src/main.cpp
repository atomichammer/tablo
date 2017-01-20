#include <QtWidgets/QApplication>
#include <QDebug>
#include "mainform.h"


int main(int argc, char *argv[])
{
  
	QApplication app(argc, argv);

    QString fontPath = ":DejaVuSans.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1)
    {
        QFont font("MyFont");
        font.setPointSize(8);
        app.setFont(font);

    }

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
						QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);
	
	QTranslator myappTranslator;
    myappTranslator.load("translations/tablo_" + QLocale::system().name());
	app.installTranslator(&myappTranslator);
    MainForm mainform;
    mainform.show();
	
	return app.exec();
} 
