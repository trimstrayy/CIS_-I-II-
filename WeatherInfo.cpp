#include "WeatherInfo.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

WeatherInfo::WeatherInfo(QObject *parent)
    : QObject(parent), apiKey("01b70166b923c25c030d53acdc92e93d")
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

WeatherInfo::~WeatherInfo()
{
    curl_global_cleanup();
}

QString WeatherInfo::formattedWeatherText() const
{
    return m_formattedWeatherText;
}

QString WeatherInfo::formattedAQIText() const
{
    return m_formattedAQIText;
}

void WeatherInfo::fetchResult(const QString &city)
{
    if (city.isEmpty()) {
        qDebug() << "Error: Empty city name";
        emit weatherInfoUpdated("Error: Please enter a city name");
        return;
    }

    QString apiUrl = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric").arg(city).arg(apiKey);

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "../../cacert/cacert.pem");

        if(res != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
            emit weatherInfoUpdated("Error fetching weather data");
        } else {
            processWeatherData(readBuffer);
        }
    }
}

void WeatherInfo::processWeatherData(const std::string& data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(data));
    QJsonObject jsonObj = jsonDoc.object();

    QString formattedText;

    if (jsonObj.contains("coord")) {
        QJsonObject coordObj = jsonObj.value("coord").toObject();
        double lat = coordObj.value("lat").toDouble();
        double lon = coordObj.value("lon").toDouble();

        formattedText.append("Coordinates:\n");
        formattedText.append(QString("  Latitude: %1\n").arg(lat));
        formattedText.append(QString("  Longitude: %2\n\n").arg(lon));

        // Fetch AQI data
        fetchAQIData(lat, lon);
    }

    if (jsonObj.contains("main")) {
        QJsonObject mainObj = jsonObj.value("main").toObject();
        double temp = mainObj.value("temp").toDouble();
        int humidity = mainObj.value("humidity").toInt();
        int pressure = mainObj.value("pressure").toInt();

        formattedText.append("Main Weather Data:\n");
        formattedText.append(QString("  Temperature: %1°C\n").arg(temp));
        formattedText.append(QString("  Humidity: %1%\n").arg(humidity));
        formattedText.append(QString("  Pressure: %1 hPa\n\n").arg(pressure));
    }

    if (jsonObj.contains("weather")) {
        QJsonArray weatherArray = jsonObj.value("weather").toArray();
        if (!weatherArray.isEmpty()) {
            QJsonObject weatherObj = weatherArray.first().toObject();
            QString description = weatherObj.value("description").toString();
            QString main = weatherObj.value("main").toString();

            formattedText.append("Weather Conditions:\n");
            formattedText.append(QString("  Main: %1\n").arg(main));
            formattedText.append(QString("  Description: %1\n\n").arg(description));
        }
    }

    if (jsonObj.contains("wind")) {
        QJsonObject windObj = jsonObj.value("wind").toObject();
        double speed = windObj.value("speed").toDouble();

        formattedText.append("Wind Data:\n");
        formattedText.append(QString("  Speed: %1 m/s\n").arg(speed));
    }

    m_formattedWeatherText = formattedText;
    emit weatherTextChanged();
    emit weatherInfoUpdated(m_formattedWeatherText);
}

void WeatherInfo::fetchAQIData(double lat, double lon)
{
    QString apiUrl = QString("http://api.openweathermap.org/data/2.5/air_pollution?lat=%1&lon=%2&appid=%3")
                         .arg(lat).arg(lon).arg(apiKey);

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "../../cacert/cacert.pem");

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed for AQI:" << curl_easy_strerror(res);
        } else {
            processAQIData(readBuffer);
        }
    }
}

void WeatherInfo::processAQIData(const std::string& data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromStdString(data));
    QJsonObject jsonObj = jsonDoc.object();

    QString formattedText;

    if (jsonObj.contains("list")) {
        QJsonArray listArray = jsonObj.value("list").toArray();
        if (!listArray.isEmpty()) {
            QJsonObject listObj = listArray.first().toObject();

            if (listObj.contains("main")) {
                QJsonObject mainObj = listObj.value("main").toObject();
                int aqi = mainObj.value("aqi").toInt();
                formattedText.append(QString("Air Quality Index (AQI): %1\n").arg(aqi));
            }

            if (listObj.contains("components")) {
                QJsonObject componentsObj = listObj.value("components").toObject();
                formattedText.append("Pollutants (μg/m³):\n");
                formattedText.append(QString("  CO: %1\n").arg(componentsObj.value("co").toDouble()));
                formattedText.append(QString("  NO2: %1\n").arg(componentsObj.value("no2").toDouble()));
                formattedText.append(QString("  O3: %1\n").arg(componentsObj.value("o3").toDouble()));
                formattedText.append(QString("  PM2.5: %1\n").arg(componentsObj.value("pm2_5").toDouble()));
                formattedText.append(QString("  PM10: %1\n").arg(componentsObj.value("pm10").toDouble()));
            }
        }
    }

    m_formattedAQIText = formattedText;
    emit aqiTextChanged();
    emit weatherInfoUpdated(m_formattedWeatherText + "\n\n" + m_formattedAQIText);
}
