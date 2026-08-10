import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import conveyor.datetime 1.0
import conveyor.serialcom 1.0
import conveyor.navigation 1.0

Item {
    id: root
    ColumnLayout {
        anchors.fill: parent
        spacing: 1

        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/home.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 0
            onClicked: NavigationMenuSection.section = 0
            Text {
                text: "Home"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/warning.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 1
            onClicked: NavigationMenuSection.section = 1
            Text {
                text: "Alarms"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/settings.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 2
            onClicked: NavigationMenuSection.section = 2
            Text {
                text: "Settings"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/service.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 3
            onClicked: NavigationMenuSection.section = 3
            Text {
                text: "Service"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/reset.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 4
            onClicked: NavigationMenuSection.section = 4
            Text {
                text: "Reset"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/play.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 5
            Text {
                id: start_stop_text
                text: "Start"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }
            onClicked: {
                icon.source = icon.source == "../assets/icons/play.png" ? "../assets/icons/pause.png" : "../assets/icons/play.png";
                start_stop_text.text = icon.source == "../assets/icons/play.png" ? "Start" : "Stop";
                NavigationMenuSection.section = 5
            }
        }
        Button {
            Layout.fillWidth: true
            Layout.fillHeight: true
            icon.source: "../assets/icons/test.png"
            icon.width: icon_size
            icon.height: icon_size
            highlighted: NavigationMenuSection.section == 6
            Text {
                id: test_button_text
                text: "Test"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: image_text_dist
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
            }

            onClicked: {
                NavigationMenuSection.section = 6
                SerialComm.writeBytes()
            }
        }
    }
}
