#ifndef WEATHER_H
#define WEATHER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <curl/curl.h>
#include <string>
#include <QObject>

class WeatherInfo : public QObject {
    Q_OBJECT
    //Q_PROPERTY(QString formattedWeatherText READ formattedWeatherText NOTIFY weatherTextChanged)
   // Q_PROPERTY(QString formattedAQIText READ formattedAQIText NOTIFY aqiTextChanged)

public:
    explicit WeatherInfo(QObject *parent = nullptr);
    ~WeatherInfo();
    void emitCombinedData();
    QString formattedWeatherText() const;
    QString formattedAQIText() const;

    Q_INVOKABLE void fetchResult(const QString &city);

signals:
    //void weatherTextChanged();
    //void aqiTextChanged();
    void weatherInfoUpdated(const QString &info);

private:
    void processWeatherData(const std::string& data);
    void processAQIData(const std::string& data);
    void fetchAQIData(double lat, double lon);

    QByteArray response(const char url[]);
    QString m_formattedWeatherText;
    QString m_formattedAQIText;
    QString apiKey;
};

class WeatherForecast : public QObject {
    Q_OBJECT

public:
    explicit WeatherForecast(QObject *parent = nullptr);
    ~WeatherForecast();  // Destructor declaration

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
    Q_INVOKABLE QString get_date_time(QJsonObject jsonObj);
    Q_INVOKABLE void get_temperature_hourly(QString latitude, QString longitude, int index);
    Q_INVOKABLE QString get_temperature_hourly_data(QJsonArray forecast, int index);
    Q_INVOKABLE QString get_icon_hourly_data(QJsonArray forecast, int index);
    Q_INVOKABLE QString get_date_hourly_data(QJsonArray forecast, int index);

    Q_INVOKABLE void fetchResult(const QString &city) {
        m_weatherInfo.fetchResult(city);
    }
signals:
    void temperatureHourlyData(const QString &temperature, int index);
    void iconhourlyData(const QString &icon, int index);
    void datehourlyData(const QString &date, int index);
    void humidityData(const QString humidity);
    void cloudinessData(const QString clouds);
    void pressureData(const QString pressure);
    void windData(const QString wind);
    void rainData(const QString rain);
    void visibilityData(const QString visibility);
    void dateTimeData(const QString datetime);

private:
    WeatherInfo m_weatherInfo;  // Use WeatherInfo as a member
};

#endif // WEATHER_H
