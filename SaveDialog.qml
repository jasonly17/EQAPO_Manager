import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

Window {
	id: saveDialog
	signal okButtonClicked(int type, string configName, string lastConfig)
	signal cancelButtonClicked(int type)
	property color baseColor: "#000000"
	property color fontColor: "#ffffff"
	property string lastConfig: ""
	width: 422; height: 128

	Rectangle {
		id: backdrop
		color: baseColor
		border.color: fontColor
		border.width: 1
		anchors.fill: parent
		z: 0
	}

	WindowFunctions {
		disableMinimize: true
		baseColor: saveDialog.baseColor
		anchors.top: backdrop.top
		anchors.topMargin: 1
		anchors.right: backdrop.right
		anchors.rightMargin: 1
		z: 1

		onCloseButtonClicked: saveDialog.cancelButtonClicked(1)
	}

	Label {
		id: saveDescription
		height: 16
		color: saveDialog.fontColor
		text: qsTr("Enter a name for your preset:")
		textFormat: Text.PlainText
		font.pointSize: 10
		font.family: "Verdana"
		anchors.left: backdrop.left
		anchors.leftMargin: 11
		anchors.right: backdrop.right
		anchors.rightMargin: 11
		anchors.top: backdrop.top
		anchors.topMargin: 35
	}

	TextField {
		id: configNameInput
		height: 24
		font.pointSize: 8
		font.family: "Verdana"
		maximumLength: 36
		placeholderText: qsTr("")
		focus: true
		anchors.left: saveDescription.left
		anchors.right: saveDescription.right
		anchors.top: saveDescription.bottom
		anchors.topMargin: 10
		z: 1

		onAccepted: saveDialog.okButtonClicked(1, configNameInput.text, lastConfig)
	}

	Button {
		id: okButton
		width: 75; height: 22
		text: qsTr("Ok")
		isDefault: true
		anchors.right: cancelButton.left
		anchors.rightMargin: 10
		anchors.top: configNameInput.bottom
		anchors.topMargin: 10

		onClicked: saveDialog.okButtonClicked(1, configNameInput.text, lastConfig)
	}

	Button {
		id: cancelButton
		width: okButton.width; height: okButton.height
		text: qsTr("Cancel")
		anchors.right: backdrop.right
		anchors.rightMargin: 11
		anchors.top: okButton.top

		onClicked: cancelButtonClicked(1)
	}
}

