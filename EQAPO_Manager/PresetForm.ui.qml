import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    width: 620
    height: 35
    property alias deleteButton: deleteButton
    property alias saveButton: saveButton
    property alias resetButton: resetButton
    property alias presetComboBox: presetComboBox

    RowLayout {
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenterOffset: 0
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        Label {
            text: qsTr("Preset:")
            rightPadding: 5
        }

        PresetComboBox {
            id: presetComboBox
        }

        Button {
            id: resetButton
            text: qsTr("Reset")
            Layout.preferredWidth: 80
            highlighted: true
            Layout.preferredHeight: 35
        }

        Button {
            id: saveButton
            text: qsTr("Save")
            Layout.preferredWidth: 80
            highlighted: true
            Layout.preferredHeight: 35
        }

        Button {
            id: deleteButton
            text: qsTr("Delete")
            Layout.preferredWidth: 80
            highlighted: true
            Layout.preferredHeight: 35
        }
    }
}
