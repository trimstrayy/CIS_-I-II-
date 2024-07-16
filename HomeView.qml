import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Weather Forecast"
   // property int activeIndex: 1                  //TO INDICATE WHERE WE ARE VIEWING


    Image {
        source: "/home/sryn/Downloads/bg1.jpg"
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
                    source: "/home/sryn/Pictures/demo/drawer.png"  // Drawer icon
                    width: 33
                    height: 32
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
                font.pixelSize: 20
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
                width: 200
                height: 30
                color: "#2e3256"  // Background color
                radius: 5
                Layout.alignment: Qt.AlignVCenter

                Image {
                    source: "/home/sryn/Pictures/demo/search.png"
                    width: 20
                    height: 20
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                }

                TextField {
                    placeholderText: "Search"
                    color: "white"
                    background: Rectangle {
                        color: "transparent"
                    }
                    font.pixelSize: 14
                    anchors.fill: parent
                    anchors.rightMargin: 38  // Adjust the right margin to avoid overlap
                    padding: 5
                }
            }
        }
    }

    // Drawer for the navigation bar
    Drawer {
        id: navDrawer
        width: 60
        height: parent.height
        edge: Qt.LeftEdge
        interactive: true

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#2e2e2e" //makes border transparent
            ColumnLayout {
                anchors.fill: parent
                spacing: 10 // Reduced spacing between items
               anchors.margins: 10

                // ------------------------------------------------ERROR-------------------------------------------------------------------
                ItemDelegate {
                          Layout.fillWidth: true
                          Layout.preferredHeight: 3

                          Rectangle {
                              anchors.fill: parent
                              color:"transparent"
                              border.color:"transparent"
                              border.width:0
                              radius: 5


                              Image {                                                          //drawer button inside drawer ko lagi
                                  source: "/home/sryn/Pictures/demo/drawer.png"
                                  width: 37
                                  height: 37
                                  fillMode: Image.PreserveAspectFit
                                  anchors.verticalCenter:parent.verticalCenter

                                  MouseArea{
                                      anchors.fill: parent
                                  }
                              }
                          }


                      }

                ItemDelegate {
                          Layout.fillWidth: true
                          Layout.preferredHeight: 30

                          Rectangle {
                              anchors.fill: parent
                              color:"transparent"
                              border.color:"transparent"
                              border.width:1
                              radius: 5


                              Image {                                                          //home button inside drawer ko lagi
                                  source: "/home/sryn/Pictures/demo/home.png"
                                  width: 35
                                  height: 35
                                  fillMode: Image.PreserveAspectFit
                                  anchors.verticalCenter:parent.verticalCenter

                                  MouseArea{
                                      anchors.fill: parent
                                      //id:mapbutton
                                     // onClicked :{ activeIndex=0}             BLUE DEKHUANA DRAWER MA
                                  }
                              }
                          }



                      }

                    //--------------------------------------------------------------------------------------------------------------


                Text {
                    text: "Menu"
                    color: "white"
                    font.pixelSize: 20
                }

                Text {
                    text: "Map"
                    color: "white"
                    font.pixelSize: 20
                }

                Text {
                    text: ""
                    color: "white"
                    font.pixelSize: 20
                }
            }
        }

        // Close drawer on mouse leave
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onExited: {
                navDrawer.close()
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


    //MAIN CONTENT
    Item {
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
                   Layout.columnSpan: 2
                   Layout.fillWidth: true
                   height: 150
                   color: boxColor1
                   opacity: 0.8
                   radius: 10

                   RowLayout {
                       anchors.fill: parent
                       anchors.margins: 20
                       spacing: 20

                       Column {
                           Layout.fillHeight: true
                           Text {
                               text: "Kathmandu"
                               font.pixelSize: 30
                               font.weight: Font.Bold
                               color: "black"
                           }
                           Text {
                               text: "Partly Cloudy"
                               font.pixelSize: 18
                               color: "black"
                           }
                       }

                       Item { Layout.fillWidth: true }

                       Text {
                           text: "⛅"  // Unicode for partly cloudy
                           font.pixelSize: 64
                       }

                       Text {
                           text: "25°C"
                           font.pixelSize: 48
                           color: "black"
                       }
                   }
               }

               // Weather Conditions
               Rectangle {
                   Layout.fillWidth: true
                   height: 200
                   color: boxColor2
                   opacity: 0.8
                   radius: 10

                   ColumnLayout {
                       anchors.fill: parent
                       anchors.margins: 10
                       spacing: 10

                       Text {
                           text: "Weather Conditions"
                           font.pixelSize: 16
                           font.weight: Font.Bold
                           color: "black"
                       }

                       RowLayout {
                           spacing: 20

                           Column {
                               Text {
                                   text: "☁️"
                                   font.pixelSize: 40
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                               Text {
                                   text: "Cloudy"
                                   color: "black"
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                           }

                           Column {
                               Text {
                                   text: "🌧️"
                                   font.pixelSize: 40
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                               Text {
                                   text: "Rainy"
                                   color: "black"
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                           }

                           Column {
                               Text {
                                   text: "☀️"
                                   font.pixelSize: 40
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                               Text {
                                   text: "Sunny"
                                   color: "black"
                                   anchors.horizontalCenter: parent.horizontalCenter
                               }
                           }
                       }
                   }
               }

               // Hourly Forecast
               Rectangle {
                   Layout.fillWidth: true
                   height: 200
                   color: boxColor3
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
                               model: 6
                               Rectangle {
                                   width: 50
                                   height: 100
                                   color: "#34495e"
                                   radius: 5
                                   Column {
                                       anchors.centerIn: parent
                                       spacing: 5
                                       Text {
                                           text: (model.index + 1) + "h"
                                           color: "black"
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                       Text {
                                           text: model.index % 2 ? "☁️" : "🌧️"
                                           font.pixelSize: 24
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                       Text {
                                           text: "23°"
                                           color: "black"
                                           anchors.horizontalCenter: parent.horizontalCenter
                                       }
                                   }
                               }
                           }
                       }
                   }
               }


           }
       }
}
