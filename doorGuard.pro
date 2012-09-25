TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    SysSeal.cpp \
    CSerial.cpp \
    guard.cpp \
    timestamp.cpp \
    guardlog.cpp

HEADERS += \
    SysSeal.h \
    CSerial.h \
    guard.h \
    timestamp.h \
    guardlog.h

LIBS += -pthread -lrt

