import QtQuick    2.15
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Window
import command 1.0

ApplicationWindow {
    id: main_window
    width: 640
    height: 480
    // visibility: Window.FullScreen
    visible: true

    title: qsTr("Command")
    color: "gray"

    property color main_window_color: "#2e2e2e"
    property int button_height: height * 0.6
    property int button_width: height * 0.15
    property int font_size: (height <= 0.01 ? 0.01 : height * 0.015)

    property color top_toolbar_base_color: "white"
    property int image_text_dist: 10
    property int icon_size: width / 25
    property string select_page: "Home"

    ViewToolbar {
        id: view_toolbar
    }

    ViewWidgetLayer {
        id: view_widget_layer
        width: parent.width / 10
        height: parent.height - view_toolbar.height
        anchors {
            top: view_toolbar.bottom
            right: parent.right
        }
    }
}
