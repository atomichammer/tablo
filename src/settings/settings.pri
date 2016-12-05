!include( ../../common.pri ) {
    error( Couldnt find the common.pri file! )
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/dialogdetails.h \
    $$PWD/devicemodel.h \
    $$PWD/currencynamesdialog.h \
    $$PWD/configitem.h \
    $$PWD/cbdelegate.h \
    $$PWD/devicecontroller.h \
    $$PWD/currencynamesvalues.h \
    $$PWD/displayeditfilter.h

SOURCES += $$PWD/dialogdetails.cpp \
    $$PWD/devicemodel.cpp \
    $$PWD/currencynamesdialog.cpp \
    $$PWD/cbdelegate.cpp \
    $$PWD/devicecontroller.cpp \
    $$PWD/currencynamesvalues.cpp \
    $$PWD/displayeditfilter.cpp \
    $$PWD/devicesviewfilter.cpp

FORMS += $$PWD/ui/dialogdetails.ui \
    $$PWD/ui/currencynamesdialog.ui \
