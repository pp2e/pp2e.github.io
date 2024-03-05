import QtQuick
import QtQuick.Controls
import Qt.labs.platform as Labs
import QtQuick.Dialogs

import PowerPoint2 as PP2

import QtQuick.Particles

PP2.Window {
    id: window
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        anchors.fill: parent
        color: "#06070D"
    }

    PP2.Backend {
        id: backend

        onAllDone: loader.setSource("/presentation/Krim/SlideDeck.qml")
        // onFullscreenChanged: console.log("screen")
    }

    FileDialog {
        id: fileDialog
    }

    MenuBar {
        id: menuBar

        y: window.fullscreen ? -height : 0
        width: parent.width

        Menu {
            title: qsTr("File")
            Action { text: qsTr("&New...") }
            Action {
                text: qsTr("&Open...")
                onTriggered: backend.loadPresentationFromFile()
            }
            Action {
                text: "Open2"
                onTriggered: fileDialog.open()
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
                onTriggered: window.fullscreen = !window.fullscreen
            }
            // Action {
            //     text: qsTr("Full")
            //     onTriggered: {
            //         console.log(window.visibility)
            //         window.visibility = window.visibility == Window.FullScreen ? Window.Windowed : Window.FullScreen
            //     }
            // }
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

    Rectangle {
        color: "red"
        width: 10
        height: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    // Video {
    //     id: video
    //     anchors.fill: parent

    //     source: "https://www.sample-videos.com/video321/mp4/720/big_buck_bunny_720p_1mb.mp4"

    //     onErrorOccurred: console.log("error", errorString)

    //     MouseArea {
    //         anchors.fill: parent
    //         onClicked: {
    //             video.play()
    //             console.log("video")
    //         }
    //     }
    // }
}
