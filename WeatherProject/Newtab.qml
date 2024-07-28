
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Weather Forecast"
   // property int activeIndex: 1                  //TO INDICATE WHERE WE ARE VIEWING






    AnimatedImage {
        source: "photos/weathervid.gif"
        anchors.fill: parent                                                                  // BACKGROUND FOR MAIN WINDOW
        fillMode: Image.PreserveAspectCrop
    }
}
