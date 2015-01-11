import QtQuick 2.4
import QtQuick.Window 2.2

Window {
	id: mainWindow
	objectName: "mainWindow"
	signal movePressed(int x, int y, int width, int height)
	signal moveMoved(int x, int y)
	signal minimizeButtonClicked()
	signal closeButtonClicked()
	property color baseColor: "#3b3b3b"
	property color fontColor: "#f6f6f6"
	width: 1058; height: 544

	Rectangle {
		color: baseColor
		border.color: fontColor
		border.width: 1
		anchors.fill: parent
	}

	MouseArea {
		objectName: "moveMouseArea"
		anchors.fill: parent
		z: 1

		onPressed: mainWindow.movePressed(mouse.x, mouse.y, parent.width, parent.height)
		onPositionChanged: mainWindow.moveMoved(mouse.x, mouse.y)
	}

	WindowFunctions {
		objectName: "windowFunctions"
		baseColor: mainWindow.baseColor
		anchors.top: parent.top
		anchors.topMargin: 1
		anchors.right: parent.right
		anchors.rightMargin: 1
		z: 2

		onMinimizeButtonClicked: mainWindow.minimizeButtonClicked()
		onCloseButtonClicked: mainWindow.closeButtonClicked()
	}

	MainForm {
		objectName: "mainForm"
		x: 1; y: 25
		z: 2
	}
}
