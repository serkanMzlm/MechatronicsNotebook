import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import conveyor.datetime 1.0

Item {
    id: root
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
            font.pixelSize: Math.max(parent.width * 0.75, parent.height * 0.75)
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
        text: DateTime.calender
        font.bold: true
        font.pointSize: Math.max(parent.width * 0.01, 1)
        color: "#4B0082"
        anchors {
            left: parent.left
            leftMargin: parent.width * 0.01
            verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: clock_text
        text: DateTime.clock
        font.bold: true
        font.pointSize: Math.max(parent.width * 0.01, 1)
        color: "#4B0082"
        anchors {
            centerIn: parent
        }
    }
}
