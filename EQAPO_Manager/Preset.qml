import QtQuick 2.7

PresetForm {
    // fix for combo box not initially displaying text
    Component.onCompleted: {
        presetComboBox.currentIndex = 0;
    }

    presetComboBox.model: [ "Flat", "test" ]
}
