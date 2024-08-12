/*import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 700
    height: 5000
    color: "#2E3440"

    Column {
        spacing: 10
        anchors.centerIn: parent

        TextField {
            id: cityField
            placeholderText: "Enter City Name"
            width: parent.width * 0.8
        }

        Button {
            text: "Get Weather & AQI"
            onClicked: {
                weatherInfo.fetchResult(cityField.text)
            }
        }

        ScrollView {
            width: parent.width * 0.8
            height: 200

            Text {
                text: weatherInfo.formattedWeatherText
                color: "#ECEFF4"
                wrapMode: Text.Wrap
            }
        }

        ScrollView {
            width: parent.width * 0.8
            height: 200

            Text {
                text: weatherInfo.formattedAQIText
                color: "#ECEFF4"
                wrapMode: Text.Wrap
            }
        }
    }
}
