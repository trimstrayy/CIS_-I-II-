import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: weatherWindow
    width: 560
    height: 400
    title: "Additional Weather Information"
    color: "#f0f0f0"
    visible: true

    property string cityName: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            id: cityNameDisplay
            text: "Weather Information for " + cityName
            font.pixelSize: 20
            font.bold: true
            Layout.fillWidth: true
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
                font.pixelSize: 14
                padding: 10
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
                   .replace(/Air Quality Index \(AQI\):/g, "<b>Air Quality Index (AQI):</b>")
                   .replace(/Pollutants/g, "<b>Pollutants</b>")
    }

    function updateWeatherInfo(city) {
        cityName = city
        weatherInfo.fetchResult(city)
    }
}
