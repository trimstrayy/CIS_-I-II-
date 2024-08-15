#include "weather_forecast.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <curl/curl.h>
#include <QDateTime>
#include <QTimeZone>
#include <string.h>
#include <cstring>


extern QString API_KEY = "01b70166b923c25c030d53acdc92e93d";
extern QJsonParseError error;


QByteArray response(char []);
WeatherInfo::WeatherInfo(QObject *parent) : QObject(parent) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

WeatherInfo::~WeatherInfo() {
    curl_global_cleanup();
}

// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

QByteArray WeatherInfo::response(const char url[]) // Fetching the API
{
    CURL* curl;
    CURLcode res;
    QByteArray responseData;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "../../carcet/cacert.pem");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        }
        curl_easy_cleanup(curl);
    } else {
        qDebug() << "Failed to initialize curl.";
    }

    return responseData;
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
    qDebug() << "Fetching weather for city:" << city;
    if (city.isEmpty()) {
        qDebug() << "Error: Empty city name";
        emit weatherInfoUpdated("Error: Please enter a city name");
        return;
    }

    QString apiUrl = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric").arg(city).arg(API_KEY);

    QByteArray responseData = response(apiUrl.toStdString().c_str());
    if (responseData.isEmpty()) {
        qDebug() << "Failed to fetch weather data.";
        emit weatherInfoUpdated("Error fetching weather data");
    } else {
        processWeatherData(responseData.toStdString());
    }
}

void WeatherInfo::processWeatherData(const std::string& data)
{
    qDebug() << "Processing weather data...";
     qDebug() << "Raw weather data:" << QString::fromStdString(data);
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

        fetchAQIData(lat, lon);
    }

    if (jsonObj.contains("main")) {
        QJsonObject mainObj = jsonObj.value("main").toObject();
        double temp = mainObj.value("temp").toDouble();
         double feelsLike = mainObj.value("feels_like").toDouble();
        double tempMax = mainObj.value("temp_max").toDouble();
        double tempMin = mainObj.value("temp_min").toDouble();
        int humidity = mainObj.value("humidity").toInt();
        int pressure = mainObj.value("pressure").toInt();
        int grndLevel = mainObj.value("grnd_level").toInt();

        formattedText.append("Main Weather Data:\n");
        formattedText.append(QString("  Temperature: %1°C\n").arg(temp));
        formattedText.append(QString("  Feels Like: %1°C\n").arg(feelsLike ));
        formattedText.append(QString("  Humidity: %1%\n").arg(humidity));
        formattedText.append(QString("  Pressure: %1 hPa\n").arg(pressure));
        formattedText.append(QString("  Max Temperature: %1°C\n").arg(tempMax));
        formattedText.append(QString("  Min Temperature: %1°C\n").arg(tempMin));
        formattedText.append(QString("  Ground Level Pressure: %1 mmHg\n").arg(grndLevel));
    }

    if (jsonObj.contains("name") && jsonObj.contains("sys")) {
        QString city = jsonObj.value("name").toString();
        QJsonObject sysObj = jsonObj.value("sys").toObject();
        QString country = sysObj.value("country").toString();
        qint64 sunrise = sysObj.value("sunrise").toVariant().toLongLong();
        qint64 sunset = sysObj.value("sunset").toVariant().toLongLong();
        QDateTime sunriseTime;
        QDateTime sunsetTime;
        sunriseTime.setSecsSinceEpoch(sunrise);
        sunsetTime.setSecsSinceEpoch(sunset);

        formattedText.append(QString("City: %1\n").arg(city));
        formattedText.append(QString("Country: %1\n").arg(country));
        formattedText.append(QString("Sunrise: %1\n").arg(sunriseTime.toString("yyyy-MM-dd HH:mm:ss")));
        formattedText.append(QString("Sunset: %1\n\n").arg(sunsetTime.toString("yyyy-MM-dd HH:mm:ss")));
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

    // Emit the signal after processing is complete
     emitCombinedData();
}

void WeatherInfo::fetchAQIData(double lat, double lon)
{
    QString apiUrl = QString("https://api.openweathermap.org/data/2.5/air_pollution?lat=%1&lon=%2&appid=%3")
                         .arg(lat).arg(lon).arg(API_KEY);

    QByteArray responseData = response(apiUrl.toStdString().c_str());
    if (!responseData.isEmpty()) {
        processAQIData(responseData.toStdString());
    }
}

void WeatherInfo::processAQIData(const std::string& data)
{
     qDebug() << "Processing AQI data...";
    qDebug() << "Raw AQI data:" << QString::fromStdString(data);
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
  qDebug() << "Emitting weatherInfoUpdated signal"; // Debugging line
    // Emit the signal after processing AQI data
  qDebug() << "Formatted AQI text:" << m_formattedAQIText;
    emitCombinedData();
}

void WeatherInfo::emitCombinedData()
{
    QString combinedText = m_formattedWeatherText + "\n" + m_formattedAQIText;
    emit weatherInfoUpdated(combinedText);
}




