import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: root
    Layout.fillHeight: true
    width: 40

    ColumnLayout {
        anchors.fill: parent

        PlainSpinBox {
            id: spinBoxDelegate
            stepSize: 100
            to: 20 * 1000
            from: -20 * 1000
            value: 0
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 26
            horizontalAlignment: Text.AlignHCenter

            validator: IntValidator {
                bottom: Math.min(spinBoxDelegate.from, spinBoxDelegate.to)
                top: Math.max(spinBoxDelegate.from, spinBoxDelegate.to)
            }

            textFromValue: function(value, locale) {
                return Number(value/10).toLocaleString(locale, 'f', 1);
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 10;
            }

            function setValue(value) {
                spinBoxDelegate.value = value * 10;
            }

            onValueChanged: {
                if (!repeater.isUpdating) {
                    repeater.isUpdating = true;
                    var range = Math.abs(spinBoxDelegate.to - spinBoxDelegate.from);
                    var currVal = spinBoxDelegate.value / 10 + range/2000;
                    sliderDelegate.value = currVal;
                    repeater.isUpdating = false;
                }
            }
        }

        Slider {
            id: sliderDelegate
            to: 40
            from: 0
            Layout.preferredWidth: parent.width
            Layout.fillHeight: true
            orientation: Qt.Vertical
            stepSize: 0.1
            snapMode: Slider.SnapAlways
            value: 20

            onVisualPositionChanged: {
                if (!repeater.isUpdating) {
                    repeater.isUpdating = true;
                    var range = Math.abs(sliderDelegate.to - sliderDelegate.from);
                    var currVal = range/2 - (sliderDelegate.visualPosition * range);
                    spinBoxDelegate.setValue(currVal);
                    repeater.isUpdating = false;
                }
            }
        }

        Label {
            text: modelData
            Layout.preferredWidth: parent.width
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }
}
