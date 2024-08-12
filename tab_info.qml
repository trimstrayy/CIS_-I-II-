import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    width: 800
    height: 600

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        TextField {
            id: cityInput
            Layout.fillWidth: true
            placeholderText: "Enter city name"
            onAccepted: fetchButton.clicked()
        }

        Button {
            id: fetchButton
            text: "Get Weather & AQI"
            Layout.fillWidth: true
            onClicked: {
                // Call C++ function to fetch data
                backend.fetchResult(cityInput.text)
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: resultArea
                readOnly: true
                wrapMode: TextEdit.Wrap
                textFormat: TextEdit.RichText
            }
        }
    }

    // C++ backend connection
    Connections {
        target: backend
        function onResultReady(result) {
            resultArea.text = result
        }
    }
}
