QT       += core gui qml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    weather_forecast.cpp


HEADERS += \
    weather_forecast.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:/curl/curl-8.9.0_1-win64-mingw/include \
        C:/Qt/6.8.0/include/QtPositioning \
        C:/Qt/6.8.0/include/QtLocation

LIBS += -LC:\curl\curl-8.9.0_1-win64-mingw\lib -lcurl
QT += multimedia



DISTFILES += \
    Newtab.qml \
    WeatherProject.qml \
    Weatherinfo.qml
