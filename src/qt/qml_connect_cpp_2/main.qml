import QtQuick 2.15
import QtQuick.Window 2.15

Rectangle {
    width: 400
    height: 250
    visible: true
    Rectangle{
        width: 200; height: 125
        color: parseInt(_user.count)%2 == 0 ? "gray" : "black"
        radius:  20
        anchors.centerIn : parent
        Text {
            id: name
            anchors.centerIn: parent
            text: _user.count
            font.pointSize: 20
            color: "lightgray"
        }
    }
}
