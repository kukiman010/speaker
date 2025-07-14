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
        mainwindow.cpp

HEADERS += \
        bookconvector.h \
        mainwindow.h

FORMS += \
        mainwindow.ui





win32 {
        # message($$PWD/../libs/mupdf/include)
        # message(L$$PWD/../libs/mupdf/win_x64/Release)
        INCLUDEPATH += D:\programms\vcpkg\installed\x64-windows\include
        # LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib tesseract55.lib
        LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib libmupdf.lib jpeg.lib openjp2.lib tesseract55.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib
        # LIBS += -LD:/programms/vcpkg/installed/x64-windows/lib jpeg.lib openjp2.lib tesseract55.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib

        # INCLUDEPATH += $$PWD/../../../../cpp/mupdf/include
        # LIBS += -L$$PWD/../../../../cpp/mupdf\platform\win32\x64\Debug libmupdf.lib jpeg.lib openjp2.lib zlib.lib jbig2dec.lib freetype.lib harfbuzz.lib gumbo.lib

        # OBJECTS += D:/users/qt_projects/mupdf/platform/win32/x64/Release/libresources/CharisSIL_cff.obj

        # Автоматически получаем список всех obj-файлов из папки libresources:
        OBJECTS += $$files(D:/users/qt_projects/mupdf/platform/win32/x64/Release/libresources/*.obj)
        # OBJECTS += $$files(D:/users/qt_projects/temp/*.obj)
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





