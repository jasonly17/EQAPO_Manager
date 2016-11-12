import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 1050
    height: 50
    property alias preampSpinBox: preampSpinBox
    property alias preampSlider: preampSlider
    property alias powerButton: powerButton

    RowLayout {
        id: rowLayout1
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
        spacing: 5
        anchors.fill: parent

        PowerButton {
            id: powerButton
            width: 32
            height: 32
        }

        Label {
            text: qsTr("Preamp:")
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }

        Slider {
            id: preampSlider
            value: 30
            from: 0
            snapMode: Slider.SnapAlways
            to: 30
            stepSize: 1
            Layout.fillWidth: true
            spacing: 0
        }

        PlainSpinBox {
            id: preampSpinBox
            Layout.preferredWidth: 30
            Layout.preferredHeight: 26
            visible: true
        }

        Label {
            text: qsTr("dB")
            verticalAlignment: Text.AlignVCenter
        }
    }
}
