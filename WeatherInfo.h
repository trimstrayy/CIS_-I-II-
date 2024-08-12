#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <QObject>
#include <QString>
#include <curl/curl.h>
#include <string>

class WeatherInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString formattedWeatherText READ formattedWeatherText NOTIFY weatherTextChanged)
    Q_PROPERTY(QString formattedAQIText READ formattedAQIText NOTIFY aqiTextChanged)

public:
    explicit WeatherInfo(QObject *parent = nullptr);
    ~WeatherInfo();

    QString formattedWeatherText() const;
    QString formattedAQIText() const;

    Q_INVOKABLE void fetchResult(const QString &city);

signals:
    void weatherTextChanged();
    void aqiTextChanged();
    void weatherInfoUpdated(const QString &info);

private:
    void processWeatherData(const std::string& data);
    void processAQIData(const std::string& data);
    void fetchAQIData(double lat, double lon);

    QString m_formattedWeatherText;
    QString m_formattedAQIText;
    QString apiKey;
};

#endif // WEATHERINFO_H
