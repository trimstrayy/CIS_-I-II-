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
WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent) {}

// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


// Test
QString WeatherForecast::getCity(QString city_name)
{
    return city_name;
}


//Fucntions
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
        curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/Coding/c++/cacert.pem");

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
