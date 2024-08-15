import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: weatherPage
    width: Window.width
    height: Window.height
    title: "City Information"
    property string cityName: ""

    background: Rectangle {
        color: "#f0f0f0"
        Image {
            source: "path/to/subtle_weather_background.jpg"  // Add a subtle background image
            anchors.fill: parent
            opacity: 0.1
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Rectangle {
            width: parent.width
            height: 60
            color: "#3498db"
            radius: 10
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Text {
                    id: cityNameDisplay
                    text: "City Information for " + cityName
                    font.pixelSize: 24
                    font.bold: true
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Button {
                    text: "Close"
                    onClicked: weatherPage.visible = false
                    background: Rectangle {
                        color: "#2980b9"
                        radius: 5
                    }
                    contentItem: Text {
                        text: "Close"
                        color: "white"
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            background: Rectangle {
                color: "#ffffff"
                radius: 10
                border.color: "#e0e0e0"
                border.width: 1
            }

            TextArea {
                id: weatherDisplay
                readOnly: true
                wrapMode: TextArea.Wrap
                textFormat: TextArea.RichText
                font.pixelSize: 16
                padding: 15
                color: "#333333"
                // The content will be set dynamically
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
        return "<style>"
                   + "h2 { color: #2c3e50; margin-top: 15px; margin-bottom: 10px; }"
                   + "table { width: 100%; border-collapse: collapse; }"
                   + "th, td { padding: 8px; border: 1px solid black; text-align: left; }"
                   + "th { background-color: #2980b9; color: white; }"
                   + ".label { font-weight: bold; color: #34495e; text-align: left; }"
                   + ".value { color: #2980b9; text-align: right; }"
                   + "</style>"
                   + "<table>"
                   + info.replace(/\n/g, "<tr><td colspan='2'>&nbsp;</td></tr>")
                         .replace(/Coordinates:/g, "<tr><th colspan='2'>Coordinates</th></tr>")
                         .replace(/Main Weather Data:/g, "<tr><th colspan='2'>Main Weather Data</th></tr>")
                         .replace(/Weather Conditions:/g, "<tr><th colspan='2'>Weather Conditions</th></tr>")
                         .replace(/Wind Data:/g, "<tr><th colspan='2'>Wind Data</th></tr>")
                         .replace(/Air Quality Index \(AQI\):/g, "<tr><th colspan='2'>Air Quality Index (AQI)</th></tr>")
                         .replace(/Pollutants/g, "<tr><th colspan='2'>Pollutants</th></tr>")
                         .replace(/(\w+):\s*([^<]+)/g, "<tr><td class='label'>$1:</td><td class='value'>$2</td></tr>")
                   + "</table>";
    }

    function updateWeatherInfo(city) {
        cityName = city
        weatherInfo.fetchResult(city)
    }
}
