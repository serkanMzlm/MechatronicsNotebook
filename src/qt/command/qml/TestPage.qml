import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import lectron.com 1.0

Rectangle {
    id: _root
    height: main_window.height * 0.925
    width: main_window.width * 0.9
    color:  "gray"

    anchors {
        top: parent.top
        right: parent.left
    }

    property bool is_playing_test: true
    property bool is_ready: SerialComm.isready
    property bool is_active: SerialComm.isactive

    Rectangle {
        id: consol_display
        height: parent.height * 0.95
        width: parent.width * 0.1
        anchors.verticalCenter: parent.verticalCenter
        ScrollView {
            anchors.fill: parent
            TextArea {
                id: serialDisplay
                text: ""
                font.pointSize: font_size
                wrapMode: TextEdit.Wrap
                readOnly: true  // Bu satırı ekleyebilirsiniz, eğer kullanıcı metni düzenleyememeli ise.
            }
        }
        Connections {
            target: SerialComm
            function onSerial_rawChanged() {
                serialDisplay.append(SerialComm.serial_raw)
            }
        }
    }


    Rectangle {
        id: start_button
        width: main_window.width / 12
        height: width / 2.2
        radius: height / 4
        color: is_active ? "darkgreen" : "#383838"
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
            font.pointSize: font_size * 1.5
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            onClicked: {
                if(is_active)
                {
                    if(is_playing_test) {
                        is_playing_test = false
                        icon_text.text = "Stop"
                        start_button.color = "darkred"
                        SerialComm.writeBytes()
                    }
                    else {
                        is_playing_test = true
                        icon_text.text = "Start"
                        SerialComm.writeBytes(100)
                        start_button.color = "darkgreen"
                    }
                }
            }
            onPressed: {
                if(is_active) {
                    start_button.scale = 0.95
                }
            }
            onReleased: {
                if(is_active) {
                    start_button.scale = 1
                }
            }
        }
    }

    Text {
        id: serial_info_text
        text: SerialComm.info
        font.pointSize: font_size * 2
        color: "darkgreen"
        anchors {
            top: start_button.bottom
            topMargin: parent.height * 0.02
            horizontalCenter: start_button.horizontalCenter
        }
    }

    Grid {
        anchors.centerIn: parent
        visible: is_ready
        rows: 3
        columns: 4
        spacing: parent.width / 20

        Repeater {
            model: 12
            Button {
                text: "Button " + (index + 1)
                width: main_window.width / 10
                height: width / 2
                onClicked: {
                    SerialComm.writeBytes(index + 1)
                }
            }
        }
    }
}
