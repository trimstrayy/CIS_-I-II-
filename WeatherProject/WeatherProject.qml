import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtLocation 5.12
import QtPositioning 5.12
import QtMultimedia

ApplicationWindow {
    id: main
    visible: true
    width: 2560
    height: 1440
    title: "Weather Forecast"
   // property int activeIndex: 1                  //TO INDICATE WHERE WE ARE VIEWING

    property real latitude: weatherForecast.get_latitude("Kathmandu")
    property real longitude: weatherForecast.get_longitude("Kathmandu")
    property real icon_index: 0
    property var temperatures: [] // Array to store temperatures
    property var icons: [] // Array to store weather icons
    property real humiduty: 0

    Item {
        anchors.fill: parent

        MediaPlayer {
            id: player
            source: "video/background.mp4"
            audioOutput: AudioOutput {
                muted: true  // Mute the audio
            }
            videoOutput: videoOutput
            loops: MediaPlayer.Infinite
        }

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            fillMode: VideoOutput.PreserveAspectCrop
        }

        Component.onCompleted: {
            player.play()  // Start playing when the component is loaded
        }
    }
    property string errorMessage: ""

    Rectangle {
       id: errorPopup
       width: 300
       height: 50
       color: "#E74C3C"
       radius: 10
       anchors.centerIn: parent
       visible: false
       opacity: 1
       z: 1000 // This ensures it appears above other elements


       Text {
           anchors.centerIn: parent
           color: "white"
           font.pixelSize: 16
           text: errorMessage
           horizontalAlignment: Text.AlignHCenter
           verticalAlignment: Text.AlignVCenter
       }
    }


    // Top navigation bar
    Rectangle {
        width: parent.width
        height: 50
        color: "#34495e"  // Top bar color
        RowLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10



            // Top rectangle
            Rectangle {
                width: 34
                height: 34
                color: "transparent"
                border.color: "transparent"  // Remove white border
                border.width: 0
                radius: 5

                Image {
                    source: "photos//drawer.png"  // Drawer icon
                    width: 37
                    height: 37
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            navDrawer.open()
                        }
                    }
                }
            }

            Text {
                text: "Forecast"
                color: "white"
                font.bold: true
                font.pixelSize: 25
                Layout.alignment: Qt.AlignVCenter
            }

            // Spacer to push icons to the right
            Item {
                Layout.fillWidth: true
            }

            // Other icons
            Image {
                source: "jp.png"  // Add your icon source here
                width: 1
                height: 1
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
            }

            Image {
                source: "star_icon.png"  // Add your icon source here
                width: 25
                height: 25
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
            }

            Image {
                source: "shuffle_icon.png"  // Add your icon source here
                width: 25
                height: 25
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
            }

            Image {
                source: "settings_icon.png"  // Add your icon source here
                width: 25
                height: 25
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignVCenter
            }

            // Search Box
            Rectangle {
                width: 220
                height: 31
                color: "#2e3256"
                radius: 20
                Layout.alignment: Qt.AlignRight
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                property string searchText:""

                function performSearch() {
                try {
                    if (searchField.text.trim() === "") throw "Please enter a location.";

                    var latitude = weatherForecast.get_latitude(searchField.text);
                    var longitude = weatherForecast.get_longitude(searchField.text);

                    if (latitude === "error" || longitude === "error") throw "Invalid city name. Please try again.";

                    var latNum = parseFloat(latitude);
                    var lonNum = parseFloat(longitude);

                    if (isNaN(latNum) || isNaN(lonNum)) throw "Invalid city name. Please try again.";

                    if (typeof map !== 'undefined' && map.center) {
                        map.center = QtPositioning.coordinate(latNum, lonNum);
                    } else {
                        throw "Map or map.center is not available";
                    }

                    latitudelongitude.children[0].children[0].children[0].children[2].text = `${latNum.toFixed(4)} and ${lonNum.toFixed(4)}`;
                    maincontent.children[0].children[0].children[0].children[0].children[0].text = weatherForecast.getCity(searchField.text);
                    maincontent.children[0].children[0].children[0].children[0].children[1].text = weatherForecast.get_weather(latitude, longitude);
                    maincontent.children[0].children[0].children[0].children[3].text = `${weatherForecast.get_temperature(latitude, longitude)} °C`;
                    maincontent.children[0].children[0].children[0].children[2].source = weatherForecast.get_icon(latitude, longitude);
                    weatherForecast.get_temperature_hourly(latitude, longitude, 11);
                    weatherForecast.get_current_weather(latitude, longitude);
                    weatherForecast.get_aqi(latitude, longitude);
                    console.log(aqi.children[0].children[0].children[0].children[0]);
                }  catch (error) {
                    errorMessage = error;
                    errorPopup.visible = true;
                    Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorPopup.visible = false }", errorPopup);
                }
            }

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 10
                    TextField {
                        id: searchField
                        placeholderText: "Search location"
                        color: "white"
                        background: Rectangle { color: "transparent" }
                        Layout.fillWidth: true
                        font.pixelSize: 14
                        anchors.left: parent.left
                        anchors.right: searchButton.left
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 8
                        rightPadding: 5
                        padding: 5
                        onAccepted: parent.parent.performSearch()
                    }


                    Button {
                            id: searchButton
                            width: 25
                            height: 25
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 5
                            background: Rectangle {
                                            color: "transparent"
                                            border.color: "transparent"
                                        }

                            contentItem: Image {
                                source: "photos/search.png"
                                sourceSize.width: 24
                                sourceSize.height: 24
                                fillMode: Image.PreserveAspectFit
                                anchors.centerIn: parent
                             }

                            onClicked: parent.parent.performSearch()

                         }
                    }
                }
        }
    }

    /*// Define color variables
        property color boxColor1: "#3498db"  // Light blue
        property color boxColor2: "#2ecc71"  // Light green                        //BOX BHITRA COLOR KO LAGI
        property color boxColor3: "#e74c3c"  // Light red
        property color boxColor4: "#f39c12"  // Orange*/


    //-------------------------------------------------------------------------------DRAWER---------------------------------------------------------------------------------
    Drawer {
        id: navDrawer
        width: 50
        height: parent.height
        edge: Qt.LeftEdge

        Button {
                id: drawerButton
                width: 50
                height: 50
                //anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.topMargin :10

                background:
                    Rectangle {
                                color: "transparent"
                                border.color: "transparent"
                            }

                contentItem: Image {
                    source: "photos/drawer.png"                           //------------------------- DRAWER ICON --------------------------
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                 }

                onClicked: {
                //console.log("Searched Text: ", drawerField.text)
                console.log("function call")        //function call
                }
            }

        Item {
             Layout.fillHeight: true
         }

            MouseArea {
                anchors.fill: parent
                //hoverEnabled: true
                onExited: {
                    navDrawer.close()
                }
            }


    Button {
            id: homeButton
            width: 50
            height: 50
           //nchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.top:parent.top
            anchors.topMargin : 200


            background:
                Rectangle {
                            color: "transparent"
                            border.color: "transparent"

                        }

            contentItem: Image {
                source: "photos/home.png"                        //--------------------------HOME ICON -------------------------------
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent

             }

            onClicked: {
            //console.log("Searched Text: ", homeField.text)
            console.log("function call")        //function call
            }
        }

   /* Button {
            id: mapButton
            width: 50
            height: 50
           //nchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.top:parent.top
            anchors.topMargin : 400


            background:
                Rectangle {
                            color: "transparent"
                            border.color: "transparent"

                        }

            contentItem: Image {
                source: "photos/mapicon.png"                        //--------------------------MAP ICON -------------------------------
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent

             }
            StackView {
                    id: stackView
                    anchors.fill: parent

                    // Transition animation
                    pushEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 200
                        }
                    }
                    pushExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to: 0
                            duration: 200
                        }
                    }
                    popEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 200
                        }
                    }
                    popExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to: 0
                            duration: 200
                        }
                    }
            }

            onClicked: {
                var lat = weatherForecast.get_latitude(searchField.text);
                var lon = weatherForecast.get_longitude(searchField.text);
                var component = Qt.createComponent("Newtab.qml");
                if (component.status === Component.Ready) {
                    var newWindow = component.createObject(main, {
                        "initialLatitude": lat,
                        "initialLongitude": lon
                    });
                    newWindow.show();
                } else {
                    console.error("Error loading Newtab.qml:", component.errorString());
                }
            }

    }*/

    Button {
        id: weatherinfotab
        width: 50
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.top: parent.top
        anchors.topMargin: 400

        background: Rectangle {
            color: "transparent"
            border.color: "transparent"
        }

        contentItem: Image {
            source: "photos/info.jpg"
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
        }

            onClicked: {
                var component = Qt.createComponent("Weatherinfo.qml");
                if (component.status === Component.Ready) {
                    var weatherWindow = component.createObject(main);
                    weatherWindow.updateWeatherInfo(searchField.text);
                    weatherWindow.show();
                } else {
                    console.error("Error loading WeatherInfo.qml:", component.errorString());
                }
            }

    }


    Item {
         Layout.fillHeight: true
     }
    }



    //--------------------------------------------------------MAIN CONTENT---------------------------------------------------------------

        Item {
           id: maincontent;
           anchors.fill: parent
           anchors.margins: 20

           GridLayout {
               //anchors.centerIn: parent
               anchors.left:parent.left
               anchors.top: parent.top
               width: parent.width * 0.4
               columns: 2
               rowSpacing: 20
               columnSpacing: 20
               anchors.leftMargin: 100          //left to right kati bhanera
               anchors.topMargin: 125 //tala kati lagne bhanera


               // Current Weather
               Rectangle {
                   id: currentWeather;
                   Layout.columnSpan: 2
                   Layout.fillWidth: true
                   height: 150
                   color: "transparent"
                   opacity: 0.8
                   radius: 10
                   width: parent.width



                   RowLayout {
                       anchors.fill: parent
                       anchors.margins: 20
                       spacing: 20

                       Column {
                           Layout.fillHeight: true
                           Text {
                               id: city;
                               text: "Kathmandu"
                               font.pixelSize: 30
                               font.weight: Font.Bold
                               color: "black"
                           }
                           Text {
                               id: description;
                               text:{
                                   var latitude = main.latitude;
                                   var longitude = main.longitude;
                                   `${weatherForecast.get_weather(latitude, longitude)}`;
                               }

                               font.pixelSize: 18
                               color: "black"
                           }
                       }

                       Item { Layout.fillWidth: true }

                       Image {
                           id: weatherIcon
                            // source: "photos/sunny.png"
                           source: weatherForecast.get_icon(main.latitude, main.longitude);
                           fillMode: Image.PreserveAspectFit
                           Layout.preferredHeight: 65
                           Layout.preferredWidth: 65
                        }

                       Text {
                           id: temp;
                           text: {
                               var latitude = main.latitude;
                               var longitude = main.longitude;
                               `${weatherForecast.get_temperature(latitude, longitude)} °C`;}
                           font.pixelSize: 48
                           color: "black"
                       }
                   }
               }


               // Hourly Forecast
               Rectangle {
                   id:hourlyForecast
                   Layout.fillWidth: true
                   height: 250
                   width: parent.width
                   color: "transparent"//boxColor3
                   opacity: 0.8
                   radius: 10

                   ColumnLayout {
                       anchors.fill: parent
                       anchors.margins: 10

                       Text {
                           text: "Hourly Forecast"
                           font.pixelSize: 25
                           font.weight: Font.Bold
                           color: "black"
                       }

                       RowLayout {
                           spacing: 10
                           Repeater {
                               id: hourlyreapeter
                               model: 10
                               Rectangle {
                                   width: 55
                                   height: 140
                                   color: "#34495e"
                                   radius: 5
                                   Column {
                                       anchors.centerIn: parent
                                       spacing: 5
                                       Text {
                                           text: (model.index + 1) + "h"
                                           font.pixelSize: 15
                                           color: "black"
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                       Image{
                                           source: "/Coding/c++/git/desing/I-II-Project-/Project Pic src/cloudy.png";
                                           fillMode: Image.PreserveAspectFit
                                           anchors.horizontalCenter: parent.horizontalCenter
                                           width: 55
                                           height: 55
                                        }
                                       Text {
                                           id: tempText
                                           text: temperatures[model.index] !== undefined ? temperatures[model.index] + " °C" : ""
                                           font.pixelSize: 15
                                           // text: "23 C"
                                           color: "black"
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                       ParallelAnimation {
                                           running: true
                                           NumberAnimation { target: parent; property: "opacity"; from: 0; to: 1; duration: 500; easing.type: Easing.InOutQuad }
                                           NumberAnimation { target: parent; property: "scale"; from: 0.8; to: 1; duration: 500; easing.type: Easing.InOutQuad }
                                       }
                                   }
                               }
                           }
                       }
                   }
               }


           }
       }
    Item {
        anchors.fill: parent
        anchors.margins: 20

        GridLayout {
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width * 0.4
            columns: 2
            rowSpacing: 220
            columnSpacing: 20
            anchors.leftMargin: 100
            anchors.topMargin: 125


            // Current Weather (unchanged)
            Rectangle {
                // ... (existing code for current weather)
            }

            // Weather Conditions (unchanged)
            Rectangle {
                // ... (existing code for weather conditions)
            }

            // Hourly Forecast (unchanged)
            Rectangle {
                // ... (existing code for hourly forecast)
            }

            Rectangle {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                height: 250
                color: "transparent" // Light gray background
                opacity: 0.8
                radius: 10

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 2

                    Text {
                        text: "Weather Metrics"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        color: "black"
                    }

                    GridLayout {
                        id: weatherMetrics
                        columns: 3  // Display metrics in two columns
                        rowSpacing: 20
                        columnSpacing: 40

                        Repeater {
                            id: weatherMatrix
                            model: ListModel {
                                ListElement { label: "Humidity :"; value: "78.0"; icon: "photos/humidity.png" }
                                ListElement { label: "Clouds :"; value: "40.0"; icon: "photos/cloudiness.png" }
                                ListElement { label: "Pressure :"; value: "1015"; icon: "photos/pressure.png" }
                                ListElement { label: "Visibility :"; value: "5.2"; icon: "photos/visibility.png" }
                                ListElement { label: "Wind :"; value: "5.2 m/s"; icon: "photos/wind.png" }
                                ListElement { label: "Rain :"; value: "0.5 mm"; icon: "photos/rainicon2.png" }
                            }

                            delegate: RowLayout {
                                spacing: 10

                                Image {
                                    source: model.icon
                                    sourceSize.width: 24
                                    sourceSize.height: 24
                                    Layout.alignment: Qt.AlignVCenter
                                }

                                Column {
                                    Text {
                                        text: model.label
                                        font.pixelSize: 14
                                        color: "black"
                                    }
                                    Text {
                                        text: model.value
                                        font.pixelSize: 18
                                        font.weight: Font.Bold
                                    }
                                }
                                ParallelAnimation {
                                    running: true
                                    NumberAnimation { target: parent; property: "opacity"; from: 0; to: 1; duration: 500; easing.type: Easing.InOutQuad }
                                    NumberAnimation { target: parent; property: "scale"; from: 0.8; to: 1; duration: 500; easing.type: Easing.InOutQuad }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //--------------------------------------------------------MAP------------------------------------------------------------------
        Item {
                anchors.fill: parent
                anchors.margins: 20

                GridLayout {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width * 0.4
                    columns: 2
                    rowSpacing: 200
                    columnSpacing: 200
                    anchors.leftMargin: 1000
                    anchors.topMargin: 125

                    Rectangle {
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        height: 500
                        color: "transparent"//boxColor4
                        opacity: 0.7
                        radius: 10

                        Item {
                            anchors.fill: parent
                            id: mapContainer
                            Map {
                                id: map
                                anchors.fill: parent
                                plugin: Plugin {
                                    name: "osm"
                                }
                                center: QtPositioning.coordinate(27.7172, 85.3240) // Coordinates for Kathmandu
                                zoomLevel: 13 // Adjust this value to set the initial zoom level

                                Button {
                                    id: expandMapButton
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    width: 30
                                    height: 30
                                    text: "⛶"
                                    onClicked: {
                                        var component = Qt.createComponent("Newtab.qml");
                                        if (component.status === Component.Ready) {
                                            var newWindow = component.createObject(main, {"latitude": map.center.latitude, "longitude": map.center.longitude});
                                            newWindow.show();
                                        } else {
                                            console.error("Error loading newTab.qml:", component.errorString());
                                        }
                                    }
                                }

                                Button {
                                    id: zoomInButton
                                    anchors.top: expandMapButton.bottom
                                    anchors.left: parent.left
                                    width: 30
                                    height: 30
                                    text: "+"
                                    onClicked: {
                                        if (map.zoomLevel < map.maximumZoomLevel) {
                                            map.zoomLevel++
                                        }
                                    }
                                }

                                Button {
                                    id: zoomOutButton
                                    anchors.top: zoomInButton.bottom
                                    anchors.left: parent.left
                                    width: 30
                                    height: 30
                                    text: "-"
                                    onClicked: {
                                        if (map.zoomLevel > map.minimumZoomLevel) {
                                            map.zoomLevel--
                                        }
                                    }
                                }
                            }

                            Rectangle
                            {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.margins: 10
                                width: contentColumn.width + 20
                                height: contentColumn.height + 20
                                color: "white"
                                opacity: 0.7
                                radius: 5


                        }
                        }
                    }
                }
            }
    //-----------------------------------------------------LATITUDE/LONGITUDE-----------------------------------------------------------
    Item{
        id: latitudelongitude;
        anchors.fill: parent
        anchors.margins: 20

        GridLayout {
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width * 0.4
            columns: 2
            rowSpacing: 200
            columnSpacing: 200
            anchors.leftMargin: 1000    //kati left ma xa
            anchors.topMargin: 630

            Rectangle {
                id: coordrectangle
                Layout.columnSpan: 2
                Layout.fillWidth: true
                height: 60
                color: "transparent"
                opacity: 0.8
                radius: 15
                anchors.topMargin: parent.topMargin
                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                     Item { Layout.fillWidth: true } // Spacer

                    Image {
                            source: "photos/maps-and-flags.png"
                            sourceSize.width: 24
                            sourceSize.height: 24
                            Layout.alignment: Qt.AlignVCenter
                        }

                    Text {
                        id: latText;
                        text: `${weatherForecast.get_latitude("Kathmandu")} | ${weatherForecast.get_longitude("Kathmandu")}`
                        font.pixelSize: 24
                        font.weight: Font.Bold
                        color: "#333333"
                        Layout.alignment: Qt.AlignVCenter
                    }

                    Item { Layout.fillWidth: true }

                }
            }
        }

    }
    //---------------------------------------------------------DATETIME---------------------------------------------------
       Item {
           id: date
           anchors.fill: parent
           anchors.margins: 20

           GridLayout {
               anchors.left: parent.left
               anchors.top: parent.top
               width: parent.width * 0.2
               columns: 2
               rowSpacing: 400
               columnSpacing: 200
               anchors.leftMargin: 1450
               anchors.topMargin: 780

               Rectangle {
                   Layout.columnSpan: 2
                   Layout.fillWidth: true
                   height: 60
                   color: "transparent"
                   opacity: 0.8
                   radius: 10

                   ColumnLayout {
                       anchors.fill: parent
                       anchors.margins: 20
                       spacing: 5
                       anchors.topMargin: 10

                       Text {
                           id: timeText
                           font.pixelSize: 20
                           font.weight: Font.Bold
                           color: "white"
                           anchors.horizontalCenter: parent.horizontalCenter
                       }

                       Text {
                             id: dateText
                             font.pixelSize: 20
                             font.weight: Font.Bold
                             color: "white"
                             anchors.horizontalCenter: parent.horizontalCenter
                         }
                   }
               }
           }

           Timer {
               interval: 1000 // Update every second
               running: true
               repeat: true
               triggeredOnStart: true
               onTriggered: {
                   var now = new Date()
                   // Adjust for Nepal time (UTC+5:45)
                   now.setHours(now.getUTCHours() + 5)
                   now.setMinutes(now.getUTCMinutes() + 45)
                   timeText.text = Qt.formatDateTime(now, "hh:mm:ss")
               }
           }
       }


    //-------------------------------------------------CONECTION--------------------------------------------------------
    Connections {
       target: weatherForecast
       function onTemperatureHourlyData(temperature, index) {
           console.log("Received temperature:", temperature, "for index:", index);
           var item = hourlyreapeter.itemAt(index);
           if(item) {
               item.children[0].children[2].text = temperature + "°C";
           }
       }

      function oniconhourlyData(icon, index)
      {
          console.log("Recieved icon::", icon, "for index:", main.icon_index);
          var item = hourlyreapeter.itemAt(main.icon_index)

          if(item)
          {
            item.children[0].children[1].source = "https://openweathermap.org/img/wn/" + icon + "@2x.png";
          }

          main.icon_index++;
          if(main.icon_index > 10)
          {
              main.icon_index = 0;
          }

      }

      function onhumidityData(humidity)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Humidity :") {
                 weatherMatrix.model.setProperty(i, "value", `${humidity}%`);
                 break;
             }
         }
      }

      function oncloudinessData(clouds)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Clouds :") {
                 weatherMatrix.model.setProperty(i, "value", `${clouds}%`);
                 break;
             }
         }
      }

      function onpressureData(pressure)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Pressure :") {
                 weatherMatrix.model.setProperty(i, "value", `${pressure} hPa`);
                 break;
             }
         }
      }

      function onwindData(wind)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Wind :") {
                 weatherMatrix.model.setProperty(i, "value", `${wind} m/s`);
                 break;
             }
         }
      }

      function onrainData(rain)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Rain :") {
                 weatherMatrix.model.setProperty(i, "value", `${rain} mm`);
                 break;
             }
         }
      }

      function onvisibilityData(visibility)
      {
          for (var i = 0; i < weatherMatrix.model.count; i++) {
             if (weatherMatrix.model.get(i).label === "Visibility :") {
                 weatherMatrix.model.setProperty(i, "value", `${visibility} km`);
                 break;
             }
         }
      }

      function ondateTimeData(datetime)
       {
         console.log(date.children[0].children[0].children[0].children[0]);
         date.children[0].children[0].children[0].children[1].text = datetime;
       }

   }

    Component.onCompleted: {
      //weatherForecast.weatherInfoUpdated("Test data");
        weatherForecast.get_temperature_hourly(main.latitude, main.longitude, 11);
        weatherForecast.get_current_weather(main.latitude,main.longitude);
    }
}
