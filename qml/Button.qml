import QtQuick 1.0

Item {
	id: button
	property color buttonColor: "lightblue"
	property color onHoverColor: "gold"
	property color borderColor: "white"
	property string label

	Rectangle {
		border.width: 3
		border.color: borderColor
		height: parent.height
		width: parent.width
		radius: 10

		//determines the color of the button by using the conditional operator
		color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

		MouseArea {
			id: buttonMouseArea
			hoverEnabled: true
			anchors.fill: parent
			onEntered: parent.border.color = onHoverColor
			onExited: parent.border.color = borderColor
		}

		Text {
			id: myText
			text: label
			anchors.centerIn: parent
		}

	}
}
