import QtQuick 2.15
import QtQuick.Window 2.15

Rectangle{
    width: _Size.width
    height: _Size.height
    Text{
        id : text_1
        text: "Name: " + _String;
        font.pointSize: 20
        anchors.centerIn: parent

         MouseArea
         {  anchors.fill:parent
             onReleased: text_1.color = "red"
             onPressed: text_1.color = "blue"
         }
    }
}
