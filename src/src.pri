! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/mylineedit.h \
    $$PWD/settingsdialog.h \
    $$PWD/log.h \
    $$PWD/mainform.h \
    $$PWD/settingsstorage.h \
    $$PWD/datasender.h \
    $$PWD/logcontainer.h \
    $$PWD/maskdelegate.h \
    $$PWD/settings/devicesviewfilter.h \
    $$PWD/logmodel.h \
    $$PWD/settingsstorage_sql.h \
    $$PWD/actionbutton.h


SOURCES += $$PWD/main.cpp \
    $$PWD/mylineedit.cpp \
    $$PWD/settingsdialog.cpp \
    $$PWD/log.cpp \
    $$PWD/mainform.cpp \
    $$PWD/settingsstorage.cpp \
    $$PWD/datasender.cpp \
    $$PWD/logcontainer.cpp \
    $$PWD/maskdelegate.cpp \
    $$PWD/logmodel.cpp \
    $$PWD/settingsstorage_sql.cpp \
    $$PWD/actionbutton.cpp

FORMS += $$PWD/ui/log.ui \
    $$PWD/ui/settings.ui \
    $$PWD/ui/mainform.ui
