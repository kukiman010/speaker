#-------------------------------------------------
#
# Project created by QtCreator 2025-06-30T11:35:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = creator_ebook
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        bookconvector.cpp \
        main.cpp \
        mainwindow.cpp \
        ocrreader.cpp \
        page_main.cpp \
        pdfreader.cpp

HEADERS += \
        bookconvector.h \
        mainwindow.h \
        ocrreader.h \
        page_main.h \
        pdfreader.h

FORMS += \
        mainwindow.ui \
        page_main.ui





win32 {
        # message($$PWD/../libs/mupdf/include)
        # message(L$$PWD/../libs/mupdf/win_x64/Release)
        INCLUDEPATH += D:\programms\vcpkg\installed\x64-windows\include
        LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib tesseract55.lib leptonica-1.85.0.lib
        # LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib libmupdf.lib jpeg.lib openjp2.lib tesseract55.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib
        # LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib jpeg.lib openjp2.lib tesseract55.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib

        INCLUDEPATH += $$PWD/../../../../cpp/mupdf/include
        LIBS += -L$$PWD/../../../../cpp/mupdf\platform\win32\x64\Debug libmupdf.lib jpeg.lib openjp2.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib gumbo.lib
        # cld2.lib

        # Автоматически получаем список всех obj-файлов из папки libresources:
        OBJECTS += $$files($$PWD/../libs/mupdf/win_x64/Release/libresources/*.obj)
        OBJECTS += $$PWD/../libs/mupdf/win_x64/Release/libresources/NotoSansOriya-Regular_otf.obj
}

unix:!macx {
        # message("$$PWD/../libs/tesseract/include")
        # message("-L$$PWD/../libs/tesseract/linux/release ")

        # mupdf
        INCLUDEPATH += $$PWD/../libs/mupdf/include
        LIBS += -L$$PWD/../libs/mupdf/linux/release -lmupdf -lmupdf-third -lz -ljpeg -lopenjp2

        # tesseract
        INCLUDEPATH += $$PWD/../libs/tesseract/include
        LIBS += -L$$PWD/../libs/tesseract/linux/release -ltesseract -llept
}

macx {
        # INCLUDEPATH += $$PWD/../mupdf/include
        # LIBS += -L$$PWD/../mupdf/build-dir/Release -lmupdf
}




# Interface
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


# Control
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Control/release/ -lControl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Control/debug/ -lControl
else:unix: LIBS += -L$$OUT_PWD/../Control/ -lControl

INCLUDEPATH += $$PWD/../Control
DEPENDPATH += $$PWD/../Control

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Control/release/libControl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Control/debug/libControl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Control/release/Control.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Control/debug/Control.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Control/libControl.a


