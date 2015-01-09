import QtQuick 2.4
import QtQuick.Window 2.2

Rectangle {
	id: mainWindow
	width: 1056; height: 542
	color: "#3b3b3b"
	visible: true

	MouseArea {
		anchors.fill: parent
		z: 1

		onPressed: windowFunc.beginMoveWindow(mouse.x, mouse.y, parent.width, parent.height)
		onPositionChanged: windowFunc.moveWindow(mouse.x, mouse.y)
	}

	WindowFunctions {
		x: mainWindow.width - width; y: 0
		baseColor: "#3b3b3b"
		z: 2

		onMinimizeButtonClicked: windowFunc.minimizeWindow()
		onCloseButtonClicked: windowFunc.closeWindow()
	}

	MainForm {
		x: 0; y: 25
		z: 2
    }
}
