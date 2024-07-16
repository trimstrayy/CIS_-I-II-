import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Weather App"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left sidebar
        Rectangle {
            Layout.preferredWidth: 150
            Layout.fillHeight: true
            color: "lightgray"

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                ToolButton {
                    text: "☰"
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    text: "Map"
                    Layout.alignment: Qt.AlignLeft
                    font.pixelSize: 16
                }

                Text {
                    text: "Detailed info"
                    Layout.alignment: Qt.AlignLeft
                    font.pixelSize: 16
                }

                Text {
                    text: "Home"
                    Layout.alignment: Qt.AlignLeft
                    font.pixelSize: 16
                }

                Text {
                    text: "Your City"
                    Layout.alignment: Qt.AlignLeft
                    font.pixelSize: 16
                }
            }
        }

        // Main content area
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            // Header
            Rectangle {
                Layout.fillWidth: true
                height: 50
                color: "white"

                RowLayout {
                    anchors.fill: parent
                    Text {
                        text: "Weather"
                        font.pixelSize: 24
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    }
                    TextField {
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        placeholderText: "Search"
                    }
                }
            }

            // Weather content
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                // Left panel
                Rectangle {
                    Layout.preferredWidth: 300
                    Layout.fillHeight: true
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Rectangle {
                            Layout.fillWidth: true
                            height: 100
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "Weather icon\n(sun to rain)"
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 50
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "Current weather (°C)"
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "Humidity:\nAir Pressure:\nRainometer:"
                            }
                        }
                    }
                }

                // Right panel
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: "Weather forecast"
                            font.pixelSize: 18
                            Layout.alignment: Qt.AlignLeft
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 100
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "Today:\n0:00  2:00  4:00  6:00  8:00\n10:00  12:00  14:00  16:00  20:00"
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "This week:\nSunday  Monday  Tuesday\nWed.  Thu.  Fri.\nSat."
                            }
                        }
                    }
                }
            }
        }
    }
}
