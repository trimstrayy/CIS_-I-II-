#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include<QQmlEngine>
#include "weather_forecast.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    WeatherForecast weatherForecast;

    engine.rootContext()->setContextProperty("weatherForecast", &weatherForecast);
    engine.load(QUrl(QStringLiteral("../../WeatherProject.qml")));

    return app.exec();
}
