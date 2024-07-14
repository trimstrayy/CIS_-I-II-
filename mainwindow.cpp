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
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);
    apiKey = "c75997502cfcd1b939260acf6e491ec4";  // Replace with your actual API key

    connect(ui->pushButtonGetAQI, &QPushButton::clicked, this, &MainWindow::fetchresult);
    connect(ui->lineEditCity, &QLineEdit::returnPressed, this, &MainWindow::fetchresult);

    QRegularExpression re("^[A-Z][a-zA-Z ]+$");
    QValidator *validator = new QRegularExpressionValidator(re, this);
    ui->lineEditCity->setValidator(validator);

    // Set up QCompleter for city name suggestions
    QStringList cityNames = {{"Kathmandu", "Pokhara", "Lalitpur", "Bhaktapur", "Okhaldhunga","Biratnagar", "Bharatpur", "Janakpur", "Hetauda", "Dharan","Pyuthan", "Butwal", "Itahari", "Nepalgunj", "Dhangadhi", "Birgunj", "Birendranagar", "Damak", "Dhankuta", "Ilam", "Jumla", "Mahendranagar", "Tansen"}
};
    QStringListModel *model = new QStringListModel(cityNames, this);
    QCompleter *completer = new QCompleter(model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditCity->setCompleter(completer);

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onCityResult);
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

   // ui->labelAQI->setText("Fetching AQI data...");
    ui->labelDetails->clear(); // Clear the details label before making the request
}

void MainWindow::onCityResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString apiUrl = reply->url().toString();
        QString jsonResponse = QString::fromUtf8(responseData);


        QString formattedText = formatJsonString(jsonResponse);
        QString styledText = QString("<h2><b><u>City Info:<u><b></h2><pre style='font-family: Courier; font-size: 10pt; color: #FFFFFF;'>%1</pre>").arg(formattedText);

        ui->labelDetails->setText(styledText);
        ui->labelDetails->setWordWrap(true);

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
        } else {
           // ui->labelAQI->setText("Error: City not recognized.");
        }
    } else {
        //ui->labelAQI->setText("Network error: " + reply->errorString());
    }

    reply->deleteLater();
}

QString MainWindow::getQualitativeName(int index) {
    static const QMap<int, QString> indexToQualitativeName = {
        {1, "Good"},
        {2, "Fair"},
        {3, "Moderate"},
        {4, "Poor"},
        {5, "Very Poor"}
    };
    return indexToQualitativeName.value(index, "Unknown Index");
}

void MainWindow::onAQIResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        QString apiUrl = reply->url().toString();
        QString jsonResponse = QString::fromUtf8(responseData);

        ui->labelDetails->setText(ui->labelDetails->text() + QString("\nAQI Response JSON: %1").arg(formatJsonString(jsonResponse)));

        if (jsonObj.contains("list")) {
            QJsonArray listArray = jsonObj.value("list").toArray();
            if (!listArray.isEmpty()) {
                QJsonObject firstEntry = listArray.first().toObject();
                QJsonObject mainObj = firstEntry.value("main").toObject();
                int aqi = mainObj.value("aqi").toInt();

                QString qualitativeName = getQualitativeName(aqi);
                QString city = ui->lineEditCity->text();
                QString aqiText = QString("AQI for %1: %2 (%3)").arg(city).arg(aqi).arg(qualitativeName);

                //ui->labelAQI->setText(aqiText);

                // Save the AQI data to a .txt file
                std::ofstream outputFile("aqi_data.txt", std::ios::app);
                if (outputFile.is_open()) {
                    outputFile << aqiText.toStdString() << "\n";
                    outputFile.close();
                } else {
                    std::cerr << "Failed to open the output file." << std::endl;
                }
            } else {
                //ui->labelAQI->setText("Error: No data available.");
            }
        } else {
          //  ui->labelAQI->setText("Error: Invalid response format.");
        }
    } else {
    //    ui->labelAQI->setText("Network error: " + reply->errorString());
    }

    reply->deleteLater();
}

QString MainWindow::formatJsonString(const QString &jsonString)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    return jsonDoc.toJson(QJsonDocument::Indented);
}