// WeatherForecast implementation
WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent) {

}

// Function to write curl data into a QByteArray
/*size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
*/

WeatherForecast::~WeatherForecast() {
    curl_global_cleanup();
}


// Functions of Fetching the Weather Metrics Data

QString WeatherForecast::getCity(QString city_name)
{
    store_city_name = city_name;
    return city_name;
}


QString WeatherForecast::get_date_time(QJsonObject jsonObj)
{
    double datetime = jsonObj["dt"].toDouble();
    QJsonObject sys = jsonObj["sys"].toObject();
    QString time_zone_country = sys["country"].toString();      // Fetching the Country Code from sys
    int timezone = jsonObj["timezone"].toInteger();
    QDateTime time = QDateTime::fromSecsSinceEpoch(datetime);
    QTimeZone timeZone(timezone);
    time.setTimeZone(timeZone);
    QString datetimeString = time.toString("yyyy-MM-dd");
    qDebug() << datetimeString;
    return datetimeString;
}


QString WeatherForecast::get_visibility_data(QJsonObject jsonObj)
{
    double visibility = jsonObj["visibility"].toDouble();
    visibility /= 1000;
    QString result = QString::number(visibility, 'f', 2);
    return result;
}


QString WeatherForecast::get_rain_data(QJsonObject jsonObj)
{
    QJsonObject rain = jsonObj["rain"].toObject();
    double h = rain["1h"].toDouble();
    QString result = QString::number(h, 'f', 2);
    return result;
}


QString WeatherForecast::get_wind_data(QJsonObject jsonObj)
{
    QJsonObject wind = jsonObj["wind"].toObject();
    double speed = wind["speed"].toDouble();
    QString result = QString::number(speed, 'f', 2);
    return result;
}


QString WeatherForecast::get_pressure_data(QJsonObject jsonObj)
{
    QString result;
    QJsonObject main = jsonObj["main"].toObject();
    double humidity = main["humidity"].toDouble();
    result = QString::number(humidity, 'f', 1);
    return result;
}


QString WeatherForecast::get_cloudiness_data(QJsonObject jsonObj)
{
    QJsonObject clouds = jsonObj["clouds"].toObject();
    double all = clouds["all"].toDouble();
    QString result = QString::number(all, 'f', 1);
    return result;
}


QString WeatherForecast::get_humidity_data(QJsonObject jsonObj)
{
    QString result;
    QJsonObject main = jsonObj["main"].toObject();
    double humidity = main["humidity"].toDouble();
    result = QString::number(humidity, 'f', 1);
    return result;
}


void WeatherForecast::get_current_weather(QString latitude, QString longitude)
{
    // Current Weather API CALL
    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_weather_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result;

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QString humidity = "";
    QString cloudiness = "";
    QString pressure = "";
    QString wind = "";
    QString visibility = "";
    QString rain = "";
    QString datetime = "";
    humidity = get_humidity_data(jsonObj);
    qDebug() << humidity;
    cloudiness = get_cloudiness_data(jsonObj);
    qDebug() << cloudiness;
    pressure = get_pressure_data(jsonObj);
    qDebug() << pressure;
    wind = get_wind_data(jsonObj);
    qDebug() << wind;
    rain = get_rain_data(jsonObj);
    qDebug() << rain;
    visibility = get_visibility_data(jsonObj);
    qDebug() << visibility;
    datetime = get_date_time(jsonObj);
    qDebug() << datetime;

    emit humidityData(humidity);
    emit cloudinessData(cloudiness);
    emit pressureData(pressure);
    emit windData(wind);
    emit rainData(rain);
    emit visibilityData(visibility);
    emit dateTimeData(datetime);

}


//Fucntions of fetching Hourly Forecast Data
QString WeatherForecast::get_icon_hourly_data(QJsonArray forecast, int index)
{
    QString icon;
    QJsonObject arr = forecast[index].toObject();
    QJsonArray weather = arr["weather"].toArray();
    QJsonObject weather_data = weather[0].toObject();
    icon = weather_data["icon"].toString();
    return icon;
}


QString WeatherForecast::get_temperature_hourly_data(QJsonArray forecast ,int index)
{
    double temperature;
    QJsonObject arr = forecast[index].toObject();
    QJsonObject main = arr["main"].toObject();
    temperature = main["temp"].toDouble();
    temperature -= 273;
    QString result = QString::number(temperature, 'f', 1);
    return result;
}

void WeatherForecast::get_temperature_hourly(QString latitude, QString longitude,int index)
{
    //Call hourly forecast data
    std::string string_forecast_4Days_hr_url = "https://pro.openweathermap.org/data/2.5/forecast/hourly?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid="+API_KEY.toStdString();
    char url[150];

    //Converting std::string to char type
    strcpy(url, string_forecast_4Days_hr_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
    }

    QJsonObject jsonObj = jsonDoc.object();

    // QJsonArray forecast = jsonObj["list"].toArray();
    QJsonArray forecast = jsonObj["list"].toArray();

    for(int i = 0 ; i < index ; i++){
    QString temperature = "";
    QString icon = "";
    temperature = get_temperature_hourly_data(forecast, i);
    icon = get_icon_hourly_data(forecast, i);
    qDebug() << temperature << "\n";
    qDebug() << icon << "\n";

    emit temperatureHourlyData(temperature, i);
    emit iconhourlyData(icon, index);
    }
}


QString WeatherForecast::get_icon(QString latitude, QString longitude)
{
    // Current Weather API CALL
    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_weather_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result;

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return "error";
    }

    QJsonObject jsonObj = jsonDoc.object();

    // Exctracting the Weather:
    QJsonArray Weather = jsonObj["weather"].toArray();

    QJsonObject weather_data = Weather[0].toObject();

    // Extracting specific information:
    QString icon = weather_data["icon"].toString();
    qDebug() << icon << "\n";

    int id = weather_data["id"].toInt();

    std::string icon_string_url = "https://openweathermap.org/img/wn/"+icon.toStdString()+"@2x.png";
    QString icon_url = QString::fromStdString(icon_string_url);

    qDebug() << id ;
    return icon_url;
}


QString WeatherForecast::get_temperature(QString latitude, QString longitude)
{
    // Current Weather API CALL
    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_weather_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result;

    // Checking for Error
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return "error";
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject temp = jsonObj["main"].toObject();

    // Extracting specific information:
    double temperature = temp["temp"].toDouble() - 273;
    result = QString::number(temperature, 'f', 1);

    return result;
}


QString WeatherForecast::get_weather(QString latitude, QString longitude)
{
    // Current Weather API CALL
    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_weather_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result;

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return "error";
    }

    QJsonObject jsonObj = jsonDoc.object();

    // Exctracting the Weather:
    QJsonArray Weather = jsonObj["weather"].toArray();

    QJsonObject weather_data = Weather[0].toObject();

    // Extracting specific information:
    QString description = weather_data["description"].toString();
    // qDebug() << "Description: " << description;

    QString main_weather = weather_data["main"].toString();
    // qDebug() << "Main: " << main_weather << "\n";

    return description;
}



QString WeatherForecast::get_longitude(QString city_name)
{
    // Initialize libcurl
    qDebug() << city_name;
    std::string string_url = "http://api.openweathermap.org/geo/1.0/direct?q="+city_name.toStdString()+"&limit=5&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result = "error";  // Default to "error"

    if (!jsonDoc.isEmpty() && jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        if (!jsonArray.isEmpty()) {
            QJsonObject obj = jsonArray[0].toObject();

            if (obj.contains("lon")) {
                QJsonValue lonvalue = obj.value("lon");
                double lon = lonvalue.toDouble();
                result = QString::number(lon, 'f', 8);
            }
        }
    } else {
        qDebug() << "Empty or invalid JSON doc.";
    }


    return result;

}


QString WeatherForecast::get_latitude(QString city_name)        // get's latitude
{
    // Initialize libcurl
    std::string string_url = "http://api.openweathermap.org/geo/1.0/direct?q="+city_name.toStdString()+"&limit=5&appid="+API_KEY.toStdString();
    char url[150];

    // Converting std::string to char type
    strcpy(url, string_url.c_str());

    //fetching data
    QJsonDocument jsonDoc = response_data(response(url));

    QString result = "error";  // Default to "error"

    if (!jsonDoc.isEmpty() && jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        if (!jsonArray.isEmpty()) {
            QJsonObject obj = jsonArray[0].toObject();

            if (obj.contains("lat")) {
                QJsonValue latvalue = obj.value("lat");
                double lat = latvalue.toDouble();
                result = QString::number(lat, 'f', 8);
            }
        }
    } else {
        qDebug() << "Empty or invalid JSON doc.";
    }

    return result;
}


//Fetching , Trimming and Converting into JSONDOC form
QJsonDocument WeatherForecast::response_data(QByteArray responseData)
{
    // Trim any leading/trailing whitespace
    responseData = responseData.trimmed();

    QJsonParseError error;

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &error);

    return jsonDoc;
}


QByteArray response(char url[]) //Fetching the API
{
    CURL* curl;
    CURLcode res;
    QByteArray responseData;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Replace  with a known working URL
        curl_easy_setopt(curl, CURLOPT_URL, url);                       //sets the URL to fetch
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);   //handle the data received from the request
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Provide a CA certificate bundle
        curl_easy_setopt(curl, CURLOPT_CAINFO, "../../carcet/cacert.pem");

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) // error checking
            qDebug() << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        else
            qDebug() << "Data fetched successfully.";

        curl_easy_cleanup(curl);
    }
    else {
        qDebug() << "Failed to initialize curl."; //error checking
    }

    curl_global_cleanup();
    return responseData;
}
