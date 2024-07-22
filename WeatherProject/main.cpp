#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<QQmlContext>
#include<QQmlEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;



    engine.load(QUrl(QStringLiteral("../../WeatherProject.qml")));

    return app.exec();
}
