QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT       += sql
QT       += charts
QT       +=network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChartView.cpp \
    Network/SocketClient.cpp \
    QSimpleLed.cpp \
    assessmentform.cpp \
    main.cpp \
    movecalculate.cpp \
    moveform.cpp \
    sqlqueryform.cpp \
    sqlwork/CSQLDatabase.cpp \
    structuredialog.cpp \
    structureform.cpp \
    widget.cpp \
    movewritesql.cpp \
    udpsocketclient.cpp

HEADERS += \
    ChartView.h \
    Network/Constant.h \
    Network/SocketClient.h \
    Network/SocketConstant.h \
    QSimpleLed.h \
    assessmentform.h \
    movecalculate.h \
    moveform.h \
    sqlqueryform.h \
    sqlwork/CSQLDatabase.h \
    structuredialog.h \
    structureform.h \
    viewwork/ChartView.h \
    widget.h \
    movewritesql.h \
    udpsocketclient.h \
    moveconstant.h

FORMS += \
    assessmentform.ui \
    moveform.ui \
    sqlqueryform.ui \
    structuredialog.ui \
    structureform.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
