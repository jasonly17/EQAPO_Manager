import QtQuick 2.7
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import "."

Item {
    id: root
    width: 40
    height: 40

    property bool isEnabled: false

    signal clicked(bool enabled)

    RectangularGlow {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 1.5
        width: parent.width
        height: parent.height
        glowRadius: 0.75
        opacity: 0.4
        spread: 0.7
        color: "black"
        cornerRadius: height/2
    }

    Rectangle {
        color: (isEnabled) ? Styles.secondaryColor : Styles.primaryColor
        border.color: Styles.secondaryColor
        radius: width/2
        border.width: 2
        anchors.fill: parent

        Canvas {
            id: canvas
            anchors.fill: parent
            anchors.margins: 8
            antialiasing: true

            onPaint: {
                var ctx = getContext("2d");
                ctx.save();

                ctx.clearRect(0, 0, canvas.width, canvas.height);

                ctx.beginPath();
                ctx.lineWidth = 3;
                ctx.lineCap = "round";
                ctx.strokeStyle = (isEnabled) ? Styles.primaryColor : Styles.secondaryColor;
                ctx.arc(canvas.width/2,
                        canvas.height/2,
                        canvas.width/2 - ctx.lineWidth/2,
                        -Math.PI*1/4,
                        Math.PI*5/4);
                ctx.stroke();

                ctx.beginPath();
                ctx.lineWidth = 3;
                ctx.lineCap = "round";
                ctx.strokeStyle = (isEnabled) ? Styles.primaryColor : Styles.secondaryColor;
                ctx.moveTo(canvas.width/2, 0);
                ctx.lineTo(canvas.width/2, canvas.height/2);
                ctx.stroke();

                ctx.restore();
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            isEnabled = !isEnabled;
            canvas.requestPaint();
            root.focus = true;
            root.clicked(isEnabled);
        }
    }
}
