import QtQuick 2.4
import QtQuick.Controls.Styles 1.3
import QtQuick.Controls 1.3

Item {
    id: band
	signal volumeChanged()
	signal freqChanged()
    property int bandNum: 0
    width: 42; height: 356

    Rectangle {
		id: rectangle
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

        onEditingFinished: {
			if (volumeSlider.user){
                volumeValue.text = Math.max(-20, Math.min(20, volumeValue.text))
                volumeSlider.value = Math.round(volumeValue.text * 10) / 10
            }
        }
    }

    Slider {
        id: volumeSlider
		objectName: "volumeSlider"
		property bool user: true
        x: 0; y: 29
        width: 42; height: 300
        minimumValue: -20
        maximumValue: 20
        stepSize: 0.5
        tickmarksEnabled: true
        orientation: Qt.Vertical

        style: SliderStyle {
            id: sliderStyle

            tickmarks: Row {
                id: tickmarksRight
                x: 7
                height: volumeSlider.height - 20
                spacing: tickmarksRight.height / 22.6
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

        onValueChanged: {
			volumeSlider.user = false
            volumeValue.text = Math.round(volumeSlider.value * 10) / 10
			band.volumeChanged()
			volumeSlider.user = true
        }
    }

    TextField {
        id: freqValue
		objectName: "freqValue"
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

		onTextChanged: band.freqChanged()
    }
}
