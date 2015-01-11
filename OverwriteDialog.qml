import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

Window {
	id: overwriteDialog
	signal yesButtonClicked(int type, string configName, string lastConfig)
	signal discardButtonClicked()
	signal cancelButtonClicked(int type)
	property color baseColor: "#000000"
	property color fontColor: "#ffffff"
	property string configName: ""
	property string lastConfig: ""
	width: 422; height: 110

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
		baseColor: overwriteDialog.baseColor
		anchors.top: backdrop.top
		anchors.topMargin: 1
		anchors.right: backdrop.right
		anchors.rightMargin: 1
		z: 1

		onCloseButtonClicked: overwriteDialog.discardButtonClicked()
	}

	Label {
		id: overwriteDescription
		height: 32
		color: overwriteDialog.fontColor
		text: qsTr("There is already a preset named \"" + configName +
				   "\". Do you want to overwrite the existing preset?")
		wrapMode: Text.WordWrap
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


	Button {
		id: yesButton
		width: 75; height: 22
		text: qsTr("Yes")
		anchors.right: discardButton.left
		anchors.rightMargin: 10
		anchors.top: overwriteDescription.bottom
		anchors.topMargin: 10

		onClicked: overwriteDialog.yesButtonClicked(2, configName, lastConfig)
	}

	Button {
		id: discardButton
		width: yesButton.width; height: yesButton.height
		text: qsTr("Discard")
		isDefault: true
		anchors.right: cancelButton.left
		anchors.rightMargin: 10
		anchors.top: yesButton.top

		onClicked: overwriteDialog.discardButtonClicked()
	}

	Button {
		id: cancelButton
		width: yesButton.width; height: yesButton.height
		text: qsTr("Cancel")
		anchors.right: backdrop.right
		anchors.rightMargin: 11
		anchors.top: yesButton.top

		onClicked: overwriteDialog.cancelButtonClicked(2)
	}
}

