import QtQuick 2.4

Item {
	id: windowButtons
	property color baseColor: "#000000"
	property bool disableMinimize: false
	signal minimizeButtonClicked()
	signal closeButtonClicked()
	width: 56; height: 24

	Rectangle {
		id: minimizeButton
		width: 28; height: parent.height
		color: "#3665b3"
		border.width: 0
		state: "NORMAL"
		visible: !disableMinimize
		anchors.right: closeButton.left
		z: 1

		Image {
			id: minimizeButtonImg
			width: 28; height: parent.height
			sourceSize.width: parent.height
			sourceSize.height: parent.height
			source: "qrc:/Resources/MinimizeButton.png"
			fillMode: Image.Tile
			z: 2
		}

		MouseArea {
			id: minimizeArea
			hoverEnabled: true
			anchors.fill: parent
			z: 3

			onEntered: minimizeButton.state == "HOVER" ? minimizeButton.state = "PRESSED" : minimizeButton.state = "HOVER"
			onExited: minimizeButton.state == "PRESSED" ? minimizeButton.state = "HOVER" : minimizeButton.state = "NORMAL"
			onPressed: minimizeButton.state = "PRESSED"
			onReleased: minimizeButton.state = (containsMouse) ? "RELEASED" : "NORMAL"
			onClicked: windowButtons.minimizeButtonClicked()
		}

		states: [
			State {name: "NORMAL"; PropertyChanges {target: minimizeButton; color: baseColor}},
			State {name: "HOVER"; PropertyChanges {target: minimizeButton; color: "#3665b3"}},
			State {name: "PRESSED"; PropertyChanges {target: minimizeButton; color: "#3d6099"}},
			State {name: "RELEASED"; PropertyChanges {target: minimizeButton; color: "#3665b3"}}
		]

		transitions: [
			Transition {from: "NORMAL"; to: "HOVER"; ColorAnimation {target: minimizeButton; duration: 100}},
			Transition {from: "HOVER"; to: "NORMAL"; ColorAnimation {target: minimizeButton; duration: 100}},
			Transition {from: "HOVER"; to: "PRESSED"; ColorAnimation {target: minimizeButton; duration: 0}},
			Transition {from: "PRESSED"; to: "RELEASED"; ColorAnimation {target: minimizeButton; duration: 100}},
			Transition {from: "PRESSED"; to: "NORMAL"; ColorAnimation {target: minimizeButton; duration: 100}}
		]
	}

	Rectangle {
		id: closeButton
		width: 28; height: parent.height
		color: "#00000000"
		border.width: 0
		state: "NORMAL"
		anchors.right: windowButtons.right
		z: 1

		Image {
			id: closeButtonImg
			width: 28; height: parent.height
			visible: true
			z: 2
			sourceSize.height: parent.height
			sourceSize.width: parent.height
			fillMode: Image.Tile
			source: "qrc:/Resources/CloseButton.png"
		}

		MouseArea {
			id: closeArea
			hoverEnabled: true
			anchors.fill: parent
			z: 3

			onEntered: closeButton.state = "HOVER"
			onExited: closeButton.state = "NORMAL"
			onPressed: closeButton.state = "PRESSED"
			onReleased: {closeButton.state = (containsMouse) ? "RELEASED" : "NORMAL"}
			onClicked: closeButtonClicked()
		}

		states: [
			State {name: "NORMAL"; PropertyChanges {target: closeButton; color: baseColor}},
			State {name: "HOVER"; PropertyChanges {target: closeButton; color: "#e04343"}},
			State {name: "PRESSED"; PropertyChanges {target: closeButton; color: "#993d3d"}},
			State {name: "RELEASED"; PropertyChanges {target: closeButton; color: "#e04343"}}
		]

		transitions: [
			Transition {from: "NORMAL"; to: "HOVER"; ColorAnimation {target: closeButton; duration: 100}},
			Transition {from: "HOVER"; to: "NORMAL"; ColorAnimation {target: closeButton; duration: 100}},
			Transition {from: "HOVER"; to: "PRESSED"; ColorAnimation {target: closeButton; duration: 0}},
			Transition {from: "PRESSED"; to: "RELEASED"; ColorAnimation {target: closeButton; duration: 100}},
			Transition {from: "PRESSED"; to: "NORMAL"; ColorAnimation {target: closeButton; duration: 100}}
		]
	}
}
