QT       += core gui
QT += widgets opengl openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
include($$PWD/pcl1.12.12ReleaseOpencvhalcon.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    interfaces.cpp \
    main.cpp \
    mainwindow.cpp \
    pclwidget.cpp

HEADERS += \
    interfaces.h \
    mainwindow.h \
    pclwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32 {
    CONFIG(release, debug|release) {
        # Release 模式
        LIBS += -L$$PWD/../../../../openCV/opencv/build/x64/vc16/lib \
                -lopencv_world4100      # 改为你的真实 Release 库名
    } else:CONFIG(debug, debug|release) {
        # Debug 模式
        LIBS += -L$$PWD/../../../../openCV/opencv/build/x64/vc16/lib \
                -lopencv_world4100     # 改为你的真实 Debug 库名（带 'd' 后缀）
    }
}


INCLUDEPATH += $$PWD/../../../../openCV/opencv/build/include
DEPENDPATH += $$PWD/../../../../openCV/opencv/build/include
