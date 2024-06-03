import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import PowerPoint2 as PP2

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

        onAllDone: loader.setSource("/mount/presentation/Krim/SlideDeck.qml")
        // onFullscreenChanged: console.log("screen")
        Component.onCompleted: {
            printType(backend)
        }
    }

    MenuBar {
        id: menuBar

        //y: window.fullscreen ? -height : 0
        width: parent.width

        Menu {
            title: qsTr("File")
            Action { text: qsTr("&New...") }
            Action {
                text: qsTr("&Open...")
                onTriggered: backend.loadPresentationFromFile()
            }
            Action {
                text: qsTr("&Close...")
                onTriggered: backend.unloadPresentation()
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
        Menu {
            title: qsTr("Debug")
            Action {
                text: qsTr("Print all texts")
                onTriggered: Inspector.printAllTexts(loader.item)
            }
            // Action {
            //     text: qsTr("Debug teextt")
            //     onTriggered: Inspector.printBindings(teextt)
            // }
        }
    }

    SplitView {
        anchors.top: menuBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        orientation: Qt.Horizontal

        Item {
            SplitView.preferredWidth: window.fullscreen ? 0 : 200
            SplitView.minimumWidth: 200
            TabBar {
                id: leftBar
                width: parent.width
                TabButton {
                    text: qsTr("Resources")
                }
                TabButton {
                    text: qsTr("Slides")
                }
            }
            StackLayout {
                id: leftStack
                width: parent.width
                anchors.top: leftBar.bottom
                anchors.bottom: parent.bottom
                currentIndex: leftBar.currentIndex
                TreeView {
                    model: PP2.RccModel
                    selectionModel: ItemSelectionModel {}
                    delegate: TreeViewDelegate {}
                    contentWidth: width
                    clip: true
                }
                Rectangle {
                    color: window.fullscreen ? "red" : "black"
                }
            }
        }

        Loader {
            id: loader
            SplitView.fillWidth: window.fullscreen
            clip: true
        }
    }

    Rectangle {
        color: "red"
        width: 10
        height: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
