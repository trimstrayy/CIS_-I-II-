QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets #quickwidget - from map

QT += quick location positioning #from map
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    mainwindow.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Add the following lines for libcurl
INCLUDEPATH += C:/Coding/curl/include
LIBS += -LC:/Coding/curl/lib -lcurl

RESOURCES += \
    Resources.qrc
