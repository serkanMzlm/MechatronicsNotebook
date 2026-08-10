import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import conveyor.datetime 1.0
import conveyor.serialcom 1.0

Item {
    id: root
    property color navigation_block_color: "#444A4A"
    property var navItems: [
        { "label": "Home", "iconSource": "../assets/icons/home.png" },
        { "label": "Alarms", "iconSource": "../assets/icons/warning.png" },
        { "label": "Settings", "iconSource": "../assets/icons/settings.png" },
        { "label": "Service", "iconSource": "../assets/icons/service.png" },
        { "label": "Reset", "iconSource": "../assets/icons/reset.png" },
        { "label": "Start", "iconSource": "../assets/icons/play.png" },
        { "label": "Test", "iconSource": "../assets/icons/test.png" }
    ]

    property bool isPlaying: false
    property int selectedIndex: 0

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Repeater {
            model: navItems

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: root.selectedIndex === index ? "#00AFFF" : navigation_block_color

                Image {
                    id: icon_image
                    source: modelData.iconSource !== undefined && modelData.iconSource !== ""
                            ? modelData.iconSource
                            : "default_icon.png"
                    anchors.centerIn: parent
                    width: icon_size
                    height: icon_size
                }

                Text {
                    id: icon_text
                    text: modelData.label !== undefined && modelData.label !== ""
                          ? modelData.label
                          : "Default Label"
                    anchors.top: parent.children[0].bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: 10
                    color: "white"
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.selectedIndex = index
                        if (modelData.label == "Start" && !isPlaying){
                            icon_image.source = "../assets/icons/pause.png";
                            isPlaying = true;
                            icon_text.text = "Pause"
                        }else if(modelData.label == "Start" && isPlaying){
                            icon_image.source =  modelData.iconSource;
                            isPlaying = false;
                            icon_text.text = modelData.label
                        }
                        selectedLabel =  modelData.label
                    }
                }
            }
        }
    }
}
