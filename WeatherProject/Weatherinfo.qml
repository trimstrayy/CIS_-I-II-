import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: weatherPage
    width: 1400
    height: 900
    title: "Weather Information"

    property string cityName: ""

    background: Rectangle {
        color: "#e0e0e0"
        radius: 10
        border.color: "#b0b0b0"
        border.width: 2
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Rectangle {
            width: parent.width
            height: 50
            color: "#4A90E2"
            radius: 8
            border.color: "#357ABD"
            Layout.fillWidth: true
            Text {
                id: cityNameDisplay
                text: "Weather Information for " + cityName
                font.pixelSize: 22
                font.bold: true
                color: "white"
                anchors.centerIn: parent
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            background: Rectangle {
                color: "#ffffff"
                radius: 8
                border.color: "#dcdcdc"
                border.width: 1
            }

            TextArea {
                id: weatherDisplay
                readOnly: true
                wrapMode: TextArea.Wrap
                textFormat: TextArea.RichText
                font.pixelSize: 16
                padding: 10
                color: "#333333"
            }
        }
    }

    Connections {
        target: weatherInfo
        function onWeatherInfoUpdated(info) {
            weatherDisplay.text = formatWeatherInfo(info)
        }
    }

    function formatWeatherInfo(info) {
        return info.replace(/\n/g, "<br>")
                   .replace(/Coordinates:/g, "<b>Coordinates:</b>")
                   .replace(/Main Weather Data:/g, "<b>Main Weather Data:</b>")
                   .replace(/Weather Conditions:/g, "<b>Weather Conditions:</b>")
                   .replace(/Wind Data:/g, "<b>Wind Data:</b>")
                   .replace(/Air Quality Index (AQI):/g, "<b>Air Quality Index (AQI):</b>")
                   .replace(/Pollutants/g, "<b>Pollutants</b>")
    }

    function updateWeatherInfo(city) {
        cityName = city
        weatherInfo.fetchResult(city)
    }
}
