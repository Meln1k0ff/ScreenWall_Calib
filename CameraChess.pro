#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T20:27:00
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = CameraChess
TEMPLATE = app
#opencv
#INCLUDEPATH += "H:/Install/opencv/opencv/build/include"
INCLUDEPATH += "C:/Users/ROOT/Documents/Development/opencv/opencv/build/include"
INCLUDEPATH += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include"

  CONFIG(debug){


LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lstrmiids
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lUuid
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lComCtl32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lComDlg32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOle32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleAcc
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lGdi32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOlePro32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleDlg
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleAut32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lImm32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lmsctfmonitor
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"  -lUser32
#LIBS += -L"G:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/" -lKernel32

LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_core249
LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_highgui249
LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_imgproc249

}

  CONFIG(release){

LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lstrmiids
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lUuid
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lComCtl32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lComDlg32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOle32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleAcc
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lGdi32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOlePro32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleDlg
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lOleAut32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lImm32
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" -lmsctfmonitor
LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib"  -lUser32
#LIBS += -L"G:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/" -lKernel32

LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_core249
LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_highgui249
LIBS += -L"C:\Users\ROOT\Documents\Development\opencv\opencv\build\x86\vc10\lib" -lopencv_imgproc249


}


SOURCES += main.cpp\
        mainwindow.cpp \
    gridwidget.cpp \
    opencv2qt.cpp \
    saveimagedialog.cpp \
    aboutdialog.cpp \
    streamwidget.cpp \
    qt2opencv.cpp \
    draggableitem.cpp \
    graphicsscene.cpp \
    graphicsview.cpp \
    streamthread.cpp \
    capturethread.cpp \
    sharedimagebuffer.cpp

HEADERS  += mainwindow.h \
    gridwidget.h \
    opencv2qt.h \
    saveimagedialog.h \
    aboutdialog.h \
    streamwidget.h \
    qt2opencv.h \
    draggableitem.h \
    graphicsscene.h \
    graphicsview.h \
    streamthread.h \
    capturethread.h \
    sharedimagebuffer.h \
    buffer.h \
    viewport_defines.h

FORMS    += mainwindow.ui \
    gridwidget.ui \
    saveimagedialog.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc
