import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
	id: band
	width: 42; height: 356

	Rectangle {
		id: rectangle1
		color: "#ffffff"
		visible: false
		anchors.fill: parent
	}

	TextField {
		id: volumeValue
		text: "0.0"
		font.pointSize: 8
		font.family: "Verdana"
		anchors.right: parent.right
		anchors.rightMargin: 0
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.top: parent.top
		anchors.topMargin: 0
		placeholderText: qsTr("")
	}

	Slider {
		id: volumeSlider
		x: 0; y: 29
		width: 42; height: 300
		minimumValue: -20
		maximumValue: 20
		stepSize: 1
		tickmarksEnabled: true
		orientation: Qt.Vertical

		style: SliderStyle {
			id: sliderStyle

			tickmarks: Row {
				id: tickmarksRight
				x: 7
				height: volumeSlider.height - 20
				spacing: tickmarksRight.height / 23
				Repeater {
					model: 21
					Item {
						width: 1; height: 26
						Rectangle {
							color: "#f6f6f6"
							y: 7
							width: 1; height: 3
						}
						Rectangle {
							color: "#f6f6f6"
							y: volumeSlider.width - 10
							width: 1; height: 3
						}
					}
				}
			}
		}
	}

	TextField {
		id: frequencyValue
		y: 8
		text: "0"
		font.pointSize: 8
		font.family: "Verdana"
		anchors.right: parent.right
		anchors.rightMargin: 0
		anchors.left: parent.left
		anchors.leftMargin: 0
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 0
		placeholderText: qsTr("")
	}
}
