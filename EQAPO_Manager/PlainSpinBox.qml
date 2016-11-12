import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "."

SpinBox {
    id: root
    width: 30
    height: 26
    from: -30
    to: 0
    editable: true
    up.indicator: Item { }
    down.indicator: Item { }

    property alias horizontalAlignment: textInput.horizontalAlignment

    background: Item {
        anchors.fill: root

        Rectangle {
            anchors.fill: parent
            color: Styles.primaryColor
            border.width: root.activeFocus ? 2 : 1
            border.color: root.activeFocus ? Styles.secondaryColor : Styles.tertiaryColor
        }
    }

    contentItem: TextInput {
        id: textInput
        height: root.height
        width: root.width
        text: root.textFromValue(root.value, root.locale)
        validator: root.validator
        font.pointSize: 10
        anchors.fill: root
        anchors.rightMargin: horizontalAlignment == Text.AlignRight ? 5 : 0
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter

        cursorDelegate: Rectangle {
            width: 2
            height: parent.height - 9
            color: Styles.secondaryColor
            visible: parent.activeFocus

            SequentialAnimation on opacity {
                running: true
                loops: Animation.Infinite
                NumberAnimation { to: 1; duration: 0 }
                PauseAnimation { duration: 750 }
                NumberAnimation { to: 0; duration: 0 }
                PauseAnimation { duration: 750 }
            }
        }
    }

    Keys.onEnterPressed: validateInput()
    Keys.onReturnPressed: validateInput()

    function validateInput() {
        root.focus = false;
    }
}
