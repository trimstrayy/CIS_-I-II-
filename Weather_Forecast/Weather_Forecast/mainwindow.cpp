#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // from map ----------------
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map_location.qml")));
    ui->quickWidget->show();
    // end -from map
}

MainWindow::~MainWindow()
{
    delete ui;
}
