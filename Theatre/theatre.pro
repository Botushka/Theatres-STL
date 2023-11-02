TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        globals.cpp \
        main.cpp \
        theatrecommandprocessor.cpp \
        theatredatareader.cpp

HEADERS += \
    globals.hh \
    theatrecommandprocessor.hh \
    theatredata.hh \
    theatredatareader.hh
