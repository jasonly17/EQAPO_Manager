import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import "."

ApplicationWindow {
    id: root
    visible: showWindow
    width: 1050
    height: 550
    title: qsTr("EQAPO Manager")
    color: Styles.primaryColor

    Connections {
        target: SingleApplication
        onBringToFront: {
            root.flags += Qt.WindowStaysOnTopHint;
            root.flags -= Qt.WindowStaysOnTopHint;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.bottomMargin: 10
        spacing: 0

        EQControl {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Rectangle {
            color: Styles.tertiaryColor
            height: 1
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
        }

        Preset {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.bottomMargin: 5
        }

        Editor {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }
}
