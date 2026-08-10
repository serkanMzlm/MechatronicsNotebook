import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "lightgray"

    property color navigation_block_color: "#444A4A"
    property var navItems: [
        { "label": "Home", "iconSource": "/icons/home.png", "page": "" },
        { "label": "Alarms", "iconSource": "/icons/warning.png", "page": "" },
        { "label": "Settings", "iconSource": "/icons/settings.png", "page": "" },
        { "label": "Service", "iconSource": "/icons/service.png", "page": "" },
        { "label": "Reset", "iconSource": "/icons/reset.png", "page": "" },
        { "label": "Start", "iconSource": "/icons/play.png", "page": "" },
        { "label": "Test", "iconSource": "/icons/test.png", "page": "TestPage.qml" }
    ]

    property bool is_playing: false
    property int selected_index: 0

    Loader {
        id: pageLoader
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Repeater {
            model: navItems

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: root.selected_index === index ? "#00AFFF" : navigation_block_color

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
                        root.selected_index = index
                        if (modelData.label == "Start" && !is_playing){
                            icon_image.source = "/icons/pause.png";
                            is_playing = true;
                            icon_text.text = "Pause"
                        }else if(modelData.label == "Start" && is_playing){
                            icon_image.source =  modelData.iconSource;
                            is_playing = false;
                            icon_text.text = modelData.label
                        }
                        select_page =  modelData.label
                        pageLoader.source = modelData.page
                    }
                }
            }
        }
    }
}
