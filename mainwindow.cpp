#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QCompleter>
#include <QStringListModel>
#include <QScrollArea>
#include <QFontDatabase>
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);
    apiKey = "c75997502cfcd1b939260acf6e491ec4";

    connect(ui->pushButtonGetAQI, &QPushButton::clicked, this, &MainWindow::fetchresult);
    connect(ui->lineEditCity, &QLineEdit::returnPressed, this, &MainWindow::fetchresult);

    QRegularExpression re("^[A-Z][a-zA-Z ]+$");
    QValidator *validator = new QRegularExpressionValidator(re, this);
    ui->lineEditCity->setValidator(validator);

    QStringList cityNames = {{"Kathmandu", "Pokhara", "Lalitpur", "Bhaktapur", "Okhaldhunga","Biratnagar", "Bharatpur", "Janakpur", "Hetauda", "Dharan","Pyuthan", "Butwal", "Itahari", "Nepalgunj", "Dhangadhi", "Birgunj", "Birendranagar", "Damak", "Dhankuta", "Ilam", "Jumla", "Mahendranagar", "Tansen"}};
    QStringListModel *model = new QStringListModel(cityNames, this);
    QCompleter *completer = new QCompleter(model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditCity->setCompleter(completer);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onCityResult);

    // Create QScrollArea
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(ui->labelDetails);
    ui->verticalLayout->addWidget(scrollArea); // Assuming you have a vertical layout named verticalLayout

    // Custom Styling
    setWindowTitle("Weather & AQI Info");
    setStyleSheet("background-color: #2E3440; color: #D8DEE9; font-family: 'Arial'; font-size: 14px;");

    ui->pushButtonGetAQI->setStyleSheet("background-color: #5E81AC; color: #ECEFF4; padding: 10px; border-radius: 5px;");
    ui->lineEditCity->setStyleSheet("background-color: #3B4252; color: #ECEFF4; padding: 10px; border-radius: 5px;");
    ui->labelDetails->setStyleSheet("background-color: #4C566A; color: #ECEFF4; padding: 10px; border-radius: 5px;");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkManager;
}

void MainWindow::fetchresult()
{
    QString city = ui->lineEditCity->text().trimmed();

    if (city.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    QString apiUrl = "http://api.openweathermap.org/data/2.5/weather";
    QUrl url(apiUrl);
    QUrlQuery query;
    query.addQueryItem("q", city);
    query.addQueryItem("appid", apiKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    networkManager->get(request);

    ui->labelDetails->clear();
}

void MainWindow::onCityResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString formattedText;

        // Parsing and formatting the data
        if (jsonObj.contains("coord")) {
            QJsonObject coordObj = jsonObj.value("coord").toObject();
            double lat = coordObj.value("lat").toDouble();
            double lon = coordObj.value("lon").toDouble();

            formattedText.append("Coordinates:\n");
            formattedText.append(QString("  Latitude: %1\n").arg(lat));
            formattedText.append(QString("  Longitude: %2\n\n").arg(lon));
        }

        if (jsonObj.contains("main")) {
            QJsonObject mainObj = jsonObj.value("main").toObject();
            double feelsLike = mainObj.value("feels_like").toDouble();
            double temp = mainObj.value("temp").toDouble();
            double tempMax = mainObj.value("temp_max").toDouble();
            double tempMin = mainObj.value("temp_min").toDouble();
            int humidity = mainObj.value("humidity").toInt();
            int pressure = mainObj.value("pressure").toInt();
            int seaLevel = mainObj.value("sea_level").toInt();
            int grndLevel = mainObj.value("grnd_level").toInt();

            formattedText.append("Main Weather Data:\n");
            formattedText.append(QString("  Feels Like: %1°C\n").arg(feelsLike - 273.15));
            formattedText.append(QString("  Ground Level Pressure: %1 mmHg\n").arg(grndLevel * 0.75006));
            formattedText.append(QString("  Humidity: %1%\n").arg(humidity));
            formattedText.append(QString("  Pressure: %1 mmHg\n").arg(pressure * 0.75006));
            formattedText.append(QString("  Sea Level Pressure: %1 mmHg\n").arg(seaLevel * 0.75006));
            formattedText.append(QString("  Temperature: %1°C\n").arg(temp - 273.15));
            formattedText.append(QString("  Max Temperature: %1°C\n").arg(tempMax - 273.15));
            formattedText.append(QString("  Min Temperature: %1°C\n\n").arg(tempMin - 273.15));
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

        if (jsonObj.contains("timezone")) {
            int timezone = jsonObj.value("timezone").toInt();
            formattedText.append(QString("Timezone: %1\n").arg(QDateTime::fromSecsSinceEpoch(timezone).toString("hh:mm")));
        }

        if (jsonObj.contains("visibility")) {
            int visibility = jsonObj.value("visibility").toInt();
            formattedText.append(QString("Visibility: %1 km\n\n").arg(visibility / 1000));
        }

        if (jsonObj.contains("weather")) {
            QJsonArray weatherArray = jsonObj.value("weather").toArray();
            if (!weatherArray.isEmpty()) {
                QJsonObject weatherObj = weatherArray.first().toObject();
                QString description = weatherObj.value("description").toString();
                QString icon = weatherObj.value("icon").toString();
                int id = weatherObj.value("id").toInt();
                QString main = weatherObj.value("main").toString();

                formattedText.append("Weather Conditions:\n");
                formattedText.append(QString("  Description: %1\n").arg(description));
                formattedText.append(QString("  Icon Code: %1\n").arg(icon));
                formattedText.append(QString("  ID: %1\n").arg(id));
                formattedText.append(QString("  Main: %1\n\n").arg(main));
            }
        }

        if (jsonObj.contains("wind")) {
            QJsonObject windObj = jsonObj.value("wind").toObject();
            int deg = windObj.value("deg").toInt();
            double gust = windObj.value("gust").toDouble();
            double speed = windObj.value("speed").toDouble();

            formattedText.append("Wind Data:\n");
            formattedText.append(QString("  Direction: %1°\n").arg(deg));
            formattedText.append(QString("  Gust: %1 km/h\n").arg(gust * 3.6));
            formattedText.append(QString("  Speed: %1 km/h\n").arg(speed * 3.6));
        }

        QString styledText = QString("<h2><b><u>City Info:</u></b></h2><pre style='font-family: Courier; font-size: 10pt; color: #ECEFF4;'>%1</pre>").arg(formattedText);

        ui->labelDetails->setText(styledText);
        ui->labelDetails->setWordWrap(true);

        // Fetch AQI data if coordinates are available
        if (jsonObj.contains("coord")) {
            QJsonObject coordObj = jsonObj.value("coord").toObject();
            double lat = coordObj.value("lat").toDouble();
            double lon = coordObj.value("lon").toDouble();

            QString apiUrl = "http://api.openweathermap.org/data/2.5/air_pollution";
            QUrl url(apiUrl);
            QUrlQuery query;
            query.addQueryItem("lat", QString::number(lat));
            query.addQueryItem("lon", QString::number(lon));
            query.addQueryItem("appid", apiKey);
            url.setQuery(query);

            QNetworkRequest request(url);

            disconnect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onCityResult);
            connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAQIResult);
            networkManager->get(request);
        }
    }

    reply->deleteLater();
}


