TARGET      = Control
TEMPLATE    = lib
CONFIG      += staticlib

QT = core network

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # abstractcommand.cpp \
    # abstractobject.cpp \
    command.cpp \
    socket.cpp \
    sslworker.cpp


HEADERS += \
    # abstractcommand.h \
    # abstractobject.h \
    command.h \
    saved_container.h \
    socket.h \
    sslworker.h






win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Interface/release/ -lInterface
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Interface/debug/ -lInterface
else:unix: LIBS += -L$$OUT_PWD/../Interface/ -lInterface

INCLUDEPATH += $$PWD/../Interface
DEPENDPATH += $$PWD/../Interface

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Interface/release/libInterface.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Interface/debug/libInterface.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Interface/release/Interface.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Interface/debug/Interface.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Interface/libInterface.a
