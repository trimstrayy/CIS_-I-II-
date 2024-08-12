import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

ApplicationWindow {
    id: fullMapWindow
    visible: true
    width: 2560
    height: 1440
    // Add these properties to receive values
    property real initialLatitude: 27.7083
    property real initialLongitude: 85.3206

Page{

    id: fullMapPage
    visible: true
    anchors.fill: parent
    title: "Full Map View"


    // Use the received values
    property real latitude: fullMapWindow.initialLatitude
    property real longitude: fullMapWindow.initialLongitude
    property bool showMapbool: true


    Map {
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"
        }
        center: QtPositioning.coordinate(fullMapPage.latitude, fullMapPage.longitude)
        zoomLevel: 15
    }
}
}
