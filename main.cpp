#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include<QQmlEngine>
#include "weather_forecast.h"
#include "WeatherInfo.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    WeatherForecast weatherForecast;
    WeatherInfo weatherInfo;

    engine.rootContext()->setContextProperty("weatherForecast", &weatherForecast);
    engine.rootContext()->setContextProperty("weatherInfo", &weatherInfo);
    engine.load(QUrl(QStringLiteral("../../WeatherProject.qml")));

    return app.exec();
}
