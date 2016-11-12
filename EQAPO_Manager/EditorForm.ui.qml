import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: root
    width: 1050
    height: 449

    property alias repeater: repeater

    Item {
        id: levels
        width: 45
        anchors.top: editor.top
        anchors.bottom: editor.bottom
        anchors.left: parent.left
        anchors.leftMargin: 5

        Label {
            text: qsTr("20 dB")
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
        }

        Label {
            width: 40
            text: qsTr("0 dB")
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 3
            anchors.left: parent.left
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
        }

        Label {
            width: 40
            text: qsTr("-20 dB")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 45
            anchors.left: parent.left
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
        }
    }

    ColumnLayout {
        id: editor
        anchors.left: levels.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5
        anchors.rightMargin: 10

        Label {
            text: qsTr("Level (dB)")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                Layout.fillHeight: true

                Repeater {
                    id: repeater

                    property bool isUpdating: false

                    EditorDelegate {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    }
                }
            }
        }

        Label {
            text: qsTr("Frequency (Hz)")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }
}
