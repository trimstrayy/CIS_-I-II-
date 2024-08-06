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
WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent) {

}

// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


//Main function
// QString WeatherForecast::main(QString latitude, QString longitude)


// Test
QString WeatherForecast::getCity(QString city_name)
{
    store_city_name = city_name;
    return city_name;
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

    emit humidityData(humidity);
    emit cloudinessData(cloudiness);
    emit pressureData(pressure);
    emit windData(wind);
    emit rainData(rain);
    emit visibilityData(visibility);

}


//Fucntions
QString WeatherForecast::get_date_hourly_data(QJsonArray forecast, int index)
{
    return "";
}

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
    QString dt;
    temperature = get_temperature_hourly_data(forecast, i);
    icon = get_icon_hourly_data(forecast, i);
    dt = get_date_hourly_data(forecast, i);
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
    QString id_s;
    if(id > 200 && id < 300)
    {
        id_s = "2xx";
    }
    else if(id > 300 && id < 400)
    {
        id_s = "3xx";
    }
    else if(id > 500 && id < 600)
    {
        id_s = "5xx";
    }
    else if(id > 600 && id < 700)
    {
        id_s = "6xx";
    }
    else if(id > 700 && id < 800)
    {
        id_s = "7xx";
    }
    else if(id > 800 && id < 805)
    {
        id_s = "8x";
    }
    else if(id == 800)
    {
        id_s = "800";
    }

    qDebug() << id_s;

    std::string icon_string_url = "https://openweathermap.org/img/wn/"+icon.toStdString()+"@2x.png";
    QString icon_url = QString::fromStdString(icon_string_url);
    qDebug() << id ;
    // QString icon_url = "/Coding/c++/git/desing/I-II-Project-/Project Pic src/cloudy.png";
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

    QString result;

    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();

        if(obj.contains("lon")) //finding the latitude of the given city name
        {
            QJsonValue lonvalue = obj.value("lon");
            double lon = lonvalue.toDouble();
            result = QString::number(lon, 'f', 8);
        }
    }
    else
    {
        qDebug() << "Failed to create JSON doc.";
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

    QJsonDocument jsonDoc = response_data(response(url));

    QString result;

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();

        if(obj.contains("lat")) //finding the latitude of the given city name
        {
            QJsonValue latvalue = obj.value("lat");
            double lat = latvalue.toDouble();
            result =  QString::number(lat,'f',8);
        }
    }
    else {
        qDebug() << "Failed to create JSON doc.";
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
