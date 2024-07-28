import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
import QtLocation 5.15
import QtPositioning 5.15

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Weather Map"

    property real currentLatitude: 27.7172
    property real currentLongitude: 85.3240

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Weather information display
        Rectangle {
            Layout.fillWidth: true
            height: 100
            color: "lightblue"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text {
                    id: locationText
                    font.pixelSize: 18
                    font.bold: true
                    text: "Kathmandu"
                }

                Text {
                    id: weatherText
                    font.pixelSize: 16
                    text: "Loading weather..."
                }

                Text {
                    id: temperatureText
                    font.pixelSize: 16
                    text: "Temperature: --°C"
                }
            }
        }

        // Map
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Plugin {
                id: mapPlugin
                name: "osm" // Use OpenStreetMap
            }

            Map {
                id: map
                anchors.fill: parent
                plugin: mapPlugin
                center: QtPositioning.coordinate(currentLatitude, currentLongitude)
                zoomLevel: 10

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var coordinate = map.toCoordinate(Qt.point(mouseX, mouseY))
                        currentLatitude = coordinate.latitude
                        currentLongitude = coordinate.longitude
                        marker.coordinate = coordinate
                        updateWeather()
                    }
                }

                MapQuickItem {
                    id: marker
                    anchorPoint.x: markerImage.width/2
                    anchorPoint.y: markerImage.height
                    coordinate: QtPositioning.coordinate(currentLatitude, currentLongitude)


                    sourceItem: Image {
                        id: markerImage
                        source: "path_to_your_marker_image.png" // Replace with your marker image
                        width: 40
                        height: 40
                    }
                }
            }
        }
    }

    function updateWeather() {
        // Here you would typically call your C++ backend to get weather data
        // For this example, we'll use placeholder text
        locationText.text = "Location: " + currentLatitude.toFixed(4) + ", " + currentLongitude.toFixed(4)
        weatherText.text = "Weather: Updating..."
        temperatureText.text = "Temperature: Updating..."

        // Simulate an API call delay
        weatherTimer.start()
    }

    Timer {
        id: weatherTimer
        interval: 1000 // 1 second delay to simulate API call
        onTriggered: {
            // This is where you would update with real data from your C++ backend
            weatherText.text = "Weather: Partly Cloudy"
            temperatureText.text = "Temperature: " + (Math.random() * 30 + 10).toFixed(1) + "°C"
        }
    }

    Component.onCompleted: {
        updateWeather()
    }
}
