//for map and mapHandling

import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

Rectangle {
    id : window
    //width: 1500
    //height: 500

    Plugin {
        id: googlemapview
        name: "osm"
        PluginParameter{
            name: "https://tile.thunderforest.com/atlas/{z}/{x}/{y}.png?apikey=4913456783de4d6e8f896059fe631d9b"
            value: "4913456783de4d6e8f896059fe631d9b"
        }
    }

    Map {
        id: mapview
        anchors.fill: parent
        plugin: googlemapview
        //center: QtPositioning.coordinate(27.7172, 85.3240)  //coordinates of kathmandu
        zoomLevel:10
        activeMapType: mapview.supportedMapTypes[mapview.supportedMapTypes.length - 1] //using the steet map - trying to remove the watermark.


        Component.onCompleted: {
            console.log("Map component loaded")
            console.log("Center:", center)
            console.log("Plugin name:", plugin.name)
        }

    /*    MouseArea {
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
    }*/
}
}
