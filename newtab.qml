import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

ApplicationWindow {
    width: 800
    height: 600
Page{

    id: fullMapWindow
    visible: true
    width: 3400
    height: 1600
    title: "Full Map View"

    property real latitude: 0
    property real longitude: 0
    property bool showMapbool: true


    Map {
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"
        }
        center: QtPositioning.coordinate(latitude, longitude)
        zoomLevel: 12


    }


    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 30
        height: 30
        text: "✕"
        onClicked: {
            showMapbool = false
        }
    }


}
}
