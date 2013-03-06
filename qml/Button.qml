import QtQuick 1.0

Rectangle {
	id: button
	property color buttonColor: "lightblue"
	property color onHoverColor: "gold"
	property color borderColor: "white"
	property string label
	width: 150
	height: 60
	radius: 10

	MouseArea {
		id: buttonMouseArea
		hoverEnabled: true
		anchors.fill: parent
		onEntered: parent.border.color = onHoverColor
		onExited: parent.border.color = borderColor
	}

	Text {
		text: label
		anchors.centerIn: parent
	}

	//determines the color of the button by using the conditional operator
	color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
}
