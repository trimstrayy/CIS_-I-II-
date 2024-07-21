#ifndef WEATHER_FORECAST_H
#define WEATHER_FORECAST_H
#include <QJsonDocument>

#include <QObject>

class WeatherForecast : public QObject {
    Q_OBJECT
public:
    explicit WeatherForecast(QObject *parent = nullptr);

    Q_INVOKABLE QString getForecast(QString city_name);

    Q_INVOKABLE QJsonDocument response_data(QByteArray);

    Q_INVOKABLE QString get_latitude(QString city_name);
    Q_INVOKABLE QString get_longitude(QString city_name);
};

#endif // MYCLASS_H
