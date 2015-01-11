import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

Window {
	id: deleteDialog
	signal yesButtonClicked(int type, string configName)
	signal cancelButtonClicked(int type)
	property color baseColor: "#000000"
	property color fontColor: "#ffffff"
	property string configName: ""
	width: 420; height: 78 + deleteDescription.height

	Rectangle {
		id: backdrop
		color: baseColor
		border.color: Qt.tint(fontColor, "#00000088")
		border.width: 1
		anchors.fill: parent
		z: 0
	}

	WindowFunctions {
		disableMinimize: true
		baseColor: deleteDialog.baseColor
		anchors.top: backdrop.top
		anchors.topMargin: 1
		anchors.right: backdrop.right
		anchors.rightMargin: 1
		z: 1

		onCloseButtonClicked: deleteDialog.cancelButtonClicked(3)
	}

	Label {
		id: deleteDescription
		height: paintedHeight
		color: deleteDialog.fontColor
		text: qsTr("Are you sure you want to delete \"" + configName + "\"? This cannot be undone.")
		textFormat: Text.PlainText
		wrapMode: Text.WordWrap
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
		anchors.right: cancelButton.left
		anchors.rightMargin: 10
		anchors.top: deleteDescription.bottom
		anchors.topMargin: 10

		onClicked: deleteDialog.yesButtonClicked(3, configName)
	}

	Button {
		id: cancelButton
		width: yesButton.width; height: yesButton.height
		text: qsTr("Cancel")
		isDefault: true
		anchors.right: backdrop.right
		anchors.rightMargin: 11
		anchors.top: yesButton.top

		onClicked: deleteDialog.cancelButtonClicked(3)
	}
}

