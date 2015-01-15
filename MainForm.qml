import QtQuick 2.4
import QtQuick.Controls 1.3

Rectangle {
	id: backdrop
	signal powerButtonClicked(bool checked)
	signal preampChanged(double value)
	signal comboBoxIndexChanged(string text)
	signal resetButtonClicked()
	signal saveButtonClicked()
	signal deleteButtonClicked()
	signal bandChanged(int type)
	property color fontColor: "#ffffff"
	width: 1056; height: 518
	color: "#00000000"

	Item {
		id: preampGroup
		height: 60
		anchors.right: parent.right
		anchors.rightMargin: 0
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.top: parent.top
		anchors.topMargin: 0

		Button {
			id: powerButton
			objectName: "powerButton"
			width: 40; height: 40
			text: "Power"
			checkable: true
			checked: true
			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.top: parent.top
			anchors.topMargin: 10
			z: 1

			onClicked: backdrop.powerButtonClicked(powerButton.checked)
		}

		Label {
			id: preampLabel
			width: 60
			height: 40
			color: backdrop.fontColor
			text: qsTr("Preamp:")
			textFormat: Text.PlainText
			font.pointSize: 10
			font.family: "Verdana"
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			enabled: false
			anchors.verticalCenterOffset: 0
			anchors.verticalCenter: powerButton.verticalCenter
			anchors.left: powerButton.right
			anchors.leftMargin: 10
			z: 1
		}

		Slider {
			id: preampSlider
			objectName: "preampSlider"
			property bool user: true
			height: 22
			stepSize: 0.1
			orientation: Qt.Horizontal
			maximumValue: 0
			minimumValue: -30
			value: 0
			anchors.verticalCenter: powerButton.verticalCenter
			anchors.right: preampValue.left
			anchors.rightMargin: 10
			anchors.left: preampLabel.right
			anchors.leftMargin: 10
			z: 1

			onValueChanged: {
				preampSlider.user = false
				preampValue.text = Math.round(preampSlider.value * 10) / 10
				backdrop.preampChanged(preampValue.text)
				preampSlider.user = true
			}
		}

		TextField {
			id: preampValue
			width: 40; height: 20
			text: "0"
			font.pointSize: 8
			font.family: "Verdana"
			placeholderText: qsTr("")
			anchors.verticalCenter: powerButton.verticalCenter
			anchors.right: preampdBLabel.left
			anchors.rightMargin: 10
			z: 1

			onEditingFinished: {
				if (preampSlider.user){
					preampValue.text = Math.max(-30, Math.min(0, preampValue.text))
					preampSlider.value = Math.round(preampValue.text * 10) / 10
				}
			}
		}

		Label {
			id: preampdBLabel
			width: 20; height: 40
			color: backdrop.fontColor
			text: qsTr("dB")
			textFormat: Text.PlainText
			enabled: false
			font.pointSize: 10
			font.family: "Verdana"
			font.bold: false
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			anchors.verticalCenter: powerButton.verticalCenter
			anchors.right: parent.right
			anchors.rightMargin: 10
			anchors.top: parent.top
			anchors.topMargin: 10
			z: 1
		}
	}

	Rectangle {
		id: preampDivider
		height: 1
		color: backdrop.fontColor
		anchors.top: preampGroup.bottom
		anchors.topMargin: 0
		anchors.right: parent.right
		anchors.rightMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 10
	}

	Item {
		id: optionsGroup
		x: 72
		width: 563; height: 40
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: preampDivider.bottom
		anchors.topMargin: 0

		Label {
			id: presetLabel
			height: 30
			color: backdrop.fontColor
			text: qsTr("Preset:")
			textFormat: Text.PlainText
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			font.family: "Verdana"
			font.pointSize: 10
			anchors.verticalCenter: parent.verticalCenter
			enabled: false
			anchors.left: parent.left
			anchors.leftMargin: 0
			z: 1
		}

		ComboBox {
			id: presetComboBox
			objectName: "presetComboBox"
			width: 250; height: 24
			model: comboBoxModel
			anchors.verticalCenter: presetLabel.verticalCenter
			anchors.left: presetLabel.right
			anchors.leftMargin: 10
			z: 1

			onCurrentIndexChanged: backdrop.comboBoxIndexChanged(presetComboBox.currentText)
		}

		Button {
			id: presetResetButton
			height: 24
			text: qsTr("Reset")
			anchors.verticalCenter: presetLabel.verticalCenter
			anchors.left: presetComboBox.right
			anchors.leftMargin: 10
			z: 1

			onClicked: backdrop.resetButtonClicked()
		}

		Button {
			id: presetSaveButton
			height: 24
			text: qsTr("Save")
			anchors.left: presetResetButton.right
			anchors.leftMargin: 10
			anchors.verticalCenter: presetLabel.verticalCenter
			z: 1

			onClicked: backdrop.saveButtonClicked()
		}

		Button {
			id: presetDeleteButton
			height: 24
			text: qsTr("Delete")
			anchors.left: presetSaveButton.right
			anchors.leftMargin: 10
			anchors.verticalCenter: presetLabel.verticalCenter
			z: 1

			onClicked: backdrop.deleteButtonClicked()
		}
	}

	Item {
		id: equalizerGroup
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 10
		anchors.top: optionsGroup.bottom
		anchors.topMargin: 0
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.right: parent.right
		anchors.rightMargin: 0

		Text {
			id: levelLabel
			x: 16
			color: backdrop.fontColor
			text: qsTr("Level (dB)")
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			enabled: false
			z: 1
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.top: parent.top
			anchors.topMargin: 0
			font.pointSize: 10
			font.family: "Verdana"
			textFormat: Text.PlainText
		}

		Column {
			id: dBLabels
			width: 40
			visible: true
			spacing: 127
			anchors.left: parent.left
			anchors.leftMargin: 10
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 0
			anchors.top: parent.top
			anchors.topMargin: 58

			Text {
				id: maxdBLabel
				color: backdrop.fontColor
				text: qsTr("20 dB")
				anchors.right: parent.right
				anchors.rightMargin: 0
				anchors.left: parent.left
				anchors.leftMargin: 0
				enabled: false
				horizontalAlignment: Text.AlignRight
				font.pointSize: 9
				z: 1
				font.family: "Verdana"
				verticalAlignment: Text.AlignVCenter
				textFormat: Text.PlainText
			}

			Text {
				id: neutraldBLabel
				color: backdrop.fontColor
				text: qsTr("0 dB")
				anchors.right: parent.right
				enabled: false
				horizontalAlignment: Text.AlignRight
				anchors.leftMargin: 0
				font.pointSize: 9
				anchors.rightMargin: 0
				z: 1
				anchors.left: parent.left
				font.family: "Verdana"
				verticalAlignment: Text.AlignVCenter
				textFormat: Text.PlainText
			}

			Text {
				id: mindBLabel
				color: backdrop.fontColor
				text: qsTr("-20 dB")
				anchors.right: parent.right
				enabled: false
				horizontalAlignment: Text.AlignRight
				anchors.leftMargin: 0
				anchors.rightMargin: 0
				font.pointSize: 9
				z: 1
				anchors.left: parent.left
				font.family: "Verdana"
				verticalAlignment: Text.AlignVCenter
				textFormat: Text.PlainText
			}
		}

		Row {
			id: bandsGroup
			spacing: 8
			anchors.bottom: frequencyLabel.top
			anchors.bottomMargin: 10
			anchors.right: parent.right
			anchors.rightMargin: 10
			anchors.left: dBLabels.right
			anchors.leftMargin: 6
			anchors.top: levelLabel.bottom
			anchors.topMargin: 10

			Repeater {
				objectName: "bands"
				model: 20
				BandSliderForm {
					bandNum: index

					onFreqChanged: backdrop.bandChanged(0)
					onVolumeChanged: backdrop.bandChanged(1)
				}
			}
		}

		Text {
			id: frequencyLabel
			x: 25
			y: 7
			color: backdrop.fontColor
			text: qsTr("Frequency (Hz)")
			anchors.bottom: parent.bottom
			anchors.bottomMargin: 0
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
			enabled: false
			font.pointSize: 10
			anchors.horizontalCenter: parent.horizontalCenter
			z: 1
			font.family: "Verdana"
			textFormat: Text.PlainText
		}
	}
}
