import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import "."

ComboBox {
    id: presetComboBox
    font.pointSize: 10
    topPadding: 0
    bottomPadding: 0
    currentIndex: -1
    Layout.preferredHeight: 25
    Layout.preferredWidth: 300

    delegate: ItemDelegate {
        width: presetComboBox.width
        height: presetComboBox.height
        text: modelData
        highlighted: presetComboBox.highlightedIndex == index

        contentItem: Text {
            anchors.fill: parent
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            text: parent.text
            font.pointSize: 10
            color: presetComboBox.currentIndex == index ? Styles.secondaryColor : Styles.primaryTextColor
            verticalAlignment: Text.AlignVCenter
        }
    }
}
