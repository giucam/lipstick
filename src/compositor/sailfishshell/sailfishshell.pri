INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/sailfishshell.h \
    $$PWD/sailfishsurface.h \

SOURCES += \
    $$PWD/sailfishshell.cpp \
    $$PWD/sailfishsurface.cpp \

WAYLANDSERVERSOURCES += ../protocol/sailfish-shell.xml \

QT += compositor
