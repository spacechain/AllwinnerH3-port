#-------------------------------------------------
#
# Project created by QtCreator 2018-03-27T11:29:11
#
#-------------------------------------------------

 QT += core gui widgets

TARGET = QRcTest
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    function.cpp \
    recemoney.cpp \
    pay.cpp \
    tradingrecord.cpp \
    contract.cpp \
    numkeyboard.cpp \
    thread.cpp \
    dialog.cpp


HEADERS  += widget.h \
    function.h \
    recemoney.h \
    pay.h \
    tradingrecord.h \
    contract.h \
    numkeyboard.h \
    thread.h \
    dialog.h

FORMS    += widget.ui \
    function.ui \
    recemoney.ui \
    pay.ui \
    tradingrecord.ui \
    contract.ui \
    numkeyboard.ui \
    dialog.ui


include(QRCode/QRCode.pri)
include(qjson4/qjson4.pri)

RESOURCES += \
    opensoftware.qrc
