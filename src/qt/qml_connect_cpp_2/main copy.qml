import QtQuick 2.15
import QtQuick.Window 2.15

Rectangle {
    width: 640
    height: 480
    visible: true
//    title: qsTr("Hello World")
    Rectangle{

        width: 320; height: 240
        color: "red"
        anchors.centerIn : parent
        Text {
            id: name
            anchors.centerIn: parent
            text: _user.name
            font.pointSize: 20
        }
    }
}
