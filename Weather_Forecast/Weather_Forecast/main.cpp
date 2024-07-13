#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <curl/curl.h>
#include <string>
#include <cstring>
#include <QDateTime>
#include <QTimeZone>


void forecast_date(QJsonDocument);
QByteArray response(char []);           // Function to Call API
void sun_time(QJsonDocument);           // Function to Extract Sunrise and SunSet From Current Weather API
void temperature(QJsonDocument);        // Function to Extract Temperature From Current Weather API
void weather(QJsonDocument);            // Function to Extract Weather Informatrion From Current Weather API
QString lat(QJsonDocument);             // Function to Extract Latitude
QString lon(QJsonDocument);             // Function to Extract Longitude



// Function to write curl data into a QByteArray
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((QByteArray*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize libcurl

    QString API_KEY = "c75997502cfcd1b939260acf6e491ec4";

    char url[150] = "http://api.openweathermap.org/geo/1.0/direct?q=Kathmandu&limit=5&appid=c75997502cfcd1b939260acf6e491ec4";

    QByteArray responseData = response(url);            //Fetching the Data

    // Check if data was fetched
    if(responseData.isEmpty()) {
        qDebug() << "No data fetched.";
        return -1;
    }

    // Trim any leading/trailing whitespace
    responseData = responseData.trimmed();

    QJsonParseError error;

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    // Getting Latitude and Longitude
    QString latitude = lat(jsonDoc);
    QString longitude = lon(jsonDoc);


    // ------------------------------------------------------------------------------
    // Current Weather API CALL
    std::string string_weather_url = "https://api.openweathermap.org/data/2.5/weather?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid=c75997502cfcd1b939260acf6e491ec4";
    char char_weather_url[150];

    // Converting std::string to char type
    strcpy(char_weather_url, string_weather_url.c_str());

    // Function Call
    QByteArray responsedata = response(char_weather_url);

    // Trim any leading/trailing whitespace
    responsedata = responsedata.trimmed();

    // Parsing the JSON Data
    QJsonDocument jsonDoc1 = QJsonDocument::fromJson(responsedata, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    // Function Call
    weather(jsonDoc1);
    temperature(jsonDoc1);
    sun_time(jsonDoc1);


    // ------------------------------------------------------------------------------------------------
    // Call 5 day / 3 hour forecast data
    std::string string_forecast_5Days_3hr_url = "api.openweathermap.org/data/2.5/forecast?lat="+latitude.toStdString()+"&lon="+longitude.toStdString()+"&appid=c75997502cfcd1b939260acf6e491ec4";
    char char_forecast_5Days_3hr_url[150];

    // Converting std::string to char type
    strcpy(char_forecast_5Days_3hr_url, string_forecast_5Days_3hr_url.c_str());

    // Function Call
    QByteArray forecast_response_data = response(char_forecast_5Days_3hr_url);

    // Triming any leading/trailing whitespace
    forecast_response_data = forecast_response_data.trimmed();

    // Parsing the JSON Data
    QJsonDocument forecast_jsonDoc = QJsonDocument::fromJson(forecast_response_data, &error);

    // Error Testing
    if (error.error != QJsonParseError::NoError){
        qWarning() << "Error parsing JSON:" << error.errorString();
        return -1;
    }

    //Function Call
    forecast_date(forecast_jsonDoc);

    // Displaying Widget
    MainWindow w;
    w.show();

    return a.exec();
}


void forecast_date(QJsonDocument jsonDoc)           //Call 5 day / 3 hour forecast data
{
    if(!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
    }

    qDebug() << "\n";
    QJsonObject jsonObj = jsonDoc.object();

    QJsonArray forecast = jsonObj["list"].toArray();

    for(int i = 0; i < 40; i++)
    {
        qDebug() << "Day" << (i*3/24+1);
        qDebug() << forecast[i] << "\n";
    }
}


void sun_time(QJsonDocument jsonDoc)
{
    if(!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject sun = jsonObj["sys"].toObject();
    qDebug() << sun;

    int sunrise_unix = sun["sunrise"].toInteger();
    qDebug() << "Sunrise in UNIX Code: " << sunrise_unix << "\n";

    int sunset_unix = sun["sunset"].toInteger();
    qDebug() << "Sunset in UNIX Code: " << sunset_unix << "\n";

    //Converting UNIX Code to real time/date
    QDateTime sunrise = QDateTime::fromSecsSinceEpoch(sunrise_unix);
    QDateTime sunset = QDateTime::fromSecsSinceEpoch(sunset_unix);

    //Seting the time zone
    QString time_zone_country = sun["country"].toString();      // Fetching the Country Code from sys

    int TimeZone_unix = jsonObj["timezone"].toInteger();        // Fetching timezone from current weather data

    // Set the time zone
    QTimeZone timeZone(TimeZone_unix);

    sunrise.setTimeZone(timeZone);
    sunset.setTimeZone(timeZone);

    // Format and Displaying the real time
    QString sunriseString = sunrise.toString("yyyy-MM-dd HH:mm:ss");
    QString sunsetString = sunset.toString("yyyy-MM-dd HH:mm:ss");

    qDebug() << "Sunrise (" << time_zone_country << ") :" << sunriseString;
    qDebug() << "Sunset (" << time_zone_country << ") :" << sunsetString;
}


void temperature(QJsonDocument jsonDoc) //get's temperature info
{
    // Checking for Error
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject temp = jsonObj["main"].toObject();
    qDebug() << temp;

    double temperature = temp["temp"].toDouble();
    qDebug() << "Temperature: " << temperature << "\n";
}


void weather(QJsonDocument jsonDoc) // get's weather info
{
    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not an object.";
        return;
    }

    qDebug()<<jsonDoc<<"\n";
    QJsonObject jsonObj = jsonDoc.object();

    // Exctracting the Weather:
    QJsonArray Weather = jsonObj["weather"].toArray();

    qDebug()<<Weather;

    QJsonObject weather_data = Weather[0].toObject();

    // Extracting specific information:
    QString description = weather_data["description"].toString();
    qDebug() << "Description: " << description;

    QString main_weather = weather_data["main"].toString();
    qDebug() << "Main: " << main_weather << "\n";

    return;
}


QString lat(QJsonDocument jsonDoc) // get's latitude
{
    QString result;

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        //Parsing the JSON to Object
        QJsonObject obj = jsonArray[0].toObject();

        if(obj.contains("lat")) //finding the latitude of the given city name
        {
            QJsonValue latvalue = obj.value("lat");
            double lat = latvalue.toDouble();
            qDebug() << "lat:" << QString::number(lat, 'f', 8);
            result =  QString::number(lat,'f',8);
            qDebug() << result;
        }

        qDebug() << "\n";
    }
    else {
        qDebug() << "Failed to create JSON doc.";
    }

    return result;
}


QString lon(QJsonDocument jsonDoc) // get's longitude
{
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
            qDebug() << "lon:" << QString::number(lon, 'f', 8); // Taking the precision
            result = QString::number(lon, 'f', 8);
            qDebug() << result;
        }

        qDebug()<<"\n";
    }
    else
    {
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
