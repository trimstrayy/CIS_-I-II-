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


INCLUDEPATH += ../../curl/include \
               C:/Coding/software/Qt/6.7.2/msvc2019_arm64/include/QtPositioning \
               C:/Coding/software/Qt/6.7.2/msvc2019_arm64/include/QtLocation

LIBS += -L../../curl/lib -lcurl
QT += multimedia



DISTFILES += \
    Newtab.qml \
    WeatherProject.qml \
    Weatherinfo.qml
