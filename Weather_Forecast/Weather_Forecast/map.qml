//for map and mapHandling

import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle {
    width: 1500
    height: 500

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(27.7172, 85.3240)  //coordinates of kathmandu
        zoomLevel: 12

        Component.onCompleted: {
            console.log("Map component loaded")
            console.log("Center:", center)
            console.log("Plugin name:", plugin.name)
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            drag.target: mapView
            drag.axis: Drag.XAndYAxis

            onPressed: {
                mouseArea.drag.active = true
            }

            onReleased: {
                mouseArea.drag.active = false
            }

            onPositionChanged: {
                mapView.center = mapView.toCoordinate(mouseArea.width / 2 - mouseArea.drag.target.x, mouseArea.height / 2 - mouseArea.drag.target.y)
            }
        }

        PinchArea {
            id: pinchArea
            anchors.fill: parent

            onPinchUpdated: {
                mapView.zoomLevel += pinch.scale - pinch.previousScale
            }
        }
    }
}
