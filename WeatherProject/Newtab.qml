import QtQuick 2.12
import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Window 2.12
import QtLocation 5.12
import QtPositioning 5.12

Page {
    id: fullMapWindow
    visible: true
    width: 1447
    height: 900
    title: "Full Map View"
    property real latitude: 51.5074  // Default to London coordinates
    property real longitude: -0.1278 // Adjust these as needed

    function closePage() {
        fullMapWindow.visible = false
        // If this page is opened in a separate window, use the following instead:
        // Qt.quit()
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(latitude, longitude)
        zoomLevel: 12
        property geoCoordinate startCentroid

        Component.onCompleted: {
            console.log("Map center:", center.latitude, center.longitude)
            console.log("Zoom level:", zoomLevel)
        }

        // PinchHandler {
        //     id: pinch
        //     target: null
        //     onActiveChanged: if (active) {
        //         map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
        //     }
        //     onScaleChanged: (delta) => {
        //         map.zoomLevel += Math.log2(delta)
        //         map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        //     }
        //     onRotationChanged: (delta) => {
        //         map.bearing -= delta
        //         map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        //     }
        //     grabPermissions: PointerHandler.TakeOverForbidden
        // }

        //Mouse Drag
        DragHandler {
              id: drag
              target: null
              onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
          }

        //Zoom In function
        Shortcut {
                   enabled: map.zoomLevel < map.maximumZoomLevel
                   sequence: StandardKey.ZoomIn
                   onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
               }

        //Zoom Out function
        Shortcut {
               enabled: map.zoomLevel > map.minimumZoomLevel
               sequence: StandardKey.ZoomOut
               onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
           }
    }

    Plugin {
        id: mapPlugin
        name: "osm"  // Using standard OpenStreetMap tiles
    }

    Button {
        id: closeButton
        anchors.top: parent.top
        anchors.right: parent.right
        width: 30
        height: 30
        text: "✕"
        onClicked: closePage()
    }

    Item {
        anchors.fill: parent
        focus: true
        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Escape) {
                closePage();
                event.accepted = true;
            }
        }
    }

    Component.onCompleted: {
        console.log("Map component loaded")
        forceActiveFocus()
    }
}
