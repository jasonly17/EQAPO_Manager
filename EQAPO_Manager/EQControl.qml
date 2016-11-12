import QtQuick 2.4
import QtQuick.Controls 2.0
import "."

EQControlForm {
    property bool isUpdating: false

    powerButton.onClicked: {
        console.log("Clicked");
    }

    preampSlider.onVisualPositionChanged: {
        if (!isUpdating) {
            isUpdating = true;
            var range = Math.abs(preampSlider.to - preampSlider.from);
            var currVal = -(range - (preampSlider.visualPosition * range));
            preampSpinBox.value = currVal;
            isUpdating = false;
        }
    }

    preampSpinBox.onValueChanged: {
        if (!isUpdating) {
            isUpdating = true;
            var range = Math.abs(preampSpinBox.to - preampSpinBox.from);
            var currVal = preampSpinBox.value + range;
            preampSlider.value = currVal;
            isUpdating = false;
        }
    }
}
