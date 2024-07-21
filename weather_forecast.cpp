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

QByteArray response(char []);
WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent) {}

// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


// Test
QString WeatherForecast::getForecast(QString city_name)
{
    return city_name;
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


//Fucntions
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
