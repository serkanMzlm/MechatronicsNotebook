import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import conveyor.datetime 1.0
import conveyor.serialcom 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Conveyor")
    visibility: Window.FullScreen
    color: "gray"

    property color top_toolbar_base_color: "white"
    property int image_text_dist: 10
    property int icon_size: width / 25
    property string selectedLabel: "Home"

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

    Rectangle {
        id: menu_panel
        color: "gray"
        width: parent.width - navigation_panel.width
        height: navigation_panel.height
        anchors {
            top: top_toolbar.bottom
            right: navigation_panel.left
            left: parent.left
        }
        visible: selectedLabel == "Test"
        TestModeMenu{
            anchors.fill: parent
        }
    }
}

