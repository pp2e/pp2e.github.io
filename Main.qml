import QtQuick
import QtQuick.Controls
import Qt.labs.platform as Labs

import PowerPoint2 as PP2

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        anchors.fill: parent
        color: "#333333"
    }

    PP2.Backend {
        id: backend

        onAllDone: loader.setSource("/presentation/Krim/SlideDeck.qml")
    }

    MenuBar {
        id: menuBar

        y: backend.fullscreen ? -height : 0

        Menu {
            title: qsTr("File")
            Action { text: qsTr("&New...") }
            Action {
                text: qsTr("&Open...")
                onTriggered: backend.loadPresentationFromFile()
            }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                onTriggered: Qt.quit()
            }
            Action {
                text: qsTr("&Fullscreen")
                onTriggered: backend.fullscreen = !backend.fullscreen
            }
        }
        Menu {
            title: qsTr("Edit")
        }
        Menu {
            title: qsTr("View")
        }
        Menu {
            title: qsTr("Help")
        }
    }

    Loader {
        id: loader
        anchors.top: menuBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
