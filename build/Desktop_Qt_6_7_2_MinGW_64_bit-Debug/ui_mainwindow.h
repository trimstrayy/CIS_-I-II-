/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEditCity;
    QPushButton *pushButtonGetAQI;
    QLabel *labelDetails;
    QLabel *labelAQI;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        lineEditCity = new QLineEdit(centralwidget);
        lineEditCity->setObjectName("lineEditCity");

        verticalLayout->addWidget(lineEditCity);

        pushButtonGetAQI = new QPushButton(centralwidget);
        pushButtonGetAQI->setObjectName("pushButtonGetAQI");

        verticalLayout->addWidget(pushButtonGetAQI);

        labelDetails = new QLabel(centralwidget);
        labelDetails->setObjectName("labelDetails");

        verticalLayout->addWidget(labelDetails);

        labelAQI = new QLabel(centralwidget);
        labelAQI->setObjectName("labelAQI");

        verticalLayout->addWidget(labelAQI);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonGetAQI->setText(QCoreApplication::translate("MainWindow", "Search Location", nullptr));
        labelDetails->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        labelAQI->setText(QCoreApplication::translate("MainWindow", "The AQI for your City is:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
