import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import conveyor.serialcom 1.0

Item {
    id: root
    property bool isPlayingTest: true
    Rectangle {
        id: start_button
        width: menu_panel.width / 10
        height: width / 2
        radius: height / 4
        color: "darkgreen"
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 20
        }

        Text {
            id: icon_text
            text: "Start"
            anchors {
                centerIn: start_button
            }

            color: "white"
            font.bold: true
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            onClicked: {
                if(isPlayingTest) {
                    isPlayingTest = false
                    icon_text.text = "Stop"
                    start_button.color = "darkred"
                    SerialComm.writeBytes()
                }
                else {
                    isPlayingTest = true
                    icon_text.text = "Start"
                    SerialComm.writeBytes(21)
                    start_button.color = "darkgreen"
                }

            }
            onPressed:
            {
                start_button.scale = 0.95
            }
            onReleased:
            {
                start_button.scale = 1
            }
        }
    }

    Text {
        id: serial_error_text
        text: "Error: " + SerialComm.error
        font.pointSize: 20
        color: "darkred"
    }

    Text {
        id: serial_info_text
        text: "Debug: " + SerialComm.info
        font.pointSize: serial_error_text.font.pointSize
        color: "darkgreen"
        anchors {
            top: serial_error_text.bottom
        }
    }

    Grid {
        anchors.centerIn: parent
        rows: 3
        columns: 4
        spacing: parent.width / 12

        Repeater {
            model: 12
            Button {
                text: "Button " + (index + 1)
                width: menu_panel.width / 10
                height: width / 2
                onClicked: {
                    SerialComm.writeBytes(index + 1)
                }
            }
        }
    }

}
