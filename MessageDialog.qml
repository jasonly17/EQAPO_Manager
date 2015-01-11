import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2

Window {
	id: messageDialog
	signal okButtonClicked(int type)
	property color baseColor: "#000000"
	property color fontColor: "#ffffff"
	property string message: ""
	width: 422; height: 78 + messageLabel.height

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
		baseColor: messageDialog.baseColor
		anchors.top: backdrop.top
		anchors.topMargin: 1
		anchors.right: backdrop.right
		anchors.rightMargin: 1
		z: 1

		onCloseButtonClicked: messageDialog.okButtonClicked(0)
	}

	Label {
		id: messageLabel
		height: paintedHeight
		color: messageDialog.fontColor
		text: qsTr(messageDialog.message)
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
		id: okButton
		width: 75; height: 22
		text: qsTr("Ok")
		isDefault: true
		anchors.right: backdrop.right
		anchors.rightMargin: 11
		anchors.top: messageLabel.bottom
		anchors.topMargin: 10

		onClicked: messageDialog.okButtonClicked(0)
	}
}