void MainWindow::onAQIResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        //if (jsonObj.contains("coord")) {
          //  QJsonObject coordObj = jsonObj.value("coord").toObject();
    //double lat = coordObj.value("lat").toDouble();
      //          double lon = coordObj.value("lon").toDouble();

            QString formattedText;
           /* formattedText.append(QString("coordinates:\n latitude:%1 longitude:%2\n\n").arg(lat).arg(lon));*/

            if (jsonObj.contains("list")) {
                QJsonArray listArray = jsonObj.value("list").toArray();
                if (!listArray.isEmpty()) {
                    QJsonObject firstEntry = listArray.first().toObject();
                    QJsonObject components = firstEntry.value("components").toObject();
                    formattedText.append("components: \n");
                    QStringList componentList;
                    for (const QString& key : components.keys()) {
                        componentList.append(QString("   %1: %2").arg(key).arg(components.value(key).toDouble()));
                    }
                    formattedText.append(componentList.join("\n"));
                    formattedText.append("\n\n");

                    qint64 dt = firstEntry.value("dt").toVariant().toLongLong();
                    QDateTime dateTime;
                    dateTime.setSecsSinceEpoch(dt);
                    QString dateTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");

                    QJsonObject mainObj = firstEntry.value("main").toObject();
                    int aqi = mainObj.value("aqi").toInt();

                    formattedText.append(QString("Date-time:  %1 \naqi:  %2").arg(dateTimeStr).arg(aqi));

                    QString styledText = QString("<h2><b><u></u></b></h2><pre style='font-family: Courier; font-size: 10pt; color: #ECEFF4;'>%1</pre>").arg(formattedText);
                    ui->labelDetails->setText(ui->labelDetails->text() + styledText);

                    QString city = ui->lineEditCity->text();
                    QString aqiText = QString("AQI for %1: %2").arg(city).arg(aqi);

                    std::ofstream outputFile("aqi_data.txt", std::ios::app);
                    if (outputFile.is_open()) {
                        outputFile << aqiText.toStdString() << "\n";
                        outputFile.close();
                    } else {
                        std::cerr << "Failed to open the output file." << std::endl;
                    }
                }
            }

    }

    reply->deleteLater();
    disconnect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onAQIResult);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onCityResult);
}


QString MainWindow::formatJsonString(const QString &jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    return jsonDoc.toJson(QJsonDocument::Indented);
}

