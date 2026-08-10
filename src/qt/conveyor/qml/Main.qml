import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import conveyor.datetime 1.0
import conveyor.serialcom 1.0

Window {
    id: root
    visible: true
    title: qsTr("Conveyor App")
    color: "gray"

    height: 480
    width: 680
    visibility: Window.FullScreen

    property color top_toolbar_base_color: "white"
    property int image_text_dist: 10
    property int icon_size: 80

    Rectangle {
        id: top_toolbar
        color: top_toolbar_base_color
        height: Math.max(parent.width * 0.03, parent.height * 0.03)
        width: parent.width

        anchors {
            top: parent.top
            left: parent.left
        }

        TopToolbar{
            anchors.fill: parent
        }
    }

    Rectangle {
        id: navigation_panel
        width: parent.width / 10
        height: parent.height - top_toolbar.height
        color: "lightgray"

        anchors {
            top: top_toolbar.bottom
            right: parent.right
        }

        NavigationMenu{
            anchors.fill: parent
        }
    }
}
