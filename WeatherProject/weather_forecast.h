#ifndef WEATHER_FORECAST_H
#define WEATHER_FORECAST_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QObject>

class WeatherForecast : public QObject {
    Q_OBJECT
public:
    explicit WeatherForecast(QObject *parent = nullptr);
    QString store_city_name;

    Q_INVOKABLE QString getCity(QString city_name);

    Q_INVOKABLE QJsonDocument response_data(QByteArray);

    Q_INVOKABLE QString get_latitude(QString city_name);
    Q_INVOKABLE QString get_longitude(QString city_name);
    Q_INVOKABLE QString get_weather(QString latitude, QString longitude);
    Q_INVOKABLE QString get_temperature(QString latitude, QString longitude);
    Q_INVOKABLE QString get_icon(QString latitude, QString longitude);
    Q_INVOKABLE void get_current_weather(QString latitude, QString longitude);
    Q_INVOKABLE QString get_humidity_data(QJsonObject jsonObj);
    Q_INVOKABLE QString get_cloudiness_data(QJsonObject jsonObj);
    Q_INVOKABLE QString get_pressure_data(QJsonObject jsonObj);
    Q_INVOKABLE QString get_wind_data(QJsonObject jsonObj);
    Q_INVOKABLE QString get_rain_data(QJsonObject jsonObj);
    Q_INVOKABLE QString get_visibility_data(QJsonObject jsonObj);
    Q_INVOKABLE void get_temperature_hourly(QString latitude, QString longitude, int index);
    Q_INVOKABLE QString get_temperature_hourly_data(QJsonArray forecast, int index);
    Q_INVOKABLE QString get_icon_hourly_data(QJsonArray forecast, int index);
    Q_INVOKABLE QString get_date_hourly_data(QJsonArray forecast, int index);
signals:
    void temperatureHourlyData(const QString &temperature, int index);
    void iconhourlyData(const QString &icon, int index);
    void datehourlyData(const QString &date, int index);
    void humidityData(const QString humidity);
    void cloudinessData(const QString clouds);
    void pressureData(const QString pressure);
    void windData(const QString wind);
    void rainData(const QString rain);
    void visibilityData(const QString rain);

// private:
//     double m_temperature;
//     QJsonArray m_forecast;
};

#endif // MYCLASS_H
