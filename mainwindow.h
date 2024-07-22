#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void fetchresult();
    void onAQIResult(QNetworkReply *reply);
    void onCityResult(QNetworkReply *reply);

    //QString getQualitativeName(int index);



private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString apiKey;

    QLineEdit *lineEditCity;
    QPushButton *pushButtonGetAQI;
    QLabel *labelAQI;
// QString getQualitativeName(int index);
    QString formatJsonString(const QString &jsonString);


};

#endif
