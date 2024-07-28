import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15


ApplicationWindow {
    id: main
    visible: true
     width: 800
    height: 600
    title: "Weather Forecast"
   // property int activeIndex: 1                  //TO INDICATE WHERE WE ARE VIEWING

    property real latitude: weatherForecast.get_latitude("Kathmandu")
    property real longitude: weatherForecast.get_longitude("Kathmandu")
    property real icon_index: 0
    // property real temperature: weatherForecast.get_temperature_hourly(main.latitude, main.longitude, 11);
    // property var temperatures: weatherForecast.get_temperature_hourly(main.latitude, main.longitude, 11);




    AnimatedImage {
        source: "photos/bg1.jpg"
        anchors.fill: parent                                                                  // BACKGROUND FOR MAIN WINDOW
        fillMode: Image.PreserveAspectCrop
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


            // Button {
            //         width: 25
            //         height: 25
            //         anchors.centerIn: parent

            //         contentItem: Image {
            //             source: "/Coding/c++/git/desing/I-II-Project-/Project Pic src/drawer.png"
            //             anchors.centerIn: parent
            //             fillMode: Image.PreserveAspectFit
            //         }
            //     }



            // Search Box
            // Search Box
                            Rectangle {
                                width: 200
                                height: 30
                                color: "#2e3256"  // Background color
                                radius: 5
                                Layout.alignment : Qt.AlignRight
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter


                                property string searchText:""

                                Row {
                                    anchors.fill: parent
                                    spacing: 5

                                    TextField {
                                        id: searchField
                                        placeholderText: "Search"
                                        color: "white"

                                        background: Rectangle {
                                            color: "transparent"
                                        }

                                        anchors.left: parent.left
                                        anchors.right: searchButton.left
                                        anchors.verticalCenter: parent.verticalCenter
                                        leftPadding: 5
                                        rightPadding: 5
                                        padding: 5
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
                                                fillMode: Image.PreserveAspectFit
                                                anchors.centerIn: parent
                                             }

                                            onClicked: {
                                                var latitude = weatherForecast.get_latitude(searchField.text);
                                                var longitude = weatherForecast.get_longitude(searchField.text);
                                                latitudelongitude.children[0].children[0].children[0].children[0].text = `${latitude} and ${longitude}`;
                                                maincontent.children[0].children[0].children[0].children[0].children[0].text = weatherForecast.getCity(searchField.text);
                                                maincontent.children[0].children[0].children[0].children[0].children[1].text = weatherForecast.get_weather(latitude, longitude);
                                                maincontent.children[0].children[0].children[0].children[3].text = `${weatherForecast.get_temperature(latitude, longitude)} °C`;
                                                maincontent.children[0].children[0].children[0].children[2].source = weatherForecast.get_icon(latitude, longitude);
                                                //

                                                // console.log(maincontent.children[0].children[1].children[0].children[1].children[0].children[0].children[2])
                                                weatherForecast.get_temperature_hourly(latitude, longitude, 11);
                                                // var temp = weatherForecast.get_temperature_hourly_data(10);
                                                // console.log(temp);
                                            }
                                         }
                                    }
                                }
        }
    }






    /* Blue indicator
                Rectangle {
                    width: 10
                    height: 50
                    color: "blue"
                    anchors {
                        left: parent.left
                        leftMargin: -10                                            DRAWER MA BLUE ENABLE KO LAGI KUN PAGE MA XA BHANERA
                        top: parent.children[activeIndex].top
                    }
                }


            // Placeholder for the content next to the side menu
            Item {
                width: parent.width - 150
                height: parent.height
            }*/


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

    Button {
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
            //console.log("Searched Text: ", homeField.text)
            console.log("function call")
            stackView.push("Newtab.qml")//function call
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
                   color: boxColor1
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
                           source: "/Coding/c++/git/desing/I-II-Project-/Project Pic src/cloudy.png";
                           // source: weatherForecast.get_icon(weatherForecast.get_latitude("Kathmandu"), weatherForecast.get_longitude("Kathmandu"));
                           fillMode: Image.PreserveAspectFit
                           Layout.preferredHeight: 90
                           Layout.preferredWidth: 90
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


               // // Weather Conditions
               // Rectangle {
               //     Layout.fillWidth: true
               //     height: 200
               //     color: "white"//boxColor2
               //     opacity: 0.8
               //     radius: 10

               //     ColumnLayout {
               //         anchors.fill: parent
               //         anchors.margins: 10
               //         spacing: 10

               //         Text {
               //             text: "Weather Conditions"
               //             font.pixelSize: 16
               //             font.weight: Font.Bold
               //             color: "black"
               //         }

               //         RowLayout {
               //             spacing: 20

               //             Column {
               //                 Text {
               //                     text: "☁️"
               //                     font.pixelSize: 40
               //                     anchors.horizontalCenter: parent.horizontalCenter
               //                 }
               //                 Text {
               //                     text: "Cloudy"
               //                     color: "black"
               //                     anchors.horizontalCenter: parent.horizontalCenter
               //                 }
               //             }

               //             Column {
               //                 Text {
               //                     text: "🌧️"
               //                     font.pixelSize: 40
               //                     anchors.horizontalCenter: parent.horizontalCenter
               //                 }
               //                 Text {
               //                     text: "Rainy"
               //                     color: "black"
               //                     anchors.horizontalCenter: parent.horizontalCenter
               //                 }
               //             }

               //             Column {
               //                 // Image {
               //                 //                                         source:"/home/sryn/Pictures/Project Pic src/cloudy.png"
               //                 //                                         width: 30
               //                 //                                         height: 30
               //                 //                                         anchors.horizontalCenter: parent.horizontalCenter
               //                 //                                     }
               //                 Text {
               //                  text: "☀️"
               //                  font.pixelSize: 40
               //                  anchors.horizontalCenter: parent.horizontalCenter
               //                  }
               //                 Text {
               //                     text: "Sunny"
               //                     color: "black"
               //                     anchors.horizontalCenter: parent.horizontalCenter
               //                 }
               //             }
               //         }
               //     }
               // }

               // Hourly Forecast
               Rectangle {
                   id:hourlyForecast
                   Layout.fillWidth: true
                   height: 250
                   width: parent.width
                   color: "white"//boxColor3
                   opacity: 0.8
                   radius: 10

                   ColumnLayout {
                       anchors.fill: parent
                       anchors.margins: 10

                       Text {
                           text: "Hourly Forecast"
                           font.pixelSize: 16
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
                                   height: 120
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
                                       // Text {
                                       //     text: model.index % 2 ? "☁️" : "🌧️"
                                       //     font.pixelSize: 24
                                       //     anchors.horizontalCenter: parent.horizontalCenter
                                       // }
                                       Image{
                                           source: "/Coding/c++/git/desing/I-II-Project-/Project Pic src/cloudy.png";
                                           fillMode: Image.PreserveAspectFit
                                           anchors.horizontalCenter: parent.horizontalCenter
                                           width: 45
                                           height: 45
                                        }
                                       Text {
                                           id: tempText
                                           text: "";
                                           font.pixelSize: 15
                                           // text: "23 C"
                                           color: "black"
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                   }
                               }

                               // Connections {
                               //     target: weatherForecast
                               //     onTemperatureHourlyData: {
                               //         // hourlyForecast.children[0].children[0].children[0].children[0].children[0].children[3].text = "Temperature: " + temperature + " °C"
                               //         tempText.text = "Temperature: " + temperature + " °C";
                               //     }
                               // }
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

            // New box for Humidity, Air Pressure, and Rainometer
            Rectangle {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                height: 250
                color: "white"//boxColor4
                opacity: 0.8
                radius: 10
                anchors.topMargin: parent.topMargin
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    anchors.topMargin: -20

                    Text {
                        text: "Weather Metrics"
                        font.pixelSize: 30
                        font.weight: Font.Bold
                        color: "black"
                    }

                    RowLayout {
                                Layout.fillWidth: true
                                spacing: 160


                                Repeater {
                                   // anchors.topMargin: -30

                                    model: [
                                        {label: "Humidity", value: "65%"},
                                        {label: "Air Pressure", value: "1013 hPa"},
                                        {label: "Rainometer", value: "2 mm"}
                                    ]

                                    delegate: ColumnLayout {
                                        spacing: 5

                                        Text {
                                            text: modelData.label
                                            font.pixelSize: 20
                                            color: "black"
                                            Layout.alignment: Qt.AlignLeft
                                        }

                                        Text {
                                            text: modelData.value
                                            font.pixelSize: 18
                                            font.weight: Font.Medium
                                            color: "#34495e"
                                            Layout.alignment: Qt.AlignLeft
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
                color: "white"//boxColor4
                opacity: 0.8
                radius: 10

                Item {
                    anchors.fill: parent

                    // Image {
                    //     // source: "photos/map.png"
                    //     source: "file:///C:/Coding/c++/New%20folder/vector-world-map.svg"
                    //     // source: "http://maps.openweathermap.org/maps/2.0/weather/TA2/0/0/0?appid=01b70166b923c25c030d53acdc92e93d&fill_bound=true&opacity=0.6&0:E1C86400; 0.1:C8963200; 0.2:9696AA00; 0.5:7878BE00; 1:6E6ECD4C; 10:5050E1B2; 140:1414FFE5"
                    //     anchors.fill: parent
                    //     fillMode: Image.PreserveAspectCrop
                    // }

                    Image {
                        source: "photos/map.png"
                        // source: "http://maps.openweathermap.org/maps/2.0/weather/TA2/0/0/0?appid=01b70166b923c25c030d53acdc92e93d"
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop
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

                       /* Column {
                            id: contentColumn
                            anchors.centerIn: parent
                            spacing: 5

                            Text {
                                text: "Kathmandu"
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                color: "black"
                            }

                            Text {
                                text: "Partly Cloudy"
                                font.pixelSize: 18
                                color: "black"
                            }
                        }
                    }

                    Text {
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: 10
                        text: "25°C
                        font.pixelSize: 30
                        font.weight: Font.Bold
                        color: "white"
                        style: Text.Outline
                        styleColor: "black"
                    }*/
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
                color: "#34495e"
                //opacity: 0.8
                radius: 10
                anchors.topMargin: parent.topMargin
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    anchors.topMargin: 10

                    Text {
                        id: latText;
                        text: `${weatherForecast.get_latitude("Kathmandu")} and ${weatherForecast.get_longitude("Kathmandu")}`
                        font.pixelSize: 30
                        font.weight: Font.Bold
                        color: "#333333"
                    }

                }
            }
        }

    }
    //---------------------------------------------------------POPULATION---------------------------------------------------
    Item{
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
            anchors.topMargin: 695                              //top  bata kati tala xa

            Rectangle {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                height: 60
                color:"#34495e"
                //opacity: 0.8
                radius: 10
                anchors.topMargin: parent.topMargin
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    anchors.topMargin: 10

                    Text {
                        text: "Population"
                        font.pixelSize: 30
                        font.weight: Font.Bold
                        color: "#333333"
                    }

                }
            }
        }

    }
    //----------------------------------------------------AIR QUALITY INDEX----------------------------------------
    Item{
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
            anchors.topMargin: 760                                //top  bata kati tala xa

            Rectangle {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                height: 60
                color: "#34495e"
               // opacity: 0.8
                radius: 10
                anchors.topMargin: parent.topMargin
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 10
                    anchors.topMargin: 10

                    Text {
                        text: "Air Quality Index"
                        font.pixelSize: 30
                        font.weight: Font.Bold
                        color: "#333333"
                    }

                }
            }
        }

    }
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
   }

    // Component.onCompleted: {
    //     for (var i = 0; i < 11; i++) {
    //                 weatherForecast.get_temperature_hourly(main.latitude, main.longitude);
    //             }
    // }


}

