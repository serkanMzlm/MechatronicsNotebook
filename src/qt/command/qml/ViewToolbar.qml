import QtQuick
import QtQuick.Dialogs

import lectron.sysinfo 1.0

Rectangle {
    id: root
    width: parent.width
    height: parent.height * 0.075
    color: "white"

    Rectangle {
        id: logo_rec
        width: parent.width * 0.03
        height: parent.height
        color: parent.color

        Image {
            id: logo_image
            source: "/icons/medicine.svg"
            anchors.fill:       parent
            fillMode: Image.PreserveAspectFit
        }

        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: height / 2
        }
    }

    Rectangle{
        id: exit_rectangle
        height: parent.height * 0.75
        width: height
        radius: height / 2

        color: "red"
        anchors {
            right: parent.right
            rightMargin: parent.height * 0.125
            verticalCenter: parent.verticalCenter
        }

        Text {
            text: "X"
            font.pixelSize: Math.max(parent.width * 0.85, parent.height * 0.85)
            color: "white"
            anchors.centerIn: parent
            font.bold: true
        }

        MouseArea {
            id: exit
            anchors.fill: parent
            hoverEnabled: true
            onClicked:  Qt.quit()
        }
    }

    Text {
        id: date_text
        text: SysInfo.calender
        font.pointSize: font_size * 1.5
        color: "black"
        anchors {
            left: logo_rec.right
            leftMargin: parent.width * 0.01
            verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: clock_text
        text: SysInfo.clock
        font.pointSize: font_size * 1.5
        color: "black"
        anchors {
            left: date_text.right
            top: date_text.top
            leftMargin: 20
        }
    }

    Text {
        id: navigation_menu_text
        text: select_page
        font.bold: true
        font.pointSize: Math.max(parent.width * 0.015, 1)
        color: "#4B0082"
        anchors {
            centerIn: parent
        }
    }

}
